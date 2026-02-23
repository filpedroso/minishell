/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_expander_3.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 00:00:00 by fpedroso          #+#    #+#             */
/*   Updated: 2026/02/15 13:54:29 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*lookup_env_var(const char *name, int name_len, char **envs)
{
	int	i;

	i = 0;
	while (envs[i])
	{
		if (ft_strncmp(name, envs[i], name_len) == 0
			&& envs[i][name_len] == '=')
			return (ft_strdup(envs[i] + name_len + 1));
		i++;
	}
	return (ft_strdup(""));
}

char	*handle_exit_status_expansion(t_sh *sh, int *i, char *result)
{
	char	*value;

	*i += 2;
	value = ft_itoa(sh->last_exit_st);
	if (!value)
	{
		free(result);
		return (NULL);
	}
	result = join_and_free_left(result, value);
	free(value);
	return (result);
}
