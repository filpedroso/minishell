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

t_token_lst	*lexer(char **input)
{
	t_token_lst	*tokens_lst;
    t_token_lst	*new_token;

    tokens_lst = NULL;
    while (*input)
    {
        new_token = get_next_token(input);
        if (!new_token)
			continue ;
		tok_lst_add_back(tokens_lst, new_token);
    }
    return (tokens_lst);
}



/* if (!tokens_lst)
	tokens_lst = new_token;
else
	tail->next = new_token;
tail = new_token; */
