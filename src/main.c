/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 04:31:27 by lcosta-a          #+#    #+#             */
/*   Updated: 2026/02/10 00:34:57 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	init_env_list_struct(char **envp, t_env_vars *env_vars);
static int	init_shell(t_sh *shell, char **envp);

int	main(int argc, char **argv, char **envp)
{
	t_sh	shell;

	(void)argc;
	(void)argv;
	if (init_shell(&shell, envp) == ERROR)
		return (1);
	if (minishell_routine(&shell) == ERROR)
	{
		cleanup_env(shell.env_vars);
		return (1);
	}
	cleanup_env(shell.env_vars);
	return (0);
}

static int	init_shell(t_sh *shell, char **envp)
{
	shell->last_exit_st = 0;
	return (init_env_list_struct(envp, &shell->env_vars));
}

static int	init_env_list_struct(char **envp, t_env_vars *env_vars)
{
	env_vars->persistent_envs_ptr = NULL;
	if (envp && envp[0])
	{
		env_vars->persistent_envs_ptr = envp_to_env_list(envp);
		if (!env_vars->persistent_envs_ptr)
			return (ERROR);
	}
	env_vars->inline_envs = NULL;
	return (SUCCESS);
}
