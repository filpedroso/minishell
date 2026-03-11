/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_handle_redirections_2.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 18:39:46 by fpedroso          #+#    #+#             */
/*   Updated: 2026/02/28 21:18:52 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	collect_cmd_heredocs(t_cmd *cmd);
static int	append_filepath(char *filepath, t_str_lst **temp_files_list);

int	collect_all_heredocs(t_ast_node *node)
{
	if (!node)
		return (0);
	if (node->type == NODE_PIPE)
	{
		if (collect_all_heredocs(node->left) < 0)
			return (-1);
		return (collect_all_heredocs(node->right));
	}
	return (collect_cmd_heredocs(node->cmd));
}

static int	collect_cmd_heredocs(t_cmd *cmd)
{
	int		i;
	char	*filepath;

	if (!cmd || !cmd->redirections || cmd->redirections_count == 0)
		return (0);
	i = -1;
	while (++i < cmd->redirections_count)
	{
		if (cmd->redirections[i].type != REDIR_HEREDOC)
			continue ;
		filepath = create_temp_file(
				cmd->redirections[i].target.token_word_ptr,
				cmd->redirections[i].target.context_mask_ptr,
				cmd->env_vars);
		if (!filepath)
			return (-1);
		if (append_filepath(filepath, &cmd->temp_files_list) < 0)
		{
			unlink(filepath);
			free(filepath);
			return (-1);
		}
	}
	return (0);
}

static int	append_filepath(char *filepath, t_str_lst **temp_files_list)
{
	t_str_lst	*new_file_node;

	new_file_node = malloc(sizeof(t_str_lst));
	if (!new_file_node)
	{
		perror("malloc");
		return (-1);
	}
	new_file_node->value = filepath;
	new_file_node->next = NULL;
	if (!*temp_files_list)
		*temp_files_list = new_file_node;
	else
	{
		t_str_lst	*last;

		last = *temp_files_list;
		while (last->next)
			last = last->next;
		last->next = new_file_node;
	}
	return (0);
}
