/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 20:48:44 by fpedroso          #+#    #+#             */
/*   Updated: 2026/02/24 20:48:44 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_exit(t_sh *sh, char **argv)
{
	int	code;

	ft_putstr_fd("exit", 1);
	if (!argv[1])
		exit(sh->last_exit_st);
	if (!ft_is_numeric_str(argv[1]))
	{
		ft_putstr_fd("exit: ", 2);
		ft_putstr_fd(argv[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		exit(2);
	}
	code = ft_atoi(argv[1]);
	exit((unsigned char)code);
}
