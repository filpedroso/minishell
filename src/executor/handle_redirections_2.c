/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirections_2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 18:39:46 by fpedroso          #+#    #+#             */
/*   Updated: 2025/12/16 18:39:46 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static void	dup_file_into_stdin(char *heredoc_filepath);
static char	*create_temp_file(char *delim);
static void	append_filepath(char *filepath, t_temp_files *temp_files_list);

void	set_heredoc_redir(t_command *cmd, char *heredoc_delim)
{
	char	*heredoc_filepath;
	int		heredoc_fd;

	heredoc_filepath = create_temp_file(heredoc_delim);
	if (!heredoc_filepath)
	{
		perror("heredoc reader");
		return ;
	}
	dup_file_into_stdin(heredoc_filepath);
	append_filepath(heredoc_filepath, cmd->temp_files_list);
}

static void	dup_file_into_stdin(char *heredoc_filepath)
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

static char	*create_temp_file(char *delim)
{
	// incomplete!
	printf("Code create_temp_file() function!\n");
	return;
}

static void	append_filepath(char *filepath, t_temp_files *temp_files_list)
{
	// incomplete!
	// append filepath to 
	printf("Code create_temp_file() function!\n");
	return;
}
