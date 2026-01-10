/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 04:31:27 by lcosta-a          #+#    #+#             */
/*   Updated: 2026/01/10 10:57:47 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_env_vars	init_env_list_struct(char **envp);

int	main(char **envp)
{
	t_env_vars	env_vars;

	env_vars = init_env_list_struct(envp);
	minishell_routine(env_vars);
	return (0);
}

static t_env_vars	init_env_list_struct(char **envp)
{
	t_env_vars	env_vars;

	env_vars.persistent_envs = convert_envp_to_env_list(envp);
	if (!env_vars.persistent_envs)
		exit(1);
	env_vars.inline_envs = NULL;
	return (env_vars);
}
