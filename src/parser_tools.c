/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_tools.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 17:55:38 by fpedroso          #+#    #+#             */
/*   Updated: 2026/01/22 17:55:38 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token_lst	*tok_lstlast(t_token_lst *lst)
{
	...;
}

bool	check_syntax(t_token_lst *first, t_token_lst *last)
{
	if (first->type == TOK_PIPE || last->type == TOK_PIPE)
	{
		return (false);
	}
	while (first != last)
	{
		if (first->type == TOK_PIPE && first->next->type == TOK_PIPE)
			return (false);
		first = first->next;
	}
	return (true);
}
