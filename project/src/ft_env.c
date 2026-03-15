/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 21:49:45 by fpedroso          #+#    #+#             */
/*   Updated: 2026/03/15 17:34:19 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_env(char **envp)
{
	int		i;
	char	*eq;

	i = -1;
	while (envp[++i])
	{
		eq = ft_strchr(envp[i], '=');
		if (!eq)
			continue ;
		ft_putstr_fd(envp[i], 1);
		ft_putchar_fd('\n', 1);
	}
	return (0);
}
