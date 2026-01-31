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

#include "minishell.h"

static void	set_stdin_redir(t_redirection redirection);
static void	set_stdout_redir(t_redirection redirection);
static void	set_append_redir(t_redirection redirection);
static void	redir_error_message(void);

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
			if (set_heredoc_redir(cmd, cmd->redirections[i].target) < 0)
				return (-1);
		}
		return (0);
	}
}

static void	set_append_redir(t_redirection redirection)
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

static void	set_stdout_redir(t_redirection redirection)
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

static void	redir_error_message(void)
{
	// incomplete!
	printf("Code redir_error_message() function!\n");
	return;
}
