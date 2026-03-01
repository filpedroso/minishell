/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_input_line.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 19:04:18 by fpedroso          #+#    #+#             */
/*   Updated: 2026/02/28 21:10:07 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_input_line(t_sh *sh)
{
	char	*input;

	input = readline("> ");
	if (g_signal == SIGINT)
	{
		g_signal = 0;
		sh->last_exit_st = 130;
	}
	if (!input)
	{
		write(STDOUT_FILENO, "exit\n", 5);
		return (NULL);
	}
	if (*input)
		add_history(input);
	return (input);
}
