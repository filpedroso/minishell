/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_lst_utils_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 18:51:15 by fpedroso          #+#    #+#             */
/*   Updated: 2026/02/10 00:34:48 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_var_lst	*alloc_t_var_list_node(void)
{
	t_var_lst	*node;

	node = malloc(sizeof(t_var_lst));
	if (!node)
		return (NULL);
	node->var_name = NULL;
	node->value = NULL;
	node->next = NULL;
	return (node);
}

void	destroy_env_list(t_var_lst *env_list)
{
	t_var_lst	*tmp;

	while (env_list)
	{
		tmp = env_list->next;
		free(env_list->var_name);
		free(env_list->value);
		free(env_list);
		env_list = tmp;
	}
}

void	cleanup_env(t_env_vars env_vars)
{
	if (env_vars.persistent_envs_ptr)
		destroy_env_list(env_vars.persistent_envs_ptr);
	if (env_vars.inline_envs)
		destroy_env_list(env_vars.inline_envs);
}
