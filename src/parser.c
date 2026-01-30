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

static bool			check_pipe_syntax(t_token_lst *first, t_token_lst *last);
static t_ast_node	*parse_tokens_into_ast(t_token_lst *start, t_token_lst *end, t_parse_status *status);
static void			add_env_vars_refs_to_tree(t_ast_node *tree_node, t_env_vars env_vars);
static t_ast_node	*create_pipe_ast_node(t_parse_status *status);

t_ast	make_ast(t_token_lst *tok_lst, t_env_vars env_vars)
{
	t_ast		ast;
	t_token_lst	*last_tok;

	last_tok = tok_lstlast(tok_lst);
	if (!check_pipe_syntax(tok_lst, last_tok))
	{
		ft_putstr_fd("Parse error\n", 2);
		ast.parse_status = PARSE_ERROR;
		ast.ast_root = NULL;
		return (ast);
	}
	ast.parse_status = PARSE_OK;
	ast.ast_root = parse_tokens_into_ast(tok_lst, last_tok, &ast.parse_status);
	if (ast.parse_status != PARSE_OK || ast.ast_root == NULL)
    	return (ast);
	add_env_vars_refs_to_tree(ast.ast_root, env_vars);
	return (ast);
}

static bool	check_pipe_syntax(t_token_lst *first, t_token_lst *last)
{
	if (!first || !last)
    	return (false);
	if (first->type == TOK_PIPE || last->type == TOK_PIPE)
		return (false);
	while (first && first != last)
	{
		if (first->type == TOK_PIPE && first->next->type == TOK_PIPE)
			return (false);
		first = first->next;
	}
	return (true);
}

static t_ast_node *parse_tokens_into_ast(t_token_lst *start, t_token_lst *end, t_parse_status *status)
{
    t_token_lst *pipe;
    t_ast_node  *ast_node;

    pipe = get_first_pipe(start, end);
    if (pipe)
    {
        ast_node = create_pipe_ast_node(status);
        if (*status != PARSE_OK)
            return (NULL);
        ast_node->left = parse_tokens_into_ast(start, pipe->previous, status);
        if (*status != PARSE_OK)
		{
			destroy_ast(ast_node);
            return (NULL);
		}
        ast_node->right = parse_tokens_into_ast(pipe->next, end, status);
		if (*status != PARSE_OK)
		{
			destroy_ast(ast_node);
            return (NULL);
		}
        return (ast_node);
    }
    return (new_command_node(start, end, status));
}

static void	add_env_vars_refs_to_tree(t_ast_node *tree_node, t_env_vars env_vars)
{
	if (!tree_node)
		return ;
	if (tree_node->type == NODE_PIPE)
	{
		add_env_vars_refs_to_tree(tree_node->left, env_vars);
		add_env_vars_refs_to_tree(tree_node->right, env_vars);
	}
	else if (tree_node->cmd)
		tree_node->cmd->env_vars = env_vars;
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
