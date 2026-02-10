/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_handle_redirections_1.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 18:33:42 by fpedroso          #+#    #+#             */
/*   Updated: 2026/02/10 00:34:52 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	set_stdin_redir(t_redirection redirection);
static void	set_stdout_redir(t_redirection redirection);
static void	set_append_redir(t_redirection redirection);

int	handle_redirections(t_command *cmd)
{
	t_redirection_type	redir_type;
	int					i;

	i = -1;
	while (++i < cmd->redirections_count)
	{
		redir_type = cmd->redirections[i].type;
		if (redir_type == REDIR_IN)
			set_stdin_redir(cmd->redirections[i]);
		else if (redir_type == REDIR_OUT)
			set_stdout_redir(cmd->redirections[i]);
		else if (redir_type == REDIR_APPEND)
			set_append_redir(cmd->redirections[i]);
		else if (redir_type == REDIR_HEREDOC)
		{
			if (set_heredoc_redir(cmd,
					cmd->redirections[i].target.token_word_ptr) < 0)
				return (-1);
		}
	}
	return (0);
}

static void	set_append_redir(t_redirection redirection)
{
	int	fd;

	fd = open(redirection.target.token_word_ptr, O_WRONLY | O_CREAT | O_APPEND,
			0644);
	if (fd < 0)
	{
		perror("Open append redir file");
		return ;
	}
	dup2(fd, STDOUT_FILENO);
	if (close(fd) < 0)
	{
		perror("Close");
		return ;
	}
}

static void	set_stdout_redir(t_redirection redirection)
{
	int	fd;

	fd = open(redirection.target.token_word_ptr, O_WRONLY | O_CREAT | O_TRUNC,
			0644);
	if (fd < 0)
	{
		perror("Open stdout redir file");
		return ;
	}
	dup2(fd, STDOUT_FILENO);
	if (close(fd) < 0)
	{
		perror("Close");
		return ;
	}
}

static void	set_stdin_redir(t_redirection redirection)
{
	int	fd;

	fd = open(redirection.target.token_word_ptr, O_RDONLY);
	if (fd < 0)
	{
		perror("Open stdin redir file");
		return ;
	}
	dup2(fd, STDIN_FILENO);
	if (close(fd) < 0)
	{
		perror("Close");
		return ;
	}
}
