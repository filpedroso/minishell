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

void			free_tok_lst(t_token_lst *lst);
t_token_lst		*get_next_token(char **input);
t_token_lst		*alloc_null_tok(void);
static void	state_machine_tokenizer(t_lexer_state *st, t_token_lst *tok, char c);
static void	default_state_op(t_lexer_state *state, t_token_lst *token, char c);
static void	operator_state_op(t_lexer_state *state, t_token_lst *token, char c);
static void	double_qt_state_op(t_lexer_state *state, t_token_lst *token, char c);
static void	single_qt_state_op(t_lexer_state *state, t_token_lst *token, char c);
static void	push_char(t_token_lst *token, char c, char mask, t_lexer_state *state);


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

t_token_lst	*get_next_token(char **input)
{
	t_token_lst		*new_token;
	t_lexer_state	state;

	new_token = alloc_null_tok();
	if (!new_token)
		return (NULL);
	state = STATE_NORMAL;
	while(**input && state != STATE_TOK_END)
	{
		if (state == STATE_NORMAL)
			skip_spaces(input);
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
		(*input)++;
	}
	new_token->type = get_token_type(new_token);
	return (new_token);
}

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

static void	state_machine_tokenizer(t_lexer_state *st, t_token_lst *tok, char c)
{
	if (*st == STATE_OPERATOR || is_operator(c))
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

static void	push_char(t_token_lst *token, char c, char mask, t_lexer_state *state)
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
