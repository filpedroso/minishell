/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_lst_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 18:34:38 by fpedroso          #+#    #+#             */
/*   Updated: 2026/02/10 00:34:48 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_env_var_name(char *str);
static char	*get_env_value(char *str);

t_var_lst	*envp_to_env_list(char **envp)
{
	t_var_lst	*env_list;
	t_var_lst	*last;
	t_var_lst	*new_node;
	int			i;

	i = 0;
	env_list = NULL;
	last = NULL;
	while (envp[i])
	{
		new_node = env_node_from_str(envp[i]);
		if (!new_node)
		{
			destroy_env_list(env_list);
			return (NULL);
		}
		if (env_list)
			last->next = new_node;
		else
			env_list = new_node;
		last = new_node;
		i++;
	}
	return (env_list);
}

t_var_lst	*env_node_from_str(char *str)
{
	t_var_lst	*node;

	if (!str)
		return (NULL);
	node = alloc_t_var_list_node();
	if (!node)
		return (NULL);
	node->var_name = get_env_var_name(str);
	if (!node->var_name)
	{
		free(node);
		return (NULL);
	}
	node->value = get_env_value(str);
	if (!node->value)
	{
		free(node->var_name);
		free(node);
		return (NULL);
	}
	return (node);
}

static char	*get_env_var_name(char *str)
{
	char	*var_name;
	int		i;

	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	var_name = ft_substr(str, 0, i);
	if (!var_name)
		return (NULL);
	return (var_name);
}

static char	*get_env_value(char *str)
{
	char	*value;
	int		i;

	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	if (str[i] == '=')
	{
		value = ft_strdup(str + i + 1);
		if (!value)
			return (NULL);
	}
	else
	{
		value = ft_strdup("");
		if (!value)
			return (NULL);
	}
	return (value);
}
