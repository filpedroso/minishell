/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_get_cmd_path.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 00:00:00 by fpedroso          #+#    #+#             */
/*   Updated: 2026/02/23 21:31:37 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_explicit_path(char *cmd_arg);
static char	*search_in_path_var(t_cmd *cmd);
static char	*get_path_env(char **env_vars);

char	*get_cmd_path(t_cmd *cmd)
{
	char	*cmd_str_name_ptr;

	cmd_str_name_ptr = cmd->words[0].token_word_ptr;
	if (!cmd_str_name_ptr)
		return (NULL);
	if (is_explicit_path(cmd_str_name_ptr))
	{
		if (access(cmd_str_name_ptr, X_OK) == 0)
			return (ft_strdup(cmd_str_name_ptr));
		else
			return (NULL);
	}
	return (search_in_path_var(cmd));
}

static bool	is_explicit_path(char *cmd_arg)
{
	if (ft_strchr(cmd_arg, '/'))
		return (true);
	else
		return (false);
}

static char	*search_in_path_var(t_cmd *cmd)
{
	char	*path_env_var;
	char	**current_env_vars;

	current_env_vars = get_current_envs(cmd->env_vars);
	if (!current_env_vars)
		return (NULL);
	path_env_var = get_path_env(current_env_vars);
	if (!path_env_var)
		return (NULL);
	return (find_in_path(cmd->words[0].token_word_ptr, path_env_var));
}

static char	*get_path_env(char **env_vars)
{
	int	i;

	if (!env_vars)
		return (NULL);
	i = 0;
	while (env_vars[i])
	{
		if (ft_strncmp(env_vars[i], "PATH=", 5) == 0)
			return (env_vars[i] + 5);
		i++;
	}
	return (NULL);
}
