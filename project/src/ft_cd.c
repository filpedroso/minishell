/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 21:59:25 by fpedroso          #+#    #+#             */
/*   Updated: 2026/02/25 22:56:13 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	update_pwd_vars(t_sh *sh, const char *old_pwd);
static char	*get_cd_target(t_sh *sh, char **argv);

int	ft_cd(t_sh *sh, char **argv)
{
	char	old_pwd[PATH_MAX];
	char	*target;

	if (argv[1] && argv[2])
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		return (1);
	}
	target = get_cd_target(sh, argv);
	if (!target)
		return (1);
	if (!getcwd(old_pwd, PATH_MAX))
		old_pwd[0] = '\0';
	if (chdir(target) != 0)
	{
		perror("cd:");
		return (1);
	}
	return (update_pwd_vars(sh, old_pwd));
}

static char	*get_cd_target(t_sh *sh, char **argv)
{
	t_var_lst	*home_node;

	if (argv[1])
	{
		if (ft_strncmp(argv[1], "-", 2) == 0)
			return (find_env_var(sh->env_vars.persistent_envs_ptr,
					"OLDPWD")->value);
		return (argv[1]);
	}
	home_node = find_env_var(sh->env_vars.persistent_envs_ptr, "HOME");
	if (!home_node)
	{
		ft_putstr_fd("minishell: cd: HOME not set\n", 2);
		return (NULL);
	}
	return (home_node->value);
}

static int	update_pwd_vars(t_sh *sh, const char *old_pwd)
{
	char	new_pwd[PATH_MAX];

	if (old_pwd[0] != '\0')
		set_env_var(&sh->env_vars.persistent_envs_ptr, "OLDPWD", old_pwd);
	if (getcwd(new_pwd, PATH_MAX))
		set_env_var(&sh->env_vars.persistent_envs_ptr, "PWD", new_pwd);
	return (0);
}
