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

/* **************************************************** */
/*                                                      */
//	THINK ABOUT THIS SUGGESTION:                        */
//		Only fatal errors abort parsing immediately     */
//		Syntax errors propagate normally                */
/*                                                      */
/* **************************************************** */

static t_ast_node 	*parse_tokens_into_ast(
	t_token_lst *start,
	t_token_lst *end,
	t_parse_status *status
	);
static t_ast_node	*new_command_node(
	t_token_lst *start,
	t_token_lst *end,
	t_parse_status *status
	);
static t_ast_node	*create_pipe_ast_node(t_parse_status *status);

t_ast	make_ast(t_token_lst *tok_lst)
{
	t_ast			ast;

	if (!tok_lst || !check_syntax(tok_lst))
	{
		ft_putstr_fd("Parse error\n", 2);
		ast.parse_status = PARSE_ERROR;
		return (ast);
	}
	ast.parse_status = PARSE_OK;
	ast.ast_root = parse_tokens_into_ast(tok_lst, tok_lstlast(tok_lst), &ast.parse_status);
	if (ast.parse_status == PARSE_ERROR)
		cleanup_ast(ast.ast_root);
	return (ast);
}

static t_ast_node *parse_tokens_into_ast(t_token_lst *start, t_token_lst *end, t_parse_status *status)
{
    t_token_lst *pipe;
    t_ast_node  *ast_node;

    if (!start || !end)
    {
        *status = PARSE_FATAL;
        return (NULL);
    }
    pipe = get_first_pipe(start, end);
    if (pipe)
    {
        ast_node = create_pipe_ast_node(status);
        if (*status != PARSE_OK)
            return (NULL);
        ast_node->left = parse_tokens_into_ast(start, pipe->previous, status);
        if (*status != PARSE_OK)
            return (ast_node);
        ast_node->right = parse_tokens_into_ast(pipe->next, end, status);
        return (ast_node);
    }
    return (new_command_node(start, end, status));
}

static t_ast_node	*create_pipe_ast_node(t_parse_status *status)
{
	t_ast_node	*new_pipe_node;

	new_pipe_node = malloc(sizeof(t_ast_node));
	if (!new_pipe_node)
	{
		*status = PARSE_FATAL;
		return (NULL);
	}
	new_pipe_node->type = NODE_PIPE;
	new_pipe_node->cmd = NULL;
	new_pipe_node->left = NULL;
	new_pipe_node->right = NULL;
	return (new_pipe_node);
}

static t_ast_node	*new_command_node(t_token_lst *start, t_token_lst *end, t_parse_status *status)
{
	t_token_lst	*current;
	t_ast_node	*new_cmd_node;

	new_cmd_node = alloc_cmd_node(start, end); // allocates the node, its cmd, redirs and everything it will need
	if (!new_cmd_node)
	{
		*status = STATUS_ERR;
		return (NULL);
	}
	current = start;
	while (current && current != end->next)
	{
		if (is_tok_redirection(current->type))
			current = parse_redirection(new_cmd_node, current, status);
		else
			current = parse_word(new_cmd_node, current, status);
		if (*status != STATUS_OK)
		{
			cleanup_ast(new_cmd_node);
			return (NULL);
		}
	}
	return (new_cmd_node);
}

t_token_lst	*parse_redirection(t_ast_node *cmd_ast_node, t_token_lst *token_node, t_parse_status *status)
{
    if (!token_node->next || token_node->next->type != TOK_WORD)
    {
        *status = STATUS_ERR;
        return NULL;
    }
    add_redir(cmd_ast_node, token_node, token_node->next, status);
    return (token_node->next->next);
}

t_token_lst *parse_word(t_ast_node *cmd_ast_node, t_token_lst *token_node, t_parse_status *status)
{
    add_word(cmd_ast_node, token_node, status);
    return (token_node->next);
}
