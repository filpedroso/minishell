/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_current_envs.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 19:14:13 by fpedroso          #+#    #+#             */
/*   Updated: 2026/01/10 10:24:14 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**merge_env_lists(t_var_list *inline_lst, t_var_list *persist_lst);
static int	fill_arr_from_lists(char **arr, t_var_list *lst_1, t_var_list *lst_2);

char	**get_current_envs(t_env_vars env_lists)
{
	char	**current_envs;

	current_envs =
		merge_env_lists(env_lists.inline_envs, env_lists.persistent_envs);
	if (!current_envs)
	{
		perror("get envs failed");
		return (NULL);
	}
	return (current_envs);
}

static char	**merge_env_lists(t_var_list *inline_lst, t_var_list *persist_lst)
{
	char	**vars_array;
	int		inline_amount;
	int		persistent_amount;

	inline_amount = ft_lstsize(inline_lst);
	persistent_amount = ft_lstsize(persist_lst);
	vars_array = (char **)malloc(
		(inline_amount + persistent_amount + 1) * sizeof(char *));
	if (!vars_array)
		return (NULL);
	vars_array[inline_amount + persistent_amount] = NULL;
	if (!fill_arr_from_lists(vars_array, inline_lst, persist_lst))
	{
		free_str_arr(vars_array);
		return (NULL);
	}
	return (vars_array);
}

static int	fill_arr_from_lists(char **arr, t_var_list *lst_1, t_var_list *lst_2)
{
	int	i;

	i = 0;
	while (lst_1)
	{
		arr[i] = ft_strdup(lst_1->var_name);
		if (!arr[i])
			return (NULL);
		arr[i] = ft_strjoin(arr[i], "=");
		if (!arr[i])
			return (NULL);
		arr[i] = ft_strjoin(arr[i], lst_1->value);
		if (!arr[i])
			return (NULL);
		i++;
		lst_1 = lst_1->next;
	}
	//incomplete!
	//TODO
	//fill_second_list
}
