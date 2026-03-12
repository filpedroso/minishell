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

static int	collect_cmd_heredocs(t_sh *sh, t_cmd *cmd);
static int	append_filepath(char *filepath, t_str_lst **list);
static int	register_heredoc(t_sh *sh, char *filepath);

int	collect_all_heredocs(t_sh *sh, t_ast_node *node)
{
	if (!node)
		return (0);
	if (node->type == NODE_PIPE)
	{
		if (collect_all_heredocs(sh, node->left) < 0)
			return (-1);
		return (collect_all_heredocs(sh, node->right));
	}
	return (collect_cmd_heredocs(sh, node->cmd));
}

static int	collect_cmd_heredocs(t_sh *sh, t_cmd *cmd)
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
		if (append_filepath(filepath, &cmd->temp_files_list) < 0
			|| register_heredoc(sh, filepath) < 0)
		{
			unlink(filepath);
			return (-1);
		}
	}
	return (0);
}

static int	register_heredoc(t_sh *sh, char *filepath)
{
	char	*dup;

	dup = ft_strdup(filepath);
	if (!dup)
		return (-1);
	if (append_filepath(dup, &sh->heredoc_files) < 0)
	{
		free(dup);
		return (-1);
	}
	return (0);
}

static int	append_filepath(char *filepath, t_str_lst **list)
{
	t_str_lst	*new_node;
	t_str_lst	*last;

	new_node = malloc(sizeof(t_str_lst));
	if (!new_node)
		return (-1);
	new_node->value = filepath;
	new_node->next = NULL;
	if (!*list)
	{
		*list = new_node;
		return (0);
	}
	last = *list;
	while (last->next)
		last = last->next;
	last->next = new_node;
	return (0);
}

void	unlink_heredoc_files(t_str_lst **list)
{
	t_str_lst	*current;
	t_str_lst	*next;

	if (!list || !*list)
		return ;
	current = *list;
	while (current)
	{
		next = current->next;
		unlink(current->value);
		free(current->value);
		free(current);
		current = next;
	}
	*list = NULL;
}
