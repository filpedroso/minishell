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

t_token_lst		*alloc_null_tok(void)
{
	t_token_lst		*token;

	token = malloc(sizeof(t_token_lst));
	if (!token)
		return (NULL);
	token->segment = NULL;
	token->seg_mask = NULL;
	token->next = NULL;
	token->previous = NULL;
	return (token);
}

void	tok_lst_add_back(t_token_lst **lst, t_token_lst *new)
{
	t_token_lst	*last;

	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	last = *lst;
	while(last->next)
	{
		last = last->next;
	}
	last->next = new;
}

void	free_tok_lst(t_token_lst *lst)
{
	t_token_lst	*current;
	t_token_lst	*next;

	current = lst;
	while (current)
	{
		next = current->next;
		free_token(current);
		current = next;
	}
}

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
