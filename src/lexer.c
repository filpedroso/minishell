/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 15:33:45 by fpedroso          #+#    #+#             */
/*   Updated: 2026/01/14 15:33:45 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


static t_token_lst	*get_next_token(char **input);
static void			init_get_next_token(char **input,
	t_token_lst **new_token, t_lexer_state *state);

t_token_lst	*lexer(char **input)
{
	t_token_lst	*tokens_lst;
    t_token_lst	*new_token;

    tokens_lst = NULL;
    while (**input)
    {
        new_token = get_next_token(input);
        if (!new_token)
		{
			free_tok_lst(tokens_lst);
			return (NULL);
		}
		tok_lst_add_back(&tokens_lst, new_token);
    }
    return (tokens_lst);
}

static t_token_lst	*get_next_token(char **input)
{
	t_token_lst		*new_token;
	t_lexer_state	state;

	init_get_next_token(input, &new_token, &state);
	if (!new_token)
		return (NULL);
	while(**input && state != STATE_TOK_END)
	{
		if (!**input)
			break ;
		state_machine_tokenizer(&state, new_token, **input);
		if (state == STATE_ERROR)
		{
			free_token(new_token);
			return (NULL);
		}
		if (state == STATE_TOK_END_NO_EAT)
			break ;
		if (state == STATE_NORMAL)
			skip_spaces(input);
		(*input)++;
	}
	new_token->type = get_token_type(new_token);
	return (new_token);
}

static void	init_get_next_token(char **input, t_token_lst **new_token, t_lexer_state *state)
{
	*new_token = alloc_null_tok();
	if (!new_token)
		return ;
	*state = STATE_NORMAL;
	skip_spaces(input);
}

void	push_char(t_token_lst *token, char c, char mask, t_lexer_state *state)
{
	size_t	tok_val_size;

	if (!token->segment)
		tok_val_size = 0;
	else
		tok_val_size = ft_strlen(token->segment);
	token->segment = ft_realloc(token->segment, tok_val_size, tok_val_size + 2);
	token->seg_mask = ft_realloc(token->seg_mask, tok_val_size, tok_val_size + 2);
	if (!token->segment || !token->seg_mask)
	{
		*state = STATE_ERROR;
		return ;
	}
	token->segment[tok_val_size] = c;
	token->seg_mask[tok_val_size] = mask;
	token->segment[tok_val_size + 1] = '\0';
	token->seg_mask[tok_val_size + 1] = '\0';
	return ;
}
