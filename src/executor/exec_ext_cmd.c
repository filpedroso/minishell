/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_ext_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 20:15:43 by fpedroso          #+#    #+#             */
/*   Updated: 2025/12/16 20:15:43 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static int	get_args_and_execute(t_node *node);

void	exec_ext_cmd(t_node *node)
{
	pid_t	pid;
	int		exit_code;

	pid = fork();
	if (pid == CHILD)
	{
		exit_code = get_args_and_execute(node);
		exit(exit_code);
	}
	wait(NULL);
	free_stuff(node);
}

static int	get_args_and_execute(t_node *node)
{
	char	*path;
	char	**current_envs;

	path = get_cmd_path(node->cmd);
	if (!path)
	{
		free_stuff(node);
		perror("Get path");
		return (1);
	}
	current_envs = get_current_envs(node->cmd.env_vars);
	if (!current_envs)
	{
		free_stuff(node);
		perror("Get current envs");
		return (1);
	}
	if (execve(path, node->cmd.args, current_envs) == -1)
	{
		perror("Execve");
		free_str_arr(current_envs);
		return(1);
	}
}
