/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 22:00:00 by fpedroso          #+#    #+#             */
/*   Updated: 2026/02/25 22:56:13 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_valid_unset_id(const char *str);

int	ft_unset(t_sh *sh, char **argv)
{
	int	i;
	int	ret;

	ret = 0;
	i = 1;
	while (argv[i])
	{
		if (!is_valid_unset_id(argv[i]))
		{
			ft_putstr_fd("minishell: unset: `", 2);
			ft_putstr_fd(argv[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			ret = 1;
		}
		else
			remove_env_var(&sh->env_vars.persistent_envs_ptr, argv[i]);
		i++;
	}
	return (ret);
}

static bool	is_valid_unset_id(const char *str)
{
	int	i;

	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (false);
	i = 1;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (false);
		i++;
	}
	return (true);
}
