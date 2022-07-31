/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmenezes <jmenezes@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 18:16:42 by jmenezes          #+#    #+#             */
/*   Updated: 2022/07/31 02:57:00 by jmenezes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdlib.h>
#include <unistd.h>

void	*ft_memmove(void *dst, const void *src, size_t n);
char	*ft_strchr(const char *str, int c);
char	*ft_strjoin(const char *str1, const char *str2);
size_t	ft_strlcpy(char *dst, const char *src, size_t size);
size_t	ft_strlen(const char *str);

static ssize_t	write_buffer(int fd, char **str, char *buffer)
{
	ssize_t	rcnt;

	rcnt = read(fd, buffer, BUFFER_SIZE);
	if (rcnt >= 0)
		buffer[rcnt] = '\0';
	else if (rcnt == -1)
	{
		free(str);
		*str = NULL;
	}
	return (rcnt);
}

static void	read_buffer(char **str, char *buffer)
{
	char	*line;
	size_t	lnlen;
	char	*tmp;

	tmp = ft_strchr(buffer, '\n');
	if (tmp != NULL)
		lnlen = tmp - buffer + 1;
	else
		lnlen = ft_strlen(buffer);
	tmp = *str;
	line = malloc(lnlen + 1);
	if (line != NULL)
	{
		ft_strlcpy(line, buffer, lnlen + 1);
		*str = line;
		if (tmp != NULL)
		{
			*str = ft_strjoin(tmp, line);
			free(line);
		}
		if (*str != NULL)
			ft_memmove(buffer, buffer + lnlen, ft_strlen(buffer) - lnlen + 1);
	}
	free(tmp);
}

char	*get_next_line(int fd)
{
	static char	buffer[BUFFER_SIZE + 1];
	char		*str;

	str = NULL;
	while (1)
	{
		if (ft_strlen(buffer) == 0 && write_buffer(fd, &str, buffer) <= 0)
		{
			buffer[0] = '\0';
			break ;
		}
		read_buffer(&str, buffer);
		if (str == NULL || ft_strchr(str, '\n') != NULL)
			break ;
	}
	return (str);
}
