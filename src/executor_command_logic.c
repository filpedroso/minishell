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

void	command_logic(t_ast_node *node)
{
	if (handle_redirections(node->cmd) < 0)
	{
		destroy_cmd_node(node);
		return ;
	}
	if (node->cmd->type == EXT)
		exec_ext_cmd(node);
	else
		puts("TODO: BUILTINS");
	// else if (node->cmd->type == BUILTIN)
	// {
	// 	if (node->cmd->is_pipeline)
	// 		exec_forked_builtin(node);
	// 	else
	// 		exec_builtin(node);
	// }
}
