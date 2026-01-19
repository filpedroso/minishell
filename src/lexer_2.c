/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 16:37:08 by fpedroso          #+#    #+#             */
/*   Updated: 2026/01/19 16:37:08 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	default_state_op(t_lexer_state *state, t_token_lst *token, char c);
static void	operator_state_op(t_lexer_state *state, t_token_lst *token, char c);
static void	double_qt_state_op(t_lexer_state *state, t_token_lst *token, char c);
static void	single_qt_state_op(t_lexer_state *state, t_token_lst *token, char c);

void	state_machine_tokenizer(t_lexer_state *st, t_token_lst *tok, char c)
{
	if (*st == STATE_OPERATOR || ( is_operator(c) && *st == STATE_NORMAL ))
		return (operator_state_op(st, tok, c));
	if (*st == STATE_NORMAL)
		return (default_state_op(st, tok, c));
	if (*st == STATE_DOUB_QUOTE)
		return (double_qt_state_op(st, tok, c));
	if (*st == STATE_SING_QUOTE)
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

static void	operator_state_op(t_lexer_state *state, t_token_lst *token, char c)
{
	if (!is_operator(c))
	{
		*state = STATE_TOK_END_NO_EAT;
		return ;
	}
	if (!token->segment)
	{
		if (c != '|')
			*state = STATE_OPERATOR;
		else
			*state = STATE_TOK_END;
		return (push_char(token, c, 'o', state));
	}
	if (ft_strlen(token->segment) == 1 && token->segment[0] == c)
	{
		*state = STATE_TOK_END;
		return (push_char(token, c, 'o', state));
	}
	*state = STATE_TOK_END_NO_EAT;
	return ;
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
