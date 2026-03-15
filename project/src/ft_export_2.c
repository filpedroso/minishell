/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/12 16:28:48 by fpedroso          #+#    #+#             */
/*   Updated: 2026/03/15 18:25:32 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	change_node_value_special(t_var_lst *node);

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

int	special_value_not_set(t_var_lst **lst, const char *name)
{
	t_var_lst	*node;
	t_var_lst	*tail;

	node = find_env_var(*lst, name);
	if (node)
		return (change_node_value_special(node));
	node = new_env_node(name, "");
	if (!node)
		return (1);
	node->is_set = false;
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

static int	change_node_value_special(t_var_lst *node)
{
	if (!node)
		return (1);
	free(node->value);
	node->value = ft_strdup("");
	if (!node->value)
		return (1);
	node->is_set = false;
	return (0);
}
