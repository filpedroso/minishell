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
//	                                                    */
//		            *** BE HAPPY! ***                   */
//		                                                */
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


t_ast	make_ast(t_token_lst *tok_lst, t_env_vars env_vars)
{
	t_ast		ast;
	t_token_lst	*last_tok;

	if (tok_lst)
		last_tok = tok_lst_last(tok_lst);
	if (!check_pipe_syntax(tok_lst, last_tok))
	{
		ft_putstr_fd("Parse error\n", 2);
		ast.parse_status = PARSE_ERROR;
		return (ast);
	}
	ast.parse_status = PARSE_OK;
	ast.ast_root = parse_tokens_into_ast(tok_lst, last_tok, &ast.parse_status);
	add_env_vars_refs_to_tree(ast.ast_root, env_vars);
	return (ast);
}


void	add_env_vars_refs_to_tree(t_ast_node *tree_node, t_env_vars env_vars)
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

/* 
** check for pipes in start & end,
** check for double pipes
*/
bool	check_pipe_syntax(t_token_lst *first, t_token_lst *last)
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

t_ast_node	*alloc_cmd_node(t_token_lst *start, t_token_lst *end, t_parse_status *status)
{
	t_ast_node	*ast_node;

	ast_node = malloc_node_and_cmd();
	if (!ast_node)
	{
		*status = PARSE_FATAL;
		return (NULL);
	}
	alloc_cmd_words_and_redirs(ast_node->cmd, start, end, status);
	if (*status != PARSE_OK)
	{
		destroy_cmd_node(ast_node);
		return (NULL);
	}
	return (ast_node);
}

void	alloc_cmd_words_and_redirs(t_command *cmd, t_token_lst *start,
	t_token_lst *end, t_parse_status *status)
{
    int	word_count;
    int	redir_count;

	get_word_and_redir_count(&word_count, &redir_count, start, end);
    cmd->words = malloc(sizeof(t_word) * (word_count));
    if (!cmd->words)
    {
        *status = PARSE_FATAL;
        return;
    }
	if (redir_count)
	{
		cmd->redirections = malloc(sizeof(t_redirection) * (redir_count));
		if (!cmd->redirections && redir_count > 0)
		{
			free(cmd->words);
			*status = PARSE_FATAL;
			return;
		}
	}
}

void	get_word_and_redir_count(int *word_count, int *redir_count, t_token_lst *start, t_token_lst *end)
{
	t_token_lst	*current;

	*word_count = 0;
	*redir_count = 0;
    current = start;
    while (current && current != end->next)
    {
        if (is_tok_redirection(current->type))
        {
            (*redir_count)++;
            current = current->next;
        }
        else
        	(*word_count)++;
		if (current)
			current = current->next;
    }
}

t_ast_node	*malloc_node_and_cmd(void)
{
	t_ast_node	*ast_node;

	ast_node = malloc(sizeof(t_ast_node));
	if (!ast_node)
		return (NULL);
	ast_node->left = NULL;
	ast_node->right = NULL;
	ast_node->type = NODE_CMD;
	ast_node->cmd = malloc(sizeof(t_command));
	if (!ast_node->cmd)
	{
		free(ast_node);
		return (NULL);
	}
	ast_node->cmd->words_count = 0;
	ast_node->cmd->redirections_count = 0;
	ast_node->cmd->words = NULL;
	ast_node->cmd->redirections = NULL;
	ast_node->cmd->temp_files_list = NULL;
	return (ast_node);
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

bool	is_tok_redirection(t_token_type tok_type)
{
	return (tok_type == TOK_APPEND || tok_type == TOK_REDIR_IN
		|| tok_type == TOK_REDIR_OUT || tok_type == TOK_HEREDOC);
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

	new_cmd_node = alloc_cmd_node(start, end, status); // allocates the node, its cmd, redirs and everything it will need
	if (*status != PARSE_OK)
		return (NULL);
	current = start;
	while (current && current != end->next)
	{
		if (is_tok_redirection(current->type))
			current = parse_redirection(new_cmd_node->cmd, current, status);
		else
			current = parse_word(new_cmd_node->cmd, current, status);
		if (*status != PARSE_OK)
		{
			destroy_cmd_node(new_cmd_node);
			return (NULL);
		}
	}
	if (is_builtin(new_cmd_node->cmd->words))
		new_cmd_node->cmd->type = BUILTIN;
	else
		new_cmd_node->cmd->type = EXT;
	return (new_cmd_node);
}

t_token_lst *parse_word(t_command *cmd, t_token_lst *token_node, t_parse_status *status)
{
	cmd->words[cmd->words_count] = get_word_from_token(token_node);
	cmd->words_count++;
    return (token_node->next);
}

t_token_lst	*parse_redirection(t_command *cmd, t_token_lst *token_node, t_parse_status *status)
{
    if (!token_node->next || token_node->next->type != TOK_WORD)
    {
        *status = PARSE_ERROR;
        return (NULL);
    }
    add_redir_to_cmd(cmd, token_node, token_node->next, status);
    return (token_node->next->next);
}

void	add_redir_to_cmd(t_command *cmd, t_token_lst *tok_redir, t_token_lst *tok_target, t_parse_status *status)
{
	t_redirection		redir;

	redir.type = get_redir_type_from_tok_type(tok_redir->type);
	redir.target = get_word_from_token(tok_target);
	cmd->redirections[cmd->redirections_count] = redir;
	cmd->redirections_count++;
}

t_word	get_word_from_token(t_token_lst *token)
{
	t_word	word;

	word.token_word_ptr = token->segment;
	word.context_mask_ptr = token->seg_mask;
	return (word);
}


t_redirection_type	get_redir_type_from_tok_type(t_token_type token_type)
{
	if (token_type == TOK_REDIR_IN)
		return (REDIR_IN);
	if (token_type == TOK_REDIR_OUT)
		return (REDIR_OUT);
	if (token_type == TOK_APPEND)
		return (REDIR_APPEND);
	if (token_type == TOK_HEREDOC)
		return (REDIR_HEREDOC);
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
