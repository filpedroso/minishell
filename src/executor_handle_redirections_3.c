/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_handle_redirections_3.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 16:47:47 by fpedroso          #+#    #+#             */
/*   Updated: 2026/03/09 17:08:45 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*create_temp_filepath(void);
static void	read_input_into_file(int fd, char *delim, char *delim_mask,
				t_env_vars env_vars);
static char	*heredoc_sigint_cleanup(int saved_stdin, char *filepath);
static char	*expand_hdoc_dollar(char *line, int *i, char *res, char **envs);
static char	*append_hdoc_literal(char *line, int *i, char *result);
static bool	delim_has_quotes(char *mask);
static void	write_to_file(int fd, char *line);

char	*create_temp_file(char *delim, char *delim_mask, t_env_vars env_vars)
{
	char	*filepath;
	int		fd;
	int		stdin_bkp;

	filepath = create_temp_filepath();
	if (!filepath)
		return (NULL);
	fd = open(filepath, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (fd < 0)
	{
		free(filepath);
		return (NULL);
	}
	stdin_bkp = dup(STDIN_FILENO);
	set_signals_heredoc();
	read_input_into_file(fd, delim, delim_mask, env_vars);
	close(fd);
	set_signals_interactive();
	if (g_signal == SIGINT)
		return (heredoc_sigint_cleanup(stdin_bkp, filepath));
	close(stdin_bkp);
	return (filepath);
}

static char	*heredoc_sigint_cleanup(int saved_stdin, char *filepath)
{
	dup2(saved_stdin, STDIN_FILENO);
	close(saved_stdin);
	unlink(filepath);
	free(filepath);
	return (NULL);
}

static char	*create_temp_filepath(void)
{
	static int	counter = 0;
	char		*num_str;
	char		*new_filepath;

	num_str = ft_itoa(counter++);
	new_filepath = ft_strjoin("/tmp/heredoc_", num_str);
	free(num_str);
	return (new_filepath);
}

static void	read_input_into_file(int fd, char *delim, char *delim_mask,
				t_env_vars env_vars)
{
	char	*line;
	char	*expanded;

	while (1)
	{
		line = readline(">> ");
		if (!line || ft_strncmp(line, delim, ft_strlen(delim) + 1) == 0)
		{
			free(line);
			break ;
		}
		if (delim_has_quotes(delim_mask))
			write_to_file(fd, line);
		else
		{
			expanded = expand_line(line, env_vars);
			write_to_file(fd, expanded);
			free(expanded);
		}
		free(line);
	}
}

static char	*expand_hdoc_dollar(char *line, int *i, char *res, char **envs)
{
	int		var_len;
	char	*value;

	(*i)++;
	if (!line[*i] || (!ft_isalpha(line[*i]) && line[*i] != '_'))
		return (join_and_free_left(res, "$"));
	var_len = 1;
	while (line[*i + var_len] && (ft_isalnum(line[*i + var_len])
			|| line[*i + var_len] == '_'))
		var_len++;
	value = lookup_env_var(&line[*i], var_len, envs);
	*i += var_len;
	res = join_and_free_left(res, value);
	free(value);
	return (res);
}

static char	*append_hdoc_literal(char *line, int *i, char *result)
{
	int		start;
	char	*seg;

	start = *i;
	while (line[*i] && line[*i] != '$')
		(*i)++;
	seg = ft_substr(line, start, *i - start);
	if (!seg)
	{
		free(result);
		return (NULL);
	}
	result = join_and_free_left(result, seg);
	free(seg);
	return (result);
}

char	*expand_line(char *line, t_env_vars env_vars)
{
	char	**envs;
	char	*result;
	int		i;

	if (!ft_strchr(line, '$'))
		return (ft_strdup(line));
	envs = get_current_envs(env_vars);
	if (!envs)
		return (NULL);
	result = ft_strdup("");
	i = 0;
	while (result && line[i])
	{
		if (line[i] == '$')
			result = expand_hdoc_dollar(line, &i, result, envs);
		else
			result = append_hdoc_literal(line, &i, result);
	}
	free_str_arr(envs);
	return (result);
}

static bool	delim_has_quotes(char *mask)
{
	int	i;

	if (!mask)
		return (false);
	i = 0;
	while(mask[i])
	{
		if (mask[i] == CONTEXT_SINGLE || mask[i] == CONTEXT_DOUBLE)
			return (true);
		i++;
	}
	return (false);
}

static void	write_to_file(int fd, char *line)
{
	if (!line)
	{
		write(fd, "HEREDOC EXPANSION ERROR", 23);
		write(fd, "\n", 1);
		return ;
	}
	write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
}

/* 
int		status;
	char	*dollar;
	char	**envp;
	char	*expanded;

	dollar = ft_strchr(line, '$');
	if (!dollar)
		return (line);
	envp = get_current_envs(env_vars);
	if (!envp)
		return (NULL);
	expanded = NULL;
	while (dollar)
	{
		status = append_up_to_next_var(expanded, dollar);
		if (status == STATUS_ERR)
		{
			free_str_arr(envp);
			return (NULL);
		}
		dollar = ft_strchr(line + 1, '$');
	}
 */