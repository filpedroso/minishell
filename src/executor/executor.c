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
static void init_builtin_table(t_builtin table[N_BUILTINS]);
static void	exec_ext_cmd(t_node *node);
static int	exec_forked_builtin(t_node *node);
static int	exec_builtin(t_node *node);

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

void	command_logic(t_node *node)
{
	handle_redirections(node->cmd);
	if (node->cmd.type == EXT)
		exec_ext_cmd(node);
	else if (node->cmd.type == BUILTIN)
	{
		if (node->cmd.is_pipeline)
			exec_forked_builtin(node);
		else
			exec_builtin(node);
	}
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

static void	exec_piped_right_node_node(int pip[2], t_node *node)
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

static void	exec_ext_cmd(t_node *node)
{
	pid_t	pid;
	int		exit_code;

	pid = fork();
	if (pid == CHILD)
	{
		exit_code = get_args_and_execute(node);
		exit(exit_code);
	}
	wait(NULL);
	free_stuff(node);
}

int	get_args_and_execute(t_node *node)
{
	char	*path;
	char	**current_envs;

	path = get_cmd_path(node->cmd);
	if (!path)
	{
		free_stuff(node);
		perror("Get path");
		return (1);
	}
	current_envs = get_current_envs(node->cmd.env_vars);
	if (!current_envs)
	{
		free_stuff(node);
		perror("Get current envs");
		return (1);
	}
	if (execve(path, node->cmd.args, current_envs) == -1)
	{
		perror("Execve");
		return(1);
	}
}

char	**get_current_envs(t_env_vars env_lists)
{
	char	**current_envs;

	current_envs = merge_env_lists(env_lists.inline_envs, env_lists.persistent_envs);
	if (!current_envs)
	{
		perror("get envs failed");
		return (NULL);
	}
	return (current_envs);
}

char	**merge_env_lists(t_var_list *inline_list, t_var_list *persistent_list)
{
	char	**vars_array;
	int		inline_amount;
	int		persistent_amount;

	inline_amount = ft_lstsize(inline_list);
	persistent_amount = ft_lstsize(persistent_list);
	vars_array = (char **)malloc(
		(inline_amount + persistent_amount + 1) * sizeof(char *));
	if (!vars_array)
		return (NULL);
	vars_array[inline_amount + persistent_amount] = NULL;
	if (!fill_arr_from_lists(vars_array, inline_list, persistent_list))
	{
		free_str_arr(vars_array);
		return (NULL);
	}
	return (vars_array);
}

int	fill_arr_from_lists(char **arr, t_var_list *lst_1, t_var_list *lst_2)
{
	int	i;

	i = 0;
	while (lst_1)
	{
		arr[i] = ft_strdup(lst_1->var_name);
		if (!arr[i])
			return (NULL);
		arr[i] = ft_strjoin(arr[i], "=");
		if (!arr[i])
			return (NULL);
		arr[i] = ft_strjoin(arr[i], lst_1->value);
		if (!arr[i])
			return (NULL);
		i++;
		lst_1 = lst_1->next;
	}
}
