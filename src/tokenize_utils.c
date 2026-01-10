/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 03:13:12 by lcosta-a          #+#    #+#             */
/*   Updated: 2026/01/10 11:51:56 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	skip_spaces(char **input)
{
	while (**input && (**input == ' ' || **input == '\t'))
		(*input)++;
}

int	is_space(char c)
{
	return (c == ' ' || c == '\t'
		|| c == '\n' || c == '\v' || c == '\f' || c == '\r');
}

int	is_operator(char c)
{
	return (c == '|' || c == '>' || c == '<' || c == '&');
}

int	is_double_operator(char *input)
{
	return ((input[0] == '>' && input[1] == '>') ||
			(input[0] == '<' && input[1] == '<'));
}
