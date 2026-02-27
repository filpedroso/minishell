/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_lst_utils_3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 22:00:00 by fpedroso          #+#    #+#             */
/*   Updated: 2026/02/25 22:56:13 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int			change_node_value(t_var_lst *node, const char *new_value);
static t_var_lst	*new_env_node(const char *name, const char *value);

t_var_lst	*find_env_var(t_var_lst *lst, const char *name)
{
	size_t	name_len;

	if (!name)
		return (NULL);
	name_len = ft_strlen(name);
	while (lst)
	{
		if (ft_strncmp(lst->var_name, name, name_len + 1) == 0)
			return (lst);
		lst = lst->next;
	}
	return (NULL);
}

int	set_env_var(t_var_lst **lst, const char *name, const char *value)
{
	t_var_lst	*node;
	t_var_lst	*tail;

	node = find_env_var(*lst, name);
	if (node)
		return (change_node_value(node, value));
	node = new_env_node(name, value);
	if (!node)
		return (1);
	if (!*lst)
		*lst = node;
	else
	{
		tail = *lst;
		while (tail->next)
			tail = tail->next;
		tail->next = node;
	}
	return (0);
}

static t_var_lst	*new_env_node(const char *name, const char *value)
{
	t_var_lst	*node;

	node = alloc_t_var_list_node();
	if (!node)
		return (NULL);
	node->var_name = ft_strdup(name);
	if (!node->var_name)
	{
		free(node);
		return (NULL);
	}
	node->value = ft_strdup(value);
	if (!node->value)
	{
		free(node->var_name);
		free(node);
		return (NULL);
	}
	return (node);
}

static int	change_node_value(t_var_lst *node, const char *new_value)
{
	free(node->value);
	node->value = ft_strdup(new_value);
	if (!node->value)
		return (1);
	return (0);
}

void	remove_env_var(t_var_lst **lst, const char *name)
{
	t_var_lst	*current;
	t_var_lst	*prev;
	size_t		name_len;

	if (!lst || !*lst || !name)
		return ;
	name_len = ft_strlen(name);
	prev = NULL;
	current = *lst;
	while (current)
	{
		if (ft_strlen(current->var_name) == name_len
			&& ft_strncmp(current->var_name, name, name_len) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				*lst = current->next;
			free(current->var_name);
			free(current->value);
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}
