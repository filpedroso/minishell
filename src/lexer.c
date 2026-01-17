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

// check if that "continue" will really work for empty tokens
// start coding get_next_token with the state machine
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

t_token_lst	*get_next_token(char **input)
{
	t_token_lst		*new_token;
	t_lexer_state	state;

	new_token = (t_token_lst*)malloc(sizeof(t_token_lst));
	if (!new_token)
		return (NULL);
	state = STATE_NORMAL;
	while(**input && state != STATE_TOK_END)
	{
		state_machine_tokenizer(&state, new_token, **input);
		if (state == STATE_ERROR)
		{
			free_token(new_token);
			return (NULL);
		}
		(*input)++;
	}
	new_token->next = NULL;
	new_token->previous = NULL;
	return (new_token);
}

void	state_machine_tokenizer(t_lexer_state *st, t_token_lst *tok, char c)
{
	if (state == STATE_NORMAL)
		return (default_state_op(st, tok, c));
	if (state == STATE_DOUB_QUOTE)
		return (double_qt_state_op(st, tok, c));
	if (state == STATE_SING_QUOTE)
		return (single_qt_state_op(st, tok, c));
}

static void	default_state_op(t_lexer_state *state, t_token_lst *token, char c)
{
	if (c == ' ')
	{
		*state = STATE_TOK_END;
		return ;
	}
	if (c == '"')
	{
		*state = STATE_DOUB_QUOTE;
		return ;
	}
	if (c == '\'')
	{
		*state = STATE_SING_QUOTE;
		return ;
	}
	return (push_char(token, c, 'n', state));
}

static void	double_qt_state_op(t_lexer_state *state, t_token_lst *token, char c)
{
	if (c == '"')
	{
		*state = STATE_NORMAL;
		return ;
	}
	return (push_char(token, c, 'd', state));
}

static void	single_qt_state_op(t_lexer_state *state, t_token_lst *token, char c)
{
	if (c == '\'')
	{
		*state = STATE_NORMAL;
		return ;
	}
	return (push_char(token, c, 's', state));
}

void	push_char(t_token_lst *token, char c, char mask, t_lexer_state *state)
{
	token->value = ft_realloc(1);
	token->context_mask = ft_realloc(1);
	if (!token->value || !token->context_mask)
	{
		*state = STATE_ERROR;
		return ;
	}
	token->value[ft_strlen(token->value) - 1] = c;
	token->context_mask[ft_strlen(token->context_mask) - 1] = mask;
	return ;
}
