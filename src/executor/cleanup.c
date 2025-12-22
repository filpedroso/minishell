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
		return ;
	}
	free_str_arr(node->cmd->args);
	clean_cmd_env_vars(node->cmd->env_vars);
	clean_cmd_redirections(node->cmd->redirections);
	clean_cmd_temp_files();
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

void	clean_cmd_env_vars()
{
	//incomplete!
	printf("Code clean_cmd_env_vars() cleanup function!\n");
}

void	clean_cmd_redirections()
{
	//incomplete!
	printf("Code clean_cmd_redirections() cleanup function!\n");
}

void	clean_temclean_cmd_temp_filesp_files()
{
	//incomplete!
	printf("Code clean_cmd_temp_files() cleanup function!\n");
}
