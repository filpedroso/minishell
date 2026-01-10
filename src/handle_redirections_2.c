/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirections_2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 18:39:46 by fpedroso          #+#    #+#             */
/*   Updated: 2025/12/29 16:10:21 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static int	dup_file_into_stdin(char *heredoc_filepath);
static char	*create_temp_file(char *delim);
static int	append_filepath(char *filepath, t_intr_list *temp_files_list);

int	set_heredoc_redir(t_command *cmd, char *heredoc_delim)
{
	char	*heredoc_filepath;
	int		heredoc_fd;

	heredoc_filepath = create_temp_file(heredoc_delim);
	if (!heredoc_filepath)
	{
		perror("heredoc reader");
		return (-1);
	}
	if (dup_file_into_stdin(heredoc_filepath) < 0)
	{
		free(heredoc_filepath);
		unlink(heredoc_filepath);
		return (-1);
	}
	if (append_filepath(heredoc_filepath, cmd->temp_files_list) < 0)
	{
		free(heredoc_filepath);
		unlink(heredoc_filepath);
		return (-1);
	}
	return (0);
}

static int	dup_file_into_stdin(char *heredoc_filepath)
{
	int	heredoc_fd;

	heredoc_fd = open(heredoc_filepath, O_RDONLY);
	if (heredoc_fd < 0)
	{
		perror("heredoc temp file");
		return (-1);
	}
	if (dup2(heredoc_fd, STDIN_FILENO) < 0)
	{
		perror("heredoc dup2");
		close(heredoc_fd);
		return (-1);
	}
	if (close(heredoc_fd) < 0)
	{
		perror("heredoc temp file close");
		return (-1);
	}
	return (0);
}

static int	append_filepath(char *filepath, t_intr_list *temp_files_list)
{
	t_file_lst_node	*file_node;

	file_node = (t_file_lst_node *)malloc(sizeof(t_file_lst_node));
	if (!file_node)
	{
		perror("malloc");
		return (-1);
	}
	file_node->path = filepath;
	ft_intr_lstadd_back(&temp_files_list, &file_node->node);
	return (0);
}
