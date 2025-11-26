/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 16:51:07 by fpedroso          #+#    #+#             */
/*   Updated: 2025/11/22 16:51:07 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
/* 
**	agora o que falta é:
**	entender a questão dos exits() e dos waits() na arvore de processos, como evitar zombie processes
**	entender se o pipe precisa ser fechado e como fazer isso
**	handle redirections(): logica para configurar as redirs a partir das infos que temos no node
**	o que é envp?
 */
static void	pipe_logic(t_node *node, char **envp);
static void	exec_left(int pip[2], t_node *node, char **envp);
static void	exec_right(int pip[2], t_node *node, char **envp);

void    execute_tree(t_node *node, char **envp)
{
	pid_t	exec_pid;

    if (!node)
        return;
    if (node->type == PIPE)
		pipe_logic(node, envp);
	else
	{
		handle_redirections(node->redirections);
		if (node->type == EXT_CMD)
			exec_cmd(node, envp);
		else if (node->is_pipeline && node->type == BUILTIN)
			exec_forked_builtin(node, envp);
		else if (node->type == BUILTIN)
			exec_builtin(node, envp);
	}
}

static void	pipe_logic(t_node *node, char **envp)
{
    pid_t	left_pid;
    pid_t	right_pid;
    int		pip[2];

	if (pipe(pip) != 0)
	{
		perror("Pipe");
		exit(1);
	}
	left_pid = fork();
	if (left_pid == CHILD)
		exec_left(pip, node, envp);
	close(pip[WRITE]);
	right_pid = fork();
	if (right_pid == CHILD)
		exec_right(pip, node, envp);
	close(pip[READ]);
	wait(NULL);
	wait(NULL);
}

static void	exec_left(int pip[2], t_node *node, char **envp)
{
	close(pip[READ]);
	dup2(pip[WRITE], STDOUT_FILENO);
	close(pip[WRITE]);
	execute_tree(node->left, envp);
	exit(0);
}

static void	exec_right(int pip[2], t_node *node, char **envp)
{
	dup2(pip[READ], STDIN_FILENO);
	close(pip[READ]);
	execute_tree(node->right, envp);
	exit(0);
}

void	exec_cmd(t_node *node, char **envp)
{
	pid_t	pid;

	pid = fork();
	if (pid == CHILD)
		execve(get_path(node->cmds), node->argv, envp);
}
