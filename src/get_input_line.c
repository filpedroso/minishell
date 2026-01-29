/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_input_line.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 19:04:18 by fpedroso          #+#    #+#             */
/*   Updated: 2026/01/29 19:04:18 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_input_line(void)
{
	char	*input;

	input = readline("> ");
	if (!input)
		return (NULL);
	if (*input)
		add_history(input);
	return (input);
}
