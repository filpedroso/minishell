/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirections_3.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 16:47:47 by fpedroso          #+#    #+#             */
/*   Updated: 2025/12/29 16:47:47 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*create_temp_filepath();
static void	read_input_into_file(int fd, char *delim);

char	*create_temp_file(char *delim)
{
	char	*filepath;
	int		fd;

	filepath = create_temp_filepath();
	if (!filepath)
		return (NULL);
	fd = open(filepath, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (fd < 0)
	{
		free(filepath);
		return (NULL);
	}
	read_input_into_file(fd, delim);
	close(fd);
	return (filepath);
}

static char	*create_temp_filepath()
{
	static int	counter = 0;
	char		*num_str;
	char		*new_filepath;

	num_str = ft_itoa(counter++);
	new_filepath = ft_strjoin("/tmp/heredoc_", num_str);
	free(num_str);
	return (new_filepath);
}

static void	read_input_into_file(int fd, char *delim)
{
	char	*line;

	while (1)
	{
		line = readline(">> ");
		if (!line || ft_strcmp(line, delim) == 0)
		{
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
}
