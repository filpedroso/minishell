/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_tools.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 17:55:38 by fpedroso          #+#    #+#             */
/*   Updated: 2026/01/22 17:55:38 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_tok_redirection(t_token_type tok_type)
{
	return (tok_type == TOK_APPEND || tok_type == TOK_REDIR_IN
		|| tok_type == TOK_REDIR_OUT || tok_type == TOK_HEREDOC);
}

t_token_lst	*get_first_pipe(t_token_lst *start, t_token_lst *end)
{
	while (start && start != end)
	{
		if (start->type == TOK_PIPE)
			return (start);
		start = start->next;
	}
	if (end->type == TOK_PIPE)
		return (end);
	return (NULL);
}

t_token_lst	*tok_lstlast(t_token_lst *lst)
{
	if (!lst)
		return (NULL);
	while(lst->next)
		lst = lst->next;
	return (lst);
}

void destroy_ast(t_ast_node *node)
{
    if (!node)
        return;
    if (node->type == NODE_PIPE)
    {
        destroy_ast(node->left);
        destroy_ast(node->right);
        free(node);
    }
    else
        destroy_cmd_node(node);
}

void	destroy_cmd_node(t_ast_node *cmd_node)
{
	if (cmd_node)
	{
		if (cmd_node->cmd)
		{
			if (cmd_node->cmd->redirections)
				free(cmd_node->cmd->redirections);
			if (cmd_node->cmd->words)
				free(cmd_node->cmd->words);
			free(cmd_node->cmd);
		}
		free(cmd_node);
	}
}
