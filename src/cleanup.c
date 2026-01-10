/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 19:39:34 by fpedroso          #+#    #+#             */
/*   Updated: 2025/12/16 19:39:34 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.c"

void	cleanup_node(t_node *node)
{
	if (node->type == NODE_PIPE)
	{
		free(node);
		node = NULL;
		return ;
	}
	free_str_arr(node->cmd->args);
	clean_cmd_env_vars(node->cmd->env_vars);
	clean_cmd_redirections(node->cmd->redirections);
	ft_intr_lstclear(node->cmd->temp_files_list, del_temp_file_node);
	node->cmd = NULL;
	node = NULL;
}

void	free_str_arr(char **str_arr)
{
	int	i;

	if (!str_arr)
		return ;
	i = 0;
	while (str_arr[i])
	{
		free(str_arr[i]);
		str_arr[i] = NULL;
		i++;
	}
	free(str_arr);
	str_arr = NULL;
}

void	clean_cmd_env_vars(t_env_vars env_struct)
{
	ft_intr_lstclear(env_struct.inline_envs, del_env_list_node);
	ft_intr_lstclear(env_struct.persistent_envs, del_env_list_node);
}

void	del_env_list_node(void *node)
{
	t_var_lst_node	*env_list_node;

	env_list_node = (t_var_lst_node *)
		(char *)node - offsetof(t_var_lst_node, node);
	if (env_list_node->value)
		free(env_list_node->value);
	if (env_list_node->var_name)
		free(env_list_node->var_name);
	env_list_node->value = NULL;
	env_list_node->var_name = NULL;
}

void	clean_cmd_redirections(t_redirection *redirection)
{
	if (redirection->arg)
		free(redirection->arg);
	if (redirection->target)
		free(redirection->target);
	redirection->arg = NULL;
	redirection->target = NULL;
	redirection = NULL;
}

void	del_temp_file_node(void *node)
{
	t_file_lst_node	*temp_file_node;

	temp_file_node = (t_file_lst_node *)
		(char *)node - offsetof(t_file_lst_node, node);
	if (temp_file_node->path)
	{
		unlink(temp_file_node->path);
		free(temp_file_node->path);
		temp_file_node->path = NULL;
	}
	temp_file_node = NULL;
	return;
}
