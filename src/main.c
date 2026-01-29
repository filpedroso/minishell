/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 04:31:27 by lcosta-a          #+#    #+#             */
/*   Updated: 2026/01/11 15:32:24 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	init_env_list_struct(char **envp, t_env_vars *env_vars);

int main(char **envp)
{
    t_env_vars	env_vars;

    if (init_env_list_struct(envp, &env_vars) == ERROR)
        return (1);
    if (minishell_routine(env_vars) == ERROR)
    {
        cleanup_env(env_vars);
        return (1);
    }
    cleanup_env(env_vars);
    return (SUCCESS);
}

static int init_env_list_struct(char **envp, t_env_vars *env_vars)
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
