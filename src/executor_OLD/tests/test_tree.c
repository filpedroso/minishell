/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_tree.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 00:00:00 by fpedroso          #+#    #+#             */
/*   Updated: 2026/02/10 00:35:26 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

t_node			*create_test_tree_single(char *arg);
t_node			*create_test_tree_double(char *arg_1, char *arg_2);
t_node			*create_test_tree(void);

static t_node	*create_pipe_node(t_node *left, t_node *right);
static t_node	*create_cmd_node(t_node_type type, char **cmds);

int	main(void)
{
	t_node	*single_cmd;

	single_cmd = create_test_tree_single("pwd");
	execute_tree(single_cmd);
}
t_node	*create_test_tree_single(char *arg)
{
	char	**pwd_args;

	pwd_args = (char **)malloc(sizeof(char *) * 2);
	pwd_args[0] = arg;
	pwd_args[1] = NULL;
	return (create_cmd_node(BUILTIN, pwd_args));
}

{
	t_node *node_echo;
	t_node *node_pwd;
	char **echo_args;
	char **pwd_args;

	echo_args = (char **)malloc(sizeof(char *) * 2);
	echo_args[0] = arg_1;
	echo_args[1] = NULL;

	pwd_args = (char **)malloc(sizeof(char *) * 2);
	pwd_args[0] = "pwd";
	pwd_args[1] = NULL;

	node_echo = create_cmd_node(BUILTIN, echo_args);
	node_pwd = create_cmd_node(BUILTIN, pwd_args);

	node_echo->is_pipeline = true;
	node_pwd->is_pipeline = true;

	return (create_pipe_node(node_echo, node_pwd));
}

t_node	*create_test_tree(void)
{
	t_node	*node_echo;
	t_node	*node_pwd;
	t_node	*node_env;
	t_node	*pipe1;
	t_node	*pipe2;
	char	**echo_args;
	char	**pwd_args;
	char	**env_args;

	// Allocate command arguments
	echo_args = (char **)malloc(sizeof(char *) * 2);
	echo_args[0] = "echo";
	echo_args[1] = NULL;
	pwd_args = (char **)malloc(sizeof(char *) * 2);
	pwd_args[0] = "pwd";
	pwd_args[1] = NULL;
	env_args = (char **)malloc(sizeof(char *) * 2);
	env_args[0] = "env";
	env_args[1] = NULL;
	// Create command nodes
	node_echo = create_cmd_node(BUILTIN, echo_args);
	node_pwd = create_cmd_node(BUILTIN, pwd_args);
	node_env = create_cmd_node(BUILTIN, env_args);
	// Mark nodes as part of pipeline
	node_echo->is_pipeline = true;
	node_pwd->is_pipeline = true;
	node_env->is_pipeline = true;
	// Build pipe structure: (echo | pwd) | env
	pipe1 = create_pipe_node(node_echo, node_pwd);
	pipe2 = create_pipe_node(pipe1, node_env);
	return (pipe2);
}

static t_node	*create_cmd_node(t_node_type type, char **cmds)
{
	t_node	*node;

	node = (t_node *)malloc(sizeof(t_node));
	if (!node)
		return (NULL);
	node->type = type;
	node->left = NULL;
	node->right = NULL;
	node->cmds = cmds;
	node->env_list = NULL;
	node->inline_env_list = NULL;
	node->is_pipeline = false;
	node->redirections = 0;
	return (node);
}

static t_node	*create_pipe_node(t_node *left, t_node *right)
{
	t_node	*node;

	node = (t_node *)malloc(sizeof(t_node));
	if (!node)
		return (NULL);
	node->type = PIPE;
	node->left = left;
	node->right = right;
	node->cmds = NULL;
	node->env_list = NULL;
	node->inline_env_list = NULL;
	node->is_pipeline = true;
	node->redirections = 0;
	return (node);
}

/*
 * Creates a hardcoded syntax tree for testing
 *
 * Tree structure: echo | pwd | env
 *
 * This represents a pipeline of three commands:
 * - echo (builtin)
 * - pwd (builtin)
 * - env (builtin)
 */
