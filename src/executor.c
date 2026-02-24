/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 16:51:07 by fpedroso          #+#    #+#             */
/*   Updated: 2026/02/10 00:34:49 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	recursive_pipe_logic(t_sh *sh, t_ast_node *node);
static pid_t	exec_piped_left_node(t_sh *sh, int pip[2], t_ast_node *node);
static pid_t	exec_piped_right_node(t_sh *sh, int pip[2], t_ast_node *node);

void	execute_tree(t_sh *sh, t_ast_node *node)
{
	int	exit_status;

	if (!node)
		return ;
	if (node->type == NODE_PIPE)
		exit_status = recursive_pipe_logic(sh, node);
	if (is_exit_builtin(node))
		ft_exit(sh, node);
	else
		exit_status = command_logic(sh, node);
	sh->last_exit_st = exit_status;
	destroy_cmd_node(node);
}

static int	recursive_pipe_logic(t_sh *sh, t_ast_node *node)
{
	pid_t	left_pid;
	pid_t	right_pid;
	int		left_status;
	int		right_status;
	int		pip[2];

	if (pipe(pip) != 0)
	{
		perror("Pipe");
		return (1);
	}
	node->left->cmd->is_pipeline = true;
	node->right->cmd->is_pipeline = true;
	left_pid = exec_piped_left_node(sh, pip, node->left);
	close(pip[WRITE]);
	right_pid = exec_piped_right_node(sh, pip, node->right);
	close(pip[READ]);
	waitpid(left_pid, &left_status, 0);
	waitpid(right_pid, &right_status, 0);
	if (WIFEXITED(right_status))
		return (WEXITSTATUS(right_status));
	if (WIFSIGNALED(right_status))
		return (128 + WTERMSIG(right_status));
	return (1);
}

static pid_t	exec_piped_left_node(t_sh *sh, int pip[2], t_ast_node *node)
{
	pid_t	left_pid;

	left_pid = fork();
	if (left_pid == CHILD)
	{
		close(pip[READ]);
		dup2(pip[WRITE], STDOUT_FILENO);
		close(pip[WRITE]);
		execute_tree(sh, node);
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
		dup2(pip[READ], STDIN_FILENO);
		close(pip[READ]);
		execute_tree(sh, node);
		exit(sh->last_exit_st);
	}
	return (right_pid);
}
