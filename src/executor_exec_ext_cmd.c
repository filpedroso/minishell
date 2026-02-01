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

#include "minishell.h"

static int	get_argv_and_exec_ext_cmd(t_ast_node *node);

void	exec_ext_cmd(t_ast_node *node)
{
	pid_t	pid;
	int		exit_code;

	pid = fork();
	if (pid == CHILD)
	{
		exit_code = get_argv_and_exec_ext_cmd(node);
		exit(exit_code);
	}
	wait(NULL);
	free_stuff(node);
}

static int	get_argv_and_exec_ext_cmd(t_ast_node *node)
{
	char	*path;
	char	**current_envs;

	path = get_cmd_path(node->cmd);
	if (!path)
	{
		destroy_cmd_node(node);
		perror("Command path not found");
		return (1);
	}
	current_envs = get_current_envs(node->cmd->env_vars);
	if (!current_envs)
	{
		destroy_cmd_node(node);
		perror("Get current envs");
		return (1);
	}
	if (execve(path, produce_final_argv(node->cmd), current_envs) == -1)
	{
		perror("Execve");
		destroy_cmd_node(node);
		free_str_arr(current_envs);
		return(1);
	}
}

char	**produce_final_argv(t_command *cmd)
{
	// TODO ;
}
