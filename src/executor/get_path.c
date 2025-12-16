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

static char	*find_in_path(char *cmd, char *path_env);
static char	*get_path_env(char **envp);
static char	*join_path_with_cmd(char *dir, char *cmd);
static char	*ft_strdup(const char *s);

/*
 * Searches for the executable in PATH and returns the full path.
 * If the command contains a '/' it's treated as an absolute/relative path.
 * Returns the full path to the executable or NULL if not found.
 */
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

char	*search_in_path_var(t_command cmd)
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


int	is_explicit_path(char *cmd_arg)
{
	return (!ft_strchr(cmd_arg, '/'));
}

/*
 * Extracts the PATH value from the environment variables
 */
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

static char	*find_in_path(char *cmd_str, char *path_env)
{
	char	**paths;

	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	return (find_correct_command_path(paths, cmd_str));
}

char	*find_correct_command_path(char **paths, char *cmd_str)
{
	int		i;
	char	*current_full_path;

	i = 0;
	while (paths[i])
	{
		current_full_path = join_path_with_cmd(paths[i], cmd_str);
		if (!current_full_path)
		{
			free_str_arr(paths);
			return (NULL);
		}
		if (access(current_full_path, X_OK) == 0)
		{
			free_str_arr(paths);
			return (current_full_path);
		}
		free(current_full_path);
		i++;
	}
	free_str_arr(paths);
	return (NULL);
}

static char	*join_path_with_cmd(char *dir, char *cmd)
{
	char	*path_with_dash;

	if (!dir || !cmd)
		return (NULL);
	if (dir[ft_strlen(dir) - 1] != '/')
	{
		path_with_dash = ft_strjoin(dir, "/");
		return (ft_strjoin(path_with_dash, cmd));
	}
	else
		return (ft_strjoin(dir, cmd));
}
