/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 00:00:00 by fpedroso          #+#    #+#             */
/*   Updated: 2026/01/19 11:25:06 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_token(t_token_lst *token)
{
	if (!token)
		return ;
	if (token->segment)
		free(token->segment);
	if (token->seg_mask)
		free(token->seg_mask);
	free(token);
}

t_token_type	get_token_type(t_token_lst *token)
{
	size_t	len;

	len = ft_strlen(token->segment);
	if (len == 1 && token->segment[0] == '|')
		return (TOK_PIPE);
	if (len == 1 && token->segment[0] == '<')
		return (TOK_REDIR_IN);
	if (len == 1 && token->segment[0] == '>')
		return (TOK_REDIR_OUT);
	if (len == 2 && ft_strncmp(token->segment, "<<", 2) == 0)
		return (TOK_HEREDOC);
	if (len == 2 && ft_strncmp(token->segment, ">>", 2) == 0)
		return (TOK_APPEND);
	return (TOK_WORD);
}
