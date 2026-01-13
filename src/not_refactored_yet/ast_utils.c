/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 03:09:12 by lcosta-a          #+#    #+#             */
/*   Updated: 2025/12/30 21:03:46 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_parser.h" // was ast_utils.h"
#include "libft.h"

void	free_ast(t_node *ast)
{
	if (!ast)
		return;
	free_ast(ast->left);
	free_ast(ast->right);
	if (ast->cmd)
	{
		free_cmd_array(ast->cmd->args);
		if (ast->cmd->redirections)
			free(ast->cmd->redirections);
		free(ast->cmd);
	}
	free(ast);
}

void	free_cmd_array(char **cmd)
{
	int	i;

	i = 0;
	if (!cmd)
		return;
	while (cmd[i])
		free(cmd[i++]);
	free(cmd);
}
