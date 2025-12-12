/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirections.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 18:33:42 by fpedroso          #+#    #+#             */
/*   Updated: 2025/12/02 18:33:42 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

void	handle_redirections(t_command cmd)
{
	t_redirection_type	redir_type;
	int					i;

	i = -1;
	while (++i < cmd.redirections_count)
	{
		redir_type = cmd.redirections[i].type;
		if (redir_type == IN)
			set_stdin_redir(cmd.redirections[i]);
		else if (redir_type == OUT)
			set_stdout_redir(cmd.redirections[i]);
		else if (redir_type == APPEND)
			set_append_redir(cmd.redirections[i]);
		else if (redir_type == HEREDOC)
			set_heredoc_redir(cmd, cmd.redirections[i].target);
		else
		{
			return (redir_error_message());
		}
	}
}

// needs to signal that the temp file must be deleted
void	set_heredoc_redir(t_command cmd, char *heredoc_delim)
{
	char	*heredoc_filepath;
	int		heredoc_fd;

	heredoc_filepath = create_temp_file(redirection.target);
	if (!heredoc_filepath)
	{
		perror("heredoc reader");
		return ;
	}
	dup_file_into_stdin(heredoc_filepath);
	append_filepath(heredoc_filepath, cmd);
}
void	dup_file_into_stdin(char *heredoc_filepath)
{
	int	heredoc_fd;
	
	heredoc_fd = open(heredoc_filepath, O_RDONLY);
	if (heredoc_fd < 0)
	{
		perror("heredoc temp file");
		return ;
	}
	dup2(heredoc_fd, STDIN_FILENO);
	if (close(heredoc_fd) < 0)
	{
		perror("heredoc temp file close");
		return ;
	}
}

void	set_append_redir(t_redirection redirection)
{
	int	fd;

	fd = open(redirection.target, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		perror(redirection.target);
		return ;
	}
	dup2(fd, STDOUT_FILENO);
	if (close(fd) < 0)
	{
		perror(redirection.target);
		return ;
	}
}

void	set_stdout_redir(t_redirection redirection)
{
	int	fd;

	fd = open(redirection.target, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror(redirection.target);
		return ;
	}
	dup2(fd, STDOUT_FILENO);
	if (close(fd) < 0)
	{
		perror(redirection.target);
		return ;
	}
}

static void	set_stdin_redir(t_redirection redirection)
{
	int	fd;

	fd = open(redirection.target, O_RDONLY);
	if (fd < 0)
	{
        perror(redirection.target);
        return;
	}
	dup2(fd, STDIN_FILENO);
	if (close(fd) < 0)
	{
		perror(redirection.target);
		return ;
	}
}
