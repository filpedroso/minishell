/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 16:51:07 by fpedroso          #+#    #+#             */
/*   Updated: 2025/12/10 18:41:11 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static void	recursive_pipe_logic(t_node *node);
static void	exec_piped_left_node(int pip[2], t_node *node);
static void	exec_piped_right_node(int pip[2], t_node *node);

void	execute_tree(t_node *node)
{
    if (!node)
        return;
    if (node->type == PIPE)
		recursive_pipe_logic(node);
	else
		command_logic(node);
	cleanup_node(node); 
}

static void	recursive_pipe_logic(t_node *node)
{
    int		pip[2];

	if (pipe(pip) != 0)
	{
		perror("Pipe");
		exit(1);
	}
	node->left->cmd.is_pipeline = true;
	node->right->cmd.is_pipeline = true;
	exec_piped_left_node(pip, node->left);
	exec_piped_right_node(pip, node->right);
}

static void	exec_piped_left_node(int pip[2], t_node *node)
{
    pid_t	left_pid;

	left_pid = fork();
	if (left_pid == CHILD)
	{
		close(pip[READ]);
		dup2(pip[WRITE], STDOUT_FILENO);
		close(pip[WRITE]);
		execute_tree(node);
		exit(0);
	}
	wait(NULL);
	close(pip[WRITE]);
}

static void	exec_piped_right_node(int pip[2], t_node *node)
{
    pid_t	right_pid;

	right_pid = fork();
	if (right_pid == CHILD)
	{
		dup2(pip[READ], STDIN_FILENO);
		close(pip[READ]);
		execute_tree(node->right);
		exit(0);
	}
	wait(NULL);
	close(pip[READ]);
}
