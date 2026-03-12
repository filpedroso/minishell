/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 19:04:48 by fpedroso          #+#    #+#             */
/*   Updated: 2026/03/11 19:04:48 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_strlen_nl(const char *s);
static char		*linefill(char *old, char *buuf);
static void		set_buf(char *buuf, size_t buf_len);


char	*get_next_line(int fd)
{
	static char	buuf[FOPEN_MAX][BUFFER_SIZE + 1];
	char		*line;
	ssize_t		rbytes;

	line = NULL;
	rbytes = 1;
	while ((fd >= 0 && fd < FOPEN_MAX && BUFFER_SIZE > 0) && rbytes > 0)
	{
		if (buuf[fd][0])
		{
			line = linefill(line, buuf[fd]);
			if (!line)
				return (NULL);
			if (line[ft_strlen_nl(line) - 1] == '\n')
				break ;
		}
		else
		{
			rbytes = read(fd, buuf[fd], BUFFER_SIZE);
			if (rbytes < 0)
				return (free(line), NULL);
			buuf[fd][rbytes] = '\0';
		}
	}
	return (line);
}

static size_t	ft_strlen_nl(const char *s)
{
	const char	*ptr;

	if (!s)
		return (0);
	ptr = s;
	while (*ptr && *ptr != '\n')
		ptr++;
	if (*ptr == '\n')
		ptr++;
	return ((size_t)(ptr - s));
}

static char	*linefill(char *old, char *buuf)
{
	char		*line;
	size_t		old_len;
	size_t		buf_len;
	size_t		i;

	old_len = ft_strlen_nl(old);
	buf_len = ft_strlen_nl(buuf);
	line = (char *)malloc((old_len + buf_len + 1) * sizeof(char));
	if (!line)
		return (free(line), NULL);
	line[old_len + buf_len] = '\0';
	i = -1;
	while (++i < old_len)
		line[i] = old[i];
	free(old);
	i = -1;
	while (++i < buf_len)
		line[i + old_len] = buuf[i];
	set_buf(buuf, buf_len);
	return (line);
}

static void	set_buf(char *buuf, size_t buf_len)
{
	int	i;

	i = 0;
	while (buuf[i + buf_len])
	{
		buuf[i] = buuf[i + buf_len];
		i++;
	}
	buuf[i] = '\0';
}
