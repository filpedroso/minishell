/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_envp_to_env_list.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 19:39:57 by fpedroso          #+#    #+#             */
/*   Updated: 2026/01/10 10:59:59 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_var_lst	*env_node_from(char *env_str);

t_var_lst	*convert_envp_to_env_list(char **envp)
{
	t_var_lst	*env_list;
	t_var_lst	*new_node;
	t_var_lst	*tail;
	int			i;

	env_list = NULL;
	tail = NULL;
	i = 0;
	if (!envp)
		return (NULL);
	while (envp[i])
	{
		new_node = env_node_from(envp[i]);
		if (env_list == NULL)
			env_list = new_node;
		else
			tail->next = new_node;
		tail = new_node;
		i++;
	}
	return (env_list);
}

static t_var_lst	*env_node_from(char *env_str)
{
	t_var_lst	*env_node;
	char		**split_str;

	env_node = malloc(sizeof(t_var_lst));
	if (!env_node)
	{
		perror("Malloc in env list creation failed");
		exit(1);
	}
	split_str = ft_split(env_str, '=');
	if (!split_str)
	{
		perror("Split in env list creation failed");
		exit(1);
	}
	env_node->var_name = split_str[0];
	env_node->value = split_str[2];
	env_node->next = NULL;
	return (env_node);
}
