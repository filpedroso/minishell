/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_handle_redirections.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 18:33:42 by fpedroso          #+#    #+#             */
/*   Updated: 2026/03/15 12:12:00 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	set_stdin_redir(t_redirection redirection);
static int	set_stdout_redir(t_redirection redirection);
static int	set_append_redir(t_redirection redirection);
static int	set_heredoc_stdin(char *filepath);

int	handle_redirections(t_cmd *cmd)
{
	int			i;
	int			status;
	t_str_lst	*hdoc_file;

	if (!cmd || !cmd->redirections || cmd->redirections_count == 0)
		return (0);
	hdoc_file = cmd->temp_files_list;
	i = -1;
	while (++i < cmd->redirections_count)
	{
		if (cmd->redirections[i].type == REDIR_IN)
			status = set_stdin_redir(cmd->redirections[i]);
		else if (cmd->redirections[i].type == REDIR_OUT)
			status = set_stdout_redir(cmd->redirections[i]);
		else if (cmd->redirections[i].type == REDIR_APPEND)
			status = set_append_redir(cmd->redirections[i]);
		else if (cmd->redirections[i].type == REDIR_HEREDOC && hdoc_file)
		{
			status = set_heredoc_stdin(hdoc_file->value);
			hdoc_file = hdoc_file->next;
		}
		if (status == STATUS_ERR)
			return (-1);
	}
	return (0);
}

static int	set_append_redir(t_redirection redirection)
{
	int	fd;

	fd = open(redirection.target.token_word_ptr, O_WRONLY | O_CREAT | O_APPEND,
			0644);
	if (fd < 0)
	{
		perror("Open append redir file");
		return (STATUS_ERR);
	}
	dup2(fd, STDOUT_FILENO);
	if (close(fd) < 0)
	{
		perror("Close");
		return (STATUS_ERR);
	}
	return ((STATUS_OK));
}

static int	set_stdout_redir(t_redirection redirection)
{
	int	fd;

	fd = open(redirection.target.token_word_ptr, O_WRONLY | O_CREAT | O_TRUNC,
			0644);
	if (fd < 0)
	{
		perror("Open stdout redir file");
		return (STATUS_ERR);
	}
	dup2(fd, STDOUT_FILENO);
	if (close(fd) < 0)
	{
		perror("Close");
		return (STATUS_ERR);
	}
	return ((STATUS_OK));
}

static int	set_stdin_redir(t_redirection redirection)
{
	int	fd;

	fd = open(redirection.target.token_word_ptr, O_RDONLY);
	if (fd < 0)
	{
		perror("Open stdin redir file");
		return (STATUS_ERR);
	}
	dup2(fd, STDIN_FILENO);
	if (close(fd) < 0)
	{
		perror("Close");
		return (STATUS_ERR);
	}
	return ((STATUS_OK));
}

static int	set_heredoc_stdin(char *filepath)
{
	int	fd;

	fd = open(filepath, O_RDONLY);
	if (fd < 0)
	{
		perror("heredoc temp file");
		return (STATUS_ERR);
	}
	dup2(fd, STDIN_FILENO);
	if (close(fd) < 0)
	{
		perror("Close");
		return (STATUS_ERR);
	}
	return ((STATUS_OK));
}
