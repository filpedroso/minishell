/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_logic.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 20:17:35 by fpedroso          #+#    #+#             */
/*   Updated: 2026/02/10 00:35:21 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

int	exec_forked_builtin(t_node *node)
{
	pid_t	pid;

	pid = fork();
	if (pid == CHILD)
		exec_builtin(node);
	wait(NULL);
	return (1);
}

int	exec_builtin(t_node *node)
{
	t_builtin	builtin_table[N_BUILTINS];
	int			i;

	init_builtin_table(builtin_table);
	i = 0;
	while (i < N_BUILTINS)
	{
		if (ft_strcmp(node->cmds[0], builtin_table[i].name) == 0)
			return (builtin_table[i].func(node->cmds, node->envs));
		i++;
	}
	return (-1);
}

void	init_builtin_table(t_builtin table[N_BUILTINS])
{
	table[0].name = "echo";
	table[0].func = &ft_echo;
	table[1].name = "cd";
	table[1].func = &ft_cd;
	table[2].name = "pwd";
	table[2].func = &ft_pwd;
	table[3].name = "export";
	table[3].func = &ft_export;
	table[4].name = "unset";
	table[4].func = &ft_unset;
	table[5].name = "env";
	table[5].func = &ft_env;
	table[6].name = "exit";
	table[6].func = &ft_exit;
}
