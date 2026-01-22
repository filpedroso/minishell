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
t_ast_node	*build_ast(t_token_lst *tok_lst)
{
	t_token_lst	*last_tok;
	t_status	status;
	t_ast_node	*ast;

	if (!tok_lst)
		return (NULL);
	last_tok = tok_lstlast(tok_lst);
	if (tok_lst->type == TOK_PIPE || last_tok->type == TOK_PIPE)
	{
		ft_putstr_fd("Parse error near '|'\n", 2);
		return (NULL);
	}
	status = STATUS_OK;
	ast = parse_tokens_into_ast(tok_lst, last_tok, &status);
	if (status != STATUS_OK)
	{
		cleanup_ast(ast);
		return (NULL);
	}
	return (ast);
}

t_ast_node *parse_tokens_into_ast(t_token_lst *start, t_token_lst *end, t_status *status)
{
    t_token_lst *pipe;
    t_ast_node  *ast_node;

    if (!start || !end)
    {
        *status = STATUS_ERR;
        return (NULL);
    }
    pipe = get_first_pipe(start, end);
    if (pipe)
    {
        ast_node = create_pipe_ast_node(status);
        if (*status != STATUS_OK)
            return (NULL);
        ast_node->left = parse_tokens_into_ast(start, pipe->previous, status);
        if (*status != STATUS_OK)
            return (ast_node);
        ast_node->right = parse_tokens_into_ast(pipe->next, end, status);
        return (ast_node);
    }
    return (new_command_node(start, end, status));
}


t_ast_node	*parse_tokens_into_ast(t_token_lst *start, t_token_lst *end, t_status *status)
{
	t_token_lst	*pipe;
	t_ast_node	*ast_node;

	if (!start || !end)
		return (NULL);
	pipe = get_first_pipe(start, end);
	if (pipe)
	{
		ast_node = create_pipe_ast_node();
		if (!ast_node)
		{
			*status = STATUS_ERR;
			return (NULL);
		}
		ast_node->left = parse_tokens_into_ast(start, pipe->previous, status);
		ast_node->right = parse_tokens_into_ast(pipe->next, end, status);
		return (ast_node);
	}
	else
	{
		ast_node = new_command_node(start, end);
		if (!ast_node)
			*status = STATUS_ERR;
		return (ast_node);
	}
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