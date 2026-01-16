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
	state = STATE_DEFAULT;
	while(**input && state != STATE_TOK_END)
	{
		state_machine(&state, new_token, **input);
		(*input)++;
	}
	new_token->next = NULL;
	new_token->previous = NULL;
	new_token->type = get_token_type(new_token->value);
	return (new_token);
}
