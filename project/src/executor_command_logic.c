/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_command_logic.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 20:06:36 by fpedroso          #+#    #+#             */
/*   Updated: 2026/03/15 14:26:10 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	save_std_in_out_if_parent(t_sh *sh, int *stdin_bkp, int *stdout_bkp);
static void	restore_std_in_out_if_parent(t_sh *sh, int stdin_bkp, int stdout_backup);
static int	redir_fail(int stdin_bkp, int stdout_bkp, t_ast_node *node);

int	command_logic(t_sh *sh, t_ast_node *node)
{
	int	exit_status;
	int	stdin_bkp;
	int	stdout_bkp;

	if (!node || !node->cmd)
		return (0);
	save_std_in_out_if_parent(sh, &stdin_bkp, &stdout_bkp);
	if (handle_redirections(node->cmd) < 0)
		return (redir_fail(stdin_bkp, stdout_bkp, node));
	if (!node->cmd->words || node->cmd->words_count == 0)
	{
		restore_std_in_out_if_parent(sh, stdin_bkp, stdout_bkp);
		return (0);
	}
	if (node->cmd->type == EXT)
		exit_status = exec_ext_cmd(sh, node, stdin_bkp, stdout_bkp);
	else
		exit_status = exec_builtin(sh, node);
	restore_std_in_out_if_parent(sh, stdin_bkp, stdout_bkp);
	return (exit_status);
}

static int	redir_fail(t_sh *sh, int stdin_bkp, int stdout_bkp)
{
	restore_std_in_out_if_parent(sh, stdin_bkp, stdout_bkp);
	return (1);
}

static void	save_std_in_out_if_parent(t_sh *sh, int *stdin_bkp, int *stdout_bkp)
{
	if (!sh->in_child)
	{
		*stdin_bkp = dup(STDIN_FILENO);
		*stdout_bkp = dup(STDOUT_FILENO);
	}
	else
	{
		*stdin_bkp = -1;
		*stdout_bkp = -1;
	}
}

static void	restore_std_in_out_if_parent(t_sh *sh, int stdin_bkp, int stdout_backup)
{
	if (!sh->in_child)
	{
		dup2(stdin_bkp, STDIN_FILENO);
		dup2(stdout_backup, STDOUT_FILENO);
		close(stdin_bkp);
		close(stdout_backup);
	}
}
