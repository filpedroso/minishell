/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 16:51:07 by fpedroso          #+#    #+#             */
/*   Updated: 2026/03/15 13:45:23 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int		recursive_pipe_logic(t_sh *sh, t_ast_node *node);
static pid_t	exec_piped_left_node(t_sh *sh, int pip[2], t_ast_node *node);
static pid_t	exec_piped_right_node(t_sh *sh, int pip[2], t_ast_node *node);
static int		wait_and_return_exit_st(pid_t left_pid, pid_t right_pid);

void	execute_tree(t_sh *sh, t_ast_node *node)
{
	int	exit_status;

	if (!node)
		return ;
	exit_status = 0;
	if (node->type == NODE_PIPE)
		exit_status = recursive_pipe_logic(sh, node);
	else if (is_exit_builtin(node))
		exit_status = exec_builtin(sh, node);
	else
		exit_status = command_logic(sh, node);
	sh->last_exit_st = exit_status;
}

static int	recursive_pipe_logic(t_sh *sh, t_ast_node *node)
{
	pid_t	left_pid;
	pid_t	right_pid;
	int		pip[2];

	if (!node || !node->left || !node->right)
		return (0);
	if (pipe(pip) != 0)
		return (perror("Pipe"), 1);
	if (node->left->cmd)
		node->left->cmd->is_pipeline = true;
	if (node->right->cmd)
		node->right->cmd->is_pipeline = true;
	left_pid = exec_piped_left_node(sh, pip, node->left);
	close(pip[WRITE]);
	right_pid = exec_piped_right_node(sh, pip, node->right);
	close(pip[READ]);
	set_signals_child();
	return (wait_and_return_exit_st(left_pid, right_pid));
}

static int	wait_and_return_exit_st(pid_t left_pid, pid_t right_pid)
{
	int	left_status;
	int	right_status;

	waitpid(left_pid, &left_status, 0);
	waitpid(right_pid, &right_status, 0);
	set_signals_interactive();
	if (WIFEXITED(right_status))
		return (WEXITSTATUS(right_status));
	if (WIFSIGNALED(right_status))
	{
		write(STDOUT_FILENO, "\n", 1);
		return (128 + WTERMSIG(right_status));
	}
	return (1);
}

static pid_t	exec_piped_left_node(t_sh *sh, int pip[2], t_ast_node *node)
{
	pid_t	left_pid;

	left_pid = fork();
	if (left_pid == CHILD)
	{
		sh->in_child = true;
		set_signals_default();
		close(pip[READ]);
		dup2(pip[WRITE], STDOUT_FILENO);
		close(pip[WRITE]);
		execute_tree(sh, node);
		child_cleanup(sh);
		exit(sh->last_exit_st);
	}
	return (left_pid);
}

static pid_t	exec_piped_right_node(t_sh *sh, int pip[2], t_ast_node *node)
{
	pid_t	right_pid;

	right_pid = fork();
	if (right_pid == CHILD)
	{
		sh->in_child = true;
		set_signals_default();
		dup2(pip[READ], STDIN_FILENO);
		close(pip[READ]);
		execute_tree(sh, node);
		child_cleanup(sh);
		exit(sh->last_exit_st);
	}
	return (right_pid);
}
