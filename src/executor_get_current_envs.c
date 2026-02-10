/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_get_current_envs.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 19:14:13 by fpedroso          #+#    #+#             */
/*   Updated: 2026/02/10 00:34:51 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char			**merge_env_lists_into_arr(t_var_lst *inline_lst,
						t_var_lst *persist_lst);
static t_var_lst	*merge_and_unduplicate(t_var_lst *inline_lst,
						t_var_lst *persist_lst);
static char			**arr_from_var_lst(t_var_lst *var_lst);
static char			*var_str_from_list_node(t_var_lst *var_list_node);

char	**get_current_envs(t_env_vars env_lists)
{
	char	**current_envs;

	current_envs = merge_env_lists_into_arr(env_lists.inline_envs,
			env_lists.persistent_envs_ptr);
	if (!current_envs)
	{
		perror("get envs failed");
		return (NULL);
	}
	return (current_envs);
}

static char	**merge_env_lists_into_arr(t_var_lst *inline_lst,
		t_var_lst *persist_lst)
{
	t_var_lst	*final_lst;
	char		**final_arr;

	final_lst = merge_and_unduplicate(inline_lst, persist_lst);
	if (!final_lst)
		return (NULL);
	final_arr = arr_from_var_lst(final_lst);
	if (!final_arr)
	{
		destroy_env_list(final_lst);
		return (NULL);
	}
	destroy_env_list(final_lst);
	return (final_arr);
}

static t_var_lst	*merge_and_unduplicate(t_var_lst *inline_lst,
		t_var_lst *persist_lst)
{
	t_var_lst	*merged;
	t_var_lst	*current;

	merged = copy_var_list(inline_lst);
	if (!merged && inline_lst)
		return (NULL);
	current = persist_lst;
	while (current)
	{
		if (!lst_has_var(merged, current->var_name))
		{
			if (!append_var_to_list(&merged, current))
			{
				destroy_env_list(merged);
				return (NULL);
			}
		}
		current = current->next;
	}
	return (merged);
}

static char	**arr_from_var_lst(t_var_lst *var_lst)
{
	char	**str_arr;
	int		i;

	str_arr = malloc((var_lst_size(var_lst) + 1) * sizeof(char *));
	if (!str_arr)
		return (NULL);
	i = 0;
	while (var_lst)
	{
		str_arr[i] = var_str_from_list_node(var_lst);
		if (!str_arr[i])
		{
			free_str_arr(str_arr);
			return (NULL);
		}
		i++;
		var_lst = var_lst->next;
	}
	str_arr[i] = NULL;
	return (str_arr);
}

static char	*var_str_from_list_node(t_var_lst *var_list_node)
{
	char	*str;
	char	*tmp;

	tmp = ft_strjoin(var_list_node->var_name, "=");
	if (!tmp)
		return (NULL);
	str = ft_strjoin(tmp, var_list_node->value);
	free(tmp);
	if (!str)
		return (NULL);
	return (str);
}
