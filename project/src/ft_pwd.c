/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 21:51:04 by fpedroso          #+#    #+#             */
/*   Updated: 2026/02/24 21:51:04 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_pwd(void)
{
	char	buf[PATH_MAX];

	if (!getcwd(buf, PATH_MAX))
	{
		perror("pwd");
		return (1);
	}
	ft_putstr_fd(buf, 1);
	ft_putchar_fd('\n', 1);
	return (0);
}
