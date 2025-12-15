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

static void	pipe_logic(t_node *node);
static void	exec_left(int pip[2], t_node *node);
static void	exec_right(int pip[2], t_node *node);
static void init_builtin_table(t_builtin table[N_BUILTINS]);
static void	exec_cmd(t_node *node);
static int	exec_forked_builtin(t_node *node);
static int	exec_builtin(t_node *node);

void    execute_tree(t_node *node)
{
    if (!node)
        return;
    if (node->type == PIPE)
		pipe_logic(node);
	else
	{
		handle_redirections(node->cmd);
		if (node->cmd.type == EXT)
			exec_cmd(node);
		else if (node->cmd.is_pipeline && node->cmd.type == BUILTIN)
			exec_forked_builtin(node);
		else if (node->cmd.type == BUILTIN)
			exec_builtin(node);
	}
	cleanup_node(node); 
}

void	cleanup_node(t_node *node)
{
	clean_temp_files(); // checks if there are created files to delete
	clean_allocations();
}

static int	exec_forked_builtin(t_node *node)
{
	pid_t	pid;

	pid = fork();
	if (pid == CHILD)
		exec_builtin(node);
	wait(NULL);
	return (1);
}

static int	exec_builtin(t_node *node)
{
	t_builtin	builtin_table[N_BUILTINS];
	int			i;

	init_builtin_table(builtin_table);
	i = 0;
    while (i < N_BUILTINS)
    {
        if (ft_strcmp(node->cmds[0], builtin_table[i].name) == 0)
			return (builtin_table[i].func(node->cmds, node->envs));
        i++;
    }
    return -1;
}

void    init_builtin_table(t_builtin table[N_BUILTINS])
{
    table[0].name = "echo";
    table[0].func = &ft_echo;

    table[1].name = "cd";
    table[1].func = &ft_cd;

    table[2].name = "pwd";
    table[2].func = &ft_pwd;

    table[3].name = "export";
    table[3].func = &ft_export;

    table[4].name = "unset";
    table[4].func = &ft_unset;

    table[5].name = "env";
    table[5].func = &ft_env;

    table[6].name = "exit";
    table[6].func = &ft_exit;
}

static void	pipe_logic(t_node *node)
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
		exec_left(pip, node);
	close(pip[WRITE]);
	right_pid = fork();
	if (right_pid == CHILD)
		exec_right(pip, node);
	close(pip[READ]);
	wait(NULL);
	wait(NULL);
}

static void	exec_left(int pip[2], t_node *node)
{
	close(pip[READ]);
	dup2(pip[WRITE], STDOUT_FILENO);
	close(pip[WRITE]);
	execute_tree(node->left);
	exit(0);
}

static void	exec_right(int pip[2], t_node *node)
{
	dup2(pip[READ], STDIN_FILENO);
	close(pip[READ]);
	execute_tree(node->right);
	exit(0);
}

static void	exec_cmd(t_node *node)
{
	pid_t	pid;
	char	*path;

	pid = fork();
	if (pid == CHILD)
	{
		path = get_path(node->cmd);
		if (!path || execve(path, node->cmds, node->envs) == -1)
		{
			free_stuff(node);
			perror("Execve");
			exit(1);
		}
	}
	wait(NULL);
	// free_stuff(node);
}
