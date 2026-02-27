/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 22:00:00 by fpedroso          #+#    #+#             */
/*   Updated: 2026/02/25 22:56:13 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	print_sorted_env(t_var_lst *lst);
static int	process_export_arg(t_sh *sh, char *arg);

int	ft_export(t_sh *sh, char **argv)
{
	int	i;
	int	ret;

	if (!argv[1])
		return (print_sorted_env(sh->env_vars.persistent_envs_ptr));
	ret = 0;
	i = 1;
	while (argv[i])
	{
		if (process_export_arg(sh, argv[i]) != 0)
			ret = 1;
		i++;
	}
	return (ret);
}

bool	is_valid_identifier(const char *str)
{
	int	i;

	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (false);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (false);
		i++;
	}
	return (true);
}

static int	process_export_arg(t_sh *sh, char *arg)
{
	char	*eq_pos;
	char	*name;
	int		ret;

	if (!is_valid_identifier(arg))
	{
		ft_putstr_fd("minishell: export: `", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		return (1);
	}
	eq_pos = ft_strchr(arg, '=');
	if (!eq_pos)
		return (set_env_var(&sh->env_vars.persistent_envs_ptr, arg, ""));
	name = ft_substr(arg, 0, eq_pos - arg);
	if (!name)
		return (1);
	ret = set_env_var(&sh->env_vars.persistent_envs_ptr, name, eq_pos + 1);
	free(name);
	return (ret);
}

static void	sort_str_arr(char **arr, int size)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (i < size - 1)
	{
		j = 0;
		while (j < size - i - 1)
		{
			if (ft_strncmp(arr[j], arr[j + 1], ft_strlen(arr[j]) + 1) > 0)
			{
				tmp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}
}

static void	print_declare_x(char *env_str)
{
	int	i;

	ft_putstr_fd("declare -x ", 1);
	i = 0;
	while (env_str[i] && env_str[i] != '=')
	{
		ft_putchar_fd(env_str[i], 1);
		i++;
	}
	if (env_str[i] == '=')
	{
		ft_putstr_fd("=\"", 1);
		ft_putstr_fd(env_str + i + 1, 1);
		ft_putchar_fd('"', 1);
	}
	ft_putchar_fd('\n', 1);
}

static int	print_sorted_env(t_var_lst *lst)
{
	char		**envs;
	int			size;
	int			i;

	envs = get_current_envs((t_env_vars){lst, NULL});
	if (!envs)
		return (1);
	size = 0;
	while (envs[size])
		size++;
	sort_str_arr(envs, size);
	i = 0;
	while (envs[i])
	{
		print_declare_x(envs[i]);
		i++;
	}
	free_str_arr(envs);
	return (0);
}
