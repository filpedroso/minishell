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

void    execute_tree(t_node *node, char **envp)
{
    pid_t	left_pid;
    int		fd[2];

    if (!node)
        return;
    if (node->type == PIPE)
    {
        pipe(fd);
        left_pid = fork();
        if (left_pid == 0)
        {
            close(fd[READ]);
            dup2(fd[WRITE], STDOUT_FILENO);
            close(fd[WRITE]);
            execute_tree(node->left, envp);
            exit(0);
        }
        close(fd[WRITE]);
        dup2(fd[READ], STDIN_FILENO);
        close(fd[READ]);
        execute_tree(node->right, envp);
    }
    else if (node->type == CMD)
		exec_cmd(node, envp);
}

void	exec_cmd(t_node *node, char **envp)
{
	handle_redirections(node->redirections);
	if (is_builtin(node->cmds[0]))
		execute_builtin(node->cmds, envp);
	else // fork() and wait() ?
		execve(get_path(node->cmds[0], envp), node->cmds, envp);
}

/* 
**	agora o que falta é:
**	entender a questão dos exits() e dos waits() na arvore de processos, como evitar zombie processes
**	entender se o pipe precisa ser fechado e como fazer isso
**	handle redirections(): logica para configurar as redirs a partir das infos que temos no node
**	o que é envp?
 */

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
		{
			exec_pid = fork();
			if (exec_pid == CHILD)
			{
				execve(get_path(node->cmds[0], envp), node->cmds, envp);

			}
		}

	}
    else if (node->type == EXT_CMD)
	{
		if (is_builtin(node->cmds[0]))
			execute_builtin(node->cmds, envp);
		else // fork() and wait() ?

	}
}

void	pipe_logic(t_node *node, char **envp)
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

void	exec_cmd(t_node *node, char **envp)
{
}

{
	close(pip[READ]);
	dup2(pip[WRITE], STDOUT_FILENO);
	close(pip[WRITE]);
	execute_tree(node->left, envp);
	wait();
	exit(0);
}

{
	close(pip[WRITE]);
	dup2(pip[READ], STDIN_FILENO);
	close(pip[READ]);
	execute_tree(node->right, envp);
	wait();
	exit(0);
}
