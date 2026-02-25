/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 20:38:15 by fpedroso          #+#    #+#             */
/*   Updated: 2026/02/24 20:38:15 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	has_n_flag(char **argv, int *i);

int	ft_echo(char **argv)
{
	int		i;
	bool	n_flag;

	i = 1;
	n_flag = has_n_flag(argv, &i);
	while (argv[i])
	{
		ft_putstr_fd(argv[i], 1);
		if (argv[i + 1])
			ft_putchar_fd(' ', 1);
		i++;
	}
	if (!n_flag)
		ft_putchar_fd('\n', 1);
	return (0);
}

static bool	has_n_flag(char **argv, int *i)
{
	bool	n_flag;
	int		j;

	n_flag = false;
	while (argv[*i] && argv[*i][0] == '-' && argv[*i][1] == 'n')
	{
		j = 1;
		while (argv[*i][j] == 'n')
			j++;
		if (argv[*i][j] != '\0')
			break ;
		n_flag = true;
		(*i)++;
	}
	return (n_flag);
}