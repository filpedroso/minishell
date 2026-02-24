/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_command_logic.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 20:06:36 by fpedroso          #+#    #+#             */
/*   Updated: 2026/02/10 00:34:49 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	command_logic(t_sh *sh, t_ast_node *node)
{
	int	exit_status;

	exit_status = 0;
	if (handle_redirections(node->cmd) < 0)
	{
		destroy_cmd_node(node);
		return (1);
	}
	if (node->cmd->type == EXT)
		exit_status = exec_ext_cmd(sh, node);
	else
		exit_status = builtin_logic(sh, node);
	return (exit_status);
}
