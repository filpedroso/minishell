/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 00:00:00 by fpedroso          #+#    #+#             */
/*   Updated: 2025/12/01 18:17:05 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static int	is_explicit_path(char *cmd_arg);
static char	*search_in_path_var(t_command cmd);
static char	*get_path_env(char **env_vars);

char	*get_cmd_path(t_command cmd)
{
	char	*cmd_path;

	if (!cmd.args)
		return (NULL);
	if (is_explicit_path(cmd.args[0]))
	{
		if (access(cmd.args[0], X_OK) == 0)
			return (ft_strdup(cmd.args[0]));
		return (NULL);
	}
	else
		cmd_path = search_in_path_var(cmd);
	if (!cmd_path)
	{
		ft_putstr_fd("Command not found:", 2);
		ft_putstr_fd(cmd.args[0], 2);
		return (NULL);
	}
	return (cmd_path);
}

static int	is_explicit_path(char *cmd_arg)
{
	return (!ft_strchr(cmd_arg, '/'));
}

static char	*search_in_path_var(t_command cmd)
{
	char	*path_env_var;
	char	**current_env_vars;

	current_env_vars = get_current_envs(cmd.env_vars);
	if (!current_env_vars)
		return (NULL);
	path_env_var = get_path_env(current_env_vars);
	if (!path_env_var)
		return (NULL);
	return (find_in_path(cmd.args[0], path_env_var));
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
