/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_check_syntax.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 15:07:52 by fpedroso          #+#    #+#             */
/*   Updated: 2026/03/15 15:41:38 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool has_forbidden_chars(char *seg, char *mask);


bool	check_forbidden_chars(t_token_lst *tokens)
{
	t_token_lst	*cur;

	if (!tokens)
		return (false);
	cur = tokens;
	while(cur)
	{
		if (cur->type == TOK_WORD)
		{
			if (has_forbidden_chars(cur->segment, cur->seg_mask))
				return (false);
		}
		cur = cur->next;
	}
	return (true);
}

static bool has_forbidden_chars(char *seg, char *mask)
{
	int i;

	i = 0;
	while (seg[i])
	{
		if (mask[i] == CONTEXT_UNQUOTED)
		{
			if (seg[i] == '\\' || seg[i] == ';')
				return (true);
		}
		i++;
	}
	return (false);
}
