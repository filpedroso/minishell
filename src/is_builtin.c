/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 22:23:01 by fpedroso          #+#    #+#             */
/*   Updated: 2026/02/10 00:34:55 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	mask_is_all_n(const char *mask);
static bool	is_builtin_name(const char *s);

bool	is_builtin(t_word *words, int words_amount)
{
	int	i;

	if (!words)
		return (false);
	i = -1;
	while (++i < words_amount)
	{
		if (!mask_is_all_n(words[i].context_mask_ptr))
			continue ;
		if (is_builtin_name(words[i].token_word_ptr))
			return (true);
	}
	return (false);
}
static bool	mask_is_all_n(const char *mask)
{
	int	i;

	if (!mask)
		return (false);
	i = 0;
	while (mask[i])
	{
		if (mask[i] != 'n')
			return (false);
		i++;
	}
	return (true);
}

static bool	is_builtin_name(const char *s)
{
	if (!s)
		return (false);
	return (!strcmp(s, "echo") || !strcmp(s, "cd") || !strcmp(s, "pwd")
		|| !strcmp(s, "export") || !strcmp(s, "unset") || !strcmp(s, "env")
		|| !strcmp(s, "exit"));
}
