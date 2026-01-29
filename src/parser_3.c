/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 01:02:16 by fpedroso          #+#    #+#             */
/*   Updated: 2026/01/29 01:02:16 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	add_redir_to_cmd(t_command *cmd,
	t_token_lst *tok_redir,
	t_token_lst *tok_target,
	t_parse_status *status);
static t_word				get_word_from_token(t_token_lst *token);
static t_redirection_type	get_redir_type_from_tok_type(t_token_type token_type);


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

static void	add_redir_to_cmd(t_command *cmd, t_token_lst *tok_redir, t_token_lst *tok_target, t_parse_status *status)
{
	t_redirection		redir;

	redir.type = get_redir_type_from_tok_type(tok_redir->type);
	redir.target = get_word_from_token(tok_target);
	cmd->redirections[cmd->redirections_count] = redir;
	cmd->redirections_count++;
}

static t_word	get_word_from_token(t_token_lst *token)
{
	t_word	word;

	word.token_word_ptr = token->segment;
	word.context_mask_ptr = token->seg_mask;
	return (word);
}

static t_redirection_type	get_redir_type_from_tok_type(t_token_type token_type)
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
