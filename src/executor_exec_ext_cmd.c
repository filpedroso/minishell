/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_exec_ext_cmd.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 20:15:43 by fpedroso          #+#    #+#             */
/*   Updated: 2026/02/28 21:11:45 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	get_argv_and_exec_ext_cmd(t_sh *sh, t_ast_node *node);
void	debug_print_args(char *path, char **argv);
void	debug_puts_many(char **arr);

int	exec_ext_cmd(t_sh *sh, t_ast_node *node)
{
	pid_t	pid;
	int		exit_status;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (1);
	}
	if (pid == CHILD)
	{
		set_signals_default();
		get_argv_and_exec_ext_cmd(sh, node);
		destroy_cmd_node(node);
		exit(1);
	}
	set_signals_child();
	waitpid(pid, &exit_status, 0);
	set_signals_interactive();
	if (WIFEXITED(exit_status))
		return (WEXITSTATUS(exit_status));
	if (WIFSIGNALED(exit_status))
		return (128 + WTERMSIG(exit_status));
	return (1);
}

static void	get_argv_and_exec_ext_cmd(t_sh *sh, t_ast_node *node)
{
	t_exec_args	ex;

	ex.path = get_cmd_path(node->cmd);
	if (!ex.path)
		return (perror("Command path not found"));
	ex.envp = get_current_envs(node->cmd->env_vars);
	if (!ex.envp)
	{
		free(ex.path);
		return (perror("Get current envs"));
	}
	ex.argv = produce_final_argv(sh, node->cmd, ex.envp);
	if (!ex.argv)
	{
		destroy_exec_args(&ex);
		return (perror("Produce final argv"));
	}
	execve(ex.path, ex.argv, ex.envp);
	destroy_exec_args(&ex);
	perror("Execve");
}

void	destroy_exec_args(t_exec_args *ex)
{
	if (ex->path)
		free(ex->path);
	if (ex->envp)	
		free_str_arr(ex->envp);
	if (ex->argv)
		free_str_arr(ex->argv);
	ex->path = NULL;
	ex->envp = NULL;
	ex->argv = NULL;
}

void	debug_print_args(char *path, char **argv)
{
	puts("PATH:");
	puts(path);

	puts("ARGV:");
	debug_puts_many(argv);

}

void	debug_puts_many(char **arr)
{
	if (!arr)
		return ;
	for (int i = 0; arr[i]; i++)
	{
		fputs(arr[i], stdout);
		fputs(" ", stdout);
	}
	puts("");
}
