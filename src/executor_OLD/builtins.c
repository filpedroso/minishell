/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 00:00:00 by fpedroso          #+#    #+#             */
/*   Updated: 2026/02/10 00:35:23 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

int	ft_echo(char **args, char **envp)
{
	(void)args;
	(void)envp;
	printf("echo\n");
	return (0);
}

int	ft_cd(char **args, char **envp)
{
	(void)args;
	(void)envp;
	printf("cd\n");
	return (0);
}

int	ft_pwd(char **args, char **envp)
{
	(void)args;
	(void)envp;
	printf("pwd\n");
	return (0);
}

int	ft_export(char **args, char **envp)
{
	(void)args;
	(void)envp;
	printf("export\n");
	return (0);
}

int	ft_unset(char **args, char **envp)
{
	(void)args;
	(void)envp;
	printf("unset\n");
	return (0);
}

int	ft_env(char **args, char **envp)
{
	(void)args;
	(void)envp;
	printf("env\n");
	return (0);
}

int	ft_exit(char **args, char **envp)
{
	(void)args;
	(void)envp;
	printf("exit\n");
	return (0);
}
