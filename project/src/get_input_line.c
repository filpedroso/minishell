/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_input_line.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 19:04:18 by fpedroso          #+#    #+#             */
/*   Updated: 2026/03/21 10:07:03 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*read_not_at_tty(void);

char	*get_input_line(t_sh *sh)
{
	char	*input;

	if (isatty(STDIN_FILENO))
		input = readline(YELLOW "mini" PINK "sheron-" YELLOW "$ " RESET);
	else
		input = read_not_at_tty();
	if (g_signal == SIGINT)
	{
		g_signal = 0;
		sh->last_exit_st = 130;
	}
	if (!input)
	{
		if (isatty(STDIN_FILENO))
		{
			if (write(STDOUT_FILENO, "exit\n", 5) == -1)
				perror("write");
		}
		return (NULL);
	}
	if (*input)
		add_history(input);
	return (input);
}

static char	*read_not_at_tty(void)
{
	char	*input;
	ssize_t	read;

	input = NULL;
	input = get_next_line(STDIN_FILENO);
	if (!input)
		return (NULL);
	read = ft_strlen(input);
	if (input[read - 1] == '\n')
		input[read - 1] = '\0';
	return (input);
}
