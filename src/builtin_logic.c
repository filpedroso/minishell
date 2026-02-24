/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_logic.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 20:17:35 by fpedroso          #+#    #+#             */
/*   Updated: 2026/02/23 21:31:37 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	exec_forked_builtin(t_sh *sh, t_ast_node *node);
static int	get_argv_and_exec_builtin(t_sh *sh, t_ast_node *node);
static int	builtin_dispatcher(t_sh *sh, t_cmd *cmd, t_exec_args *ex);

int	builtin_logic(t_sh *sh, t_ast_node *node)
{
	if (node->cmd->is_pipeline)
		return (exec_forked_builtin(sh, node));
	else
		return (get_argv_and_exec_builtin(sh, node));
}

static int	exec_forked_builtin(t_sh *sh, t_ast_node *node)
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
		exit_status = get_argv_and_exec_builtin(sh, node);
		destroy_cmd_node(node);
		exit(exit_status);
	}
	waitpid(pid, &exit_status, 0);
	if (WIFEXITED(exit_status))
		return (WEXITSTATUS(exit_status));
	if (WIFSIGNALED(exit_status))
		return (128 + WTERMSIG(exit_status));
	return (1);
}

static int	get_argv_and_exec_builtin(t_sh *sh, t_ast_node *node)
{
	int				exit_status;
	t_exec_args		ex;

	ex.path = NULL;
	ex.envp = get_current_envs(node->cmd->env_vars);
	if (!ex.envp)
		return (perror("Get current envs"), 1);
	ex.argv = produce_final_argv(sh, node->cmd, ex.envp);
	if (!ex.argv)
	{
		destroy_exec_args(&ex);
		return (perror("Produce final argv"), 1);
	}
	exit_status = builtin_dispatcher(sh, node->cmd, ex);
	destroy_exec_args(&ex);
	return (exit_status);
}

static int	builtin_dispatcher(t_sh *sh, t_cmd *cmd, t_exec_args *ex)
{
	char	*cmd_name;

	cmd_name = cmd->words[0].token_word_ptr;
	if (ft_strncmp(cmd_name, "echo", 4) == 0)
		return (ft_echo());
	else if (ft_strncmp(cmd_name, "cd", 2) == 0)
		return (ft_cd());
	else if (ft_strncmp(cmd_name, "pwd", 3) == 0)
		return (ft_pwd());
	else if (ft_strncmp(cmd_name, "export", 6) == 0)
		return (ft_export());
	else if (ft_strncmp(cmd_name, "unset", 5) == 0)
		return (ft_unset());
	else if (ft_strncmp(cmd_name, "env", 3) == 0)
		return (ft_env());
	else
		return (1);
}
