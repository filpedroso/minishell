/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 22:17:15 by fpedroso          #+#    #+#             */
/*   Updated: 2026/01/20 22:17:15 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// um problema neste design:
// tem que ter um loop pra evitar um early return,
// onde se emite um node de comando e pronto
// pensar melhor
t_ast_node	*build_expanded_ast(t_token_lst *tok_lst, t_env_vars env_vars)
{
	t_ast_node	*ast;

	if (!tok_lst)
		return (NULL);
	if (tok_lst->type == TOK_PIPE)
	{
		ast = create_pipe_ast_node();
		if (!ast)
			return (NULL);
		ast->left = build_expanded_ast(tok_lst->previous, env_vars);
		ast->right = build_expanded_ast(tok_lst->next, env_vars);
	}
	else
	{
		ast = create_expanded_ast_cmd_node(tok_lst);
		if (!ast)
			return (NULL);
	}
	return (ast);
}

/* 
atravessa a lista.
se for pipe, vira raiz, se não, vira esquerda ou direita
a árvore roda da forma que o executor roda... como converter os tokens
em nodes numa ast?
- tokens vão ser divididos em nodes diferentes?
- um node consiste em comando + redireções + args, correto?
- o token já guarda essa estrutura?
*/