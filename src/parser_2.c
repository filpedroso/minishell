/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 00:57:23 by fpedroso          #+#    #+#             */
/*   Updated: 2026/02/23 21:31:37 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_ast_node	*alloc_cmd_node(t_token_lst *start, t_token_lst *end,
						t_parse_status *status);
static t_ast_node	*malloc_node_and_cmd(void);
static void			alloc_cmd_words_and_redirs(t_cmd *cmd,
						t_token_lst *start, t_token_lst *end,
						t_parse_status *status);
static void			get_word_and_redir_count(int *word_count, int *redir_count,
						t_token_lst *start, t_token_lst *end);

t_ast_node	*new_command_node(t_token_lst *start, t_token_lst *end,
		t_parse_status *status)
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
			current = parse_word(new_cmd_node->cmd, current);
		if (*status != PARSE_OK)
		{
			destroy_cmd_node(new_cmd_node);
			return (NULL);
		}
	}
	if (is_builtin(new_cmd_node->cmd->words, new_cmd_node->cmd->words_count))
		new_cmd_node->cmd->type = BUILTIN;
	else
		new_cmd_node->cmd->type = EXT;
	return (new_cmd_node);
}

static t_ast_node	*alloc_cmd_node(t_token_lst *start, t_token_lst *end,
		t_parse_status *status)
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

static t_ast_node	*malloc_node_and_cmd(void)
{
	t_ast_node	*ast_node;

	ast_node = malloc(sizeof(t_ast_node));
	if (!ast_node)
		return (NULL);
	ast_node->left = NULL;
	ast_node->right = NULL;
	ast_node->type = NODE_CMD;
	ast_node->cmd = malloc(sizeof(t_cmd));
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

static void	alloc_cmd_words_and_redirs(t_cmd *cmd, t_token_lst *start,
		t_token_lst *end, t_parse_status *status)
{
	int	word_count;
	int	redir_count;

	get_word_and_redir_count(&word_count, &redir_count, start, end);
	cmd->words = malloc(sizeof(t_word) * (word_count));
	if (!cmd->words)
	{
		*status = PARSE_FATAL;
		return ;
	}
	if (redir_count)
	{
		cmd->redirections = malloc(sizeof(t_redirection) * (redir_count));
		if (!cmd->redirections && redir_count > 0)
		{
			free(cmd->words);
			*status = PARSE_FATAL;
			return ;
		}
	}
}

static void	get_word_and_redir_count(int *word_count, int *redir_count,
		t_token_lst *start, t_token_lst *end)
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
