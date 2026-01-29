/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 01:31:30 by fpedroso          #+#    #+#             */
/*   Updated: 2026/01/29 01:31:30 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	skip_spaces(char **input)
{
	if (!input || !*input)
		return ;
	while (**input && (**input == ' ' || **input == '\t'))
		(*input)++;
}

int	is_operator(char c)
{
	return (c == '|' || c == '>' || c == '<');
}
