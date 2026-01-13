/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 20:02:12 by fpedroso          #+#    #+#             */
/*   Updated: 2025/12/16 20:02:12 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static char	*find_correct_command_path(char **paths, char *cmd_str);
static char	*join_path_with_cmd(char *dir, char *cmd);

char	*find_in_path(char *cmd_str, char *path_env)
{
	char	**paths;

	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	return (find_correct_command_path(paths, cmd_str));
}

static char	*find_correct_command_path(char **paths, char *cmd_str)
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
