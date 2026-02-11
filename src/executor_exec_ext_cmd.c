/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_exec_ext_cmd.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 20:15:43 by fpedroso          #+#    #+#             */
/*   Updated: 2026/02/10 00:34:49 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_argv_and_exec_ext_cmd(t_ast_node *node);
void	debug_print_args(char *path, char **argv, char **current_envs);
void	debug_puts_many(char **arr);

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
}

static int	get_argv_and_exec_ext_cmd(t_ast_node *node)
{
	char	*path;
	char	**current_envs;
	char	**final_argv;

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
	final_argv = produce_final_argv(node->cmd, current_envs);
	debug_print_args(path, final_argv, current_envs);
	// if (execve(path, produce_final_argv(node->cmd, current_envs), current_envs) == -1)
	// {
	// 	perror("Execve");
	// 	destroy_cmd_node(node);
	// 	free_str_arr(current_envs);
	// 	return(1);
	// }
	return (0);
}

void	debug_print_args(char *path, char **argv, char **current_envs)
{
	puts("ARGS DEBUG >>>>>");

	puts("PATH:");
	puts(path);

	puts("ARGV:");
	debug_puts_many(argv);

	puts("ENVP:");
	debug_puts_many(current_envs);

	puts("<<<<< END ARGS DEBUG");
}

void	debug_puts_many(char **arr)
{
	if (!arr)
		return ;
	for (int i = 0; arr[i]; i++)
	{
		puts(arr[i]);
	}
}
