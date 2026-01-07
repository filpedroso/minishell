/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcosta-a <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 03:09:12 by lcosta-a          #+#    #+#             */
/*   Updated: 2025/12/23 18:36:38 by lcosta-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast_utils.h"
#include "libft/libft.h"

void	free_ast(t_node *ast)
{
	if (!ast)
		return;
	free_ast(ast->left);
	free_ast(ast->right);
	free_cmd_array(ast->cmds);
	ft_lstclear(&ast->temp_files, free);
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
