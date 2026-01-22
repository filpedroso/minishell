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

static t_ast_node 	*parse_tokens_into_ast(
	t_token_lst *start,
	t_token_lst *end,
	t_status *status
	);
static t_ast_node	*new_command_node(
	t_token_lst *start,
	t_token_lst *end,
	t_status *status
	);
static t_ast_node	*create_pipe_ast_node(t_status *status);

t_ast_node	*build_ast(t_token_lst *tok_lst)
{
	t_token_lst	*last_tok;
	t_status	status;
	t_ast_node	*ast;

	if (!tok_lst)
		return (NULL);
	last_tok = tok_lstlast(tok_lst);
	if (check_syntax(tok_lst, last_tok) == false)
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

static t_ast_node *parse_tokens_into_ast(t_token_lst *start, t_token_lst *end, t_status *status)
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

static t_ast_node	*create_pipe_ast_node(t_status *status)
{
	t_ast_node	*new_pipe_node;

	new_pipe_node = malloc(sizeof(t_ast_node));
	if (!new_pipe_node)
	{
		*status = STATUS_ERR;
		return (NULL);
	}
	new_pipe_node->type = NODE_PIPE;
	new_pipe_node->cmd = NULL;
	new_pipe_node->left = NULL;
	new_pipe_node->right = NULL;
	return (new_pipe_node);
}

static t_ast_node	*new_command_node(t_token_lst *start, t_token_lst *end, t_status *status)
{
	/* 
	tudo entre pipes é um comando só ;
	varios tokens podem fazer parte de um comando ;
	programa ;
	redireções ;
	path ;
	variáveis... ;
	é o argv do babado ;
	é focar em construir esse argv do jeito certo;
	*/
}
