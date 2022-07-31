/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmenezes <jmenezes@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 18:16:42 by jmenezes          #+#    #+#             */
/*   Updated: 2022/07/31 02:56:43 by jmenezes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"
#include <stdlib.h>
#include <unistd.h>

void	*ft_memmove(void *dst, const void *src, size_t n);
char	*ft_strchr(const char *str, int c);
char	*ft_strjoin(const char *str1, const char *str2);
size_t	ft_strlcpy(char *dst, const char *src, size_t size);
size_t	ft_strlen(const char *str);

static char	*get_buffer(t_buffer_list **lst, int fd)
{
	t_buffer_list	*prev;
	t_buffer_list	*curr;

	prev = NULL;
	curr = *lst;
	while (curr != NULL && curr->fd != fd)
	{
		prev = curr;
		curr = curr->next;
	}
	if (curr == NULL)
	{
		curr = malloc(sizeof(t_buffer_list));
		if (curr == NULL)
			return (NULL);
		curr->fd = fd;
		curr->buffer = malloc(BUFFER_SIZE + 1);
		curr->buffer[0] = '\0';
		if (prev == NULL)
			*lst = curr;
		else
			prev->next = curr;
	}
	return (curr->buffer);
}

static void	delete_buffer(t_buffer_list **lst, int fd)
{
	t_buffer_list	*prev;
	t_buffer_list	*curr;

	prev = NULL;
	curr = *lst;
	while (curr != NULL && curr->fd != fd)
	{
		prev = curr;
		curr = curr->next;
	}
	if (curr != NULL)
	{
		if (prev != NULL)
			prev->next = curr->next;
		if (curr == *lst)
			*lst = curr->next;
		free(curr->buffer);
		free(curr);
	}
}

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
	static t_buffer_list	*lst;
	char					*buffer;
	char					*str;

	str = NULL;
	buffer = get_buffer(&lst, fd);
	if (buffer != NULL)
	{
		while (1)
		{
			if (ft_strlen(buffer) == 0 && write_buffer(fd, &str, buffer) <= 0)
			{
				delete_buffer(&lst, fd);
				break ;
			}
			read_buffer(&str, buffer);
			if (str == NULL || ft_strchr(str, '\n') != NULL)
				break ;
		}
	}
	return (str);
}
