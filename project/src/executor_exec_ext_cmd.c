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

static int	get_argv_and_exec_ext_cmd(t_sh *sh, t_ast_node *node);
void		debug_print_args(char *path, char **argv);
void		debug_puts_many(char **arr);
static int	bad_cmd(t_cmd *cmd);

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
		exit_status = get_argv_and_exec_ext_cmd(sh, node);
		child_cleanup(sh);
		exit(exit_status);
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

static int	get_argv_and_exec_ext_cmd(t_sh *sh, t_ast_node *node)
{
	t_exec_args	ex;

	ex.envp = get_current_envs(node->cmd->env_vars);
	if (!ex.envp)
		return (perror("Get current envs"), 1);
	ex.argv = produce_final_argv(sh, node->cmd, ex.envp);
	if (!ex.argv)
	{
		destroy_exec_args(&ex);
		return (perror("Produce final argv"), 1);
	}
	ex.path = get_cmd_path(ex.argv[0], node->cmd);
	if (!ex.path)
	{
		destroy_exec_args(&ex);
		return (bad_cmd(node->cmd));
	}
	execve(ex.path, ex.argv, ex.envp);
	destroy_exec_args(&ex);
	perror("Execve");
	if (errno == EACCES)
		return (126);
	else
		return (127);
}

static int	bad_cmd(t_cmd *cmd)
{
	if (!cmd || !cmd->words || !cmd->words[0].token_word_ptr
		|| !cmd->words[0].token_word_ptr[0])
		return (0);
	return (perror("Command not found"), 127);
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
