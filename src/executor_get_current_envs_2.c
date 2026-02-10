/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_get_current_envs_2.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 20:44:49 by fpedroso          #+#    #+#             */
/*   Updated: 2026/02/10 00:34:52 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	var_lst_size(t_var_lst *lst)
{
	int			len;
	t_var_lst	*ptr;

	if (!lst)
		return (0);
	ptr = lst;
	len = 0;
	while (ptr)
	{
		len++;
		ptr = ptr->next;
	}
	return (len);
}

bool	lst_has_var(t_var_lst *lst, const char *var_name)
{
	while (lst)
	{
		if (ft_strncmp(lst->var_name, var_name, ft_strlen(var_name)) == 0)
			return (true);
		lst = lst->next;
	}
	return (false);
}

t_var_lst	*copy_var_list(t_var_lst *src_lst)
{
	t_var_lst	*new_head;
	t_var_lst	*new_node;
	t_var_lst	*tail;

	if (!src_lst)
		return (NULL);
	new_head = NULL;
	tail = NULL;
	while (src_lst)
	{
		new_node = duplicate_node(src_lst);
		if (!new_node)
		{
			destroy_env_list(new_head);
			return (NULL);
		}
		if (!new_head)
			new_head = new_node;
		else
			tail->next = new_node;
		tail = new_node;
		src_lst = src_lst->next;
	}
	return (new_head);
}

bool	append_var_to_list(t_var_lst **lst_ptr, t_var_lst *src_node)
{
	t_var_lst	*new_node;
	t_var_lst	*tail;

	new_node = duplicate_node(src_node);
	if (!new_node)
		return (false);
	if (!*lst_ptr)
	{
		*lst_ptr = new_node;
		return (true);
	}
	tail = *lst_ptr;
	while (tail->next)
		tail = tail->next;
	tail->next = new_node;
	return (true);
}

t_var_lst	*duplicate_node(t_var_lst *src_node)
{
	t_var_lst	*new_node;

	if (!src_node)
		return (NULL);
	new_node = malloc(sizeof(t_var_lst));
	if (!new_node)
		return (NULL);
	new_node->var_name = ft_strdup(src_node->var_name);
	if (!new_node->var_name)
	{
		free(new_node);
		return (NULL);
	}
	new_node->value = ft_strdup(src_node->value);
	if (!new_node->value)
	{
		free(new_node->var_name);
		free(new_node);
		return (NULL);
	}
	new_node->next = NULL;
	return (new_node);
}
