/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/12 16:02:48 by fpedroso          #+#    #+#             */
/*   Updated: 2026/03/12 16:02:48 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_hdoc_dollar(char *line, int *i, char *res, char **envs)
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

char	*append_hdoc_literal(char *line, int *i, char *result)
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

bool	delim_has_quotes(char *mask)
{
	int	i;

	if (!mask)
		return (false);
	i = 0;
	while (mask[i])
	{
		if (mask[i] == CONTEXT_SINGLE || mask[i] == CONTEXT_DOUBLE)
			return (true);
		i++;
	}
	return (false);
}

void	write_to_file(int fd, char *line)
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
