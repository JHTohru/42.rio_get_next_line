/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmenezes <jmenezes@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 18:16:42 by jmenezes          #+#    #+#             */
/*   Updated: 2022/09/15 00:33:58 by jmenezes         ###   ########.fr       */
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

// get_buffer() looks for a buffer entry in the linked list lst and returns it.
// If no entry is found, get_buffer() creates a new one.
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
		curr->next = NULL;
		if (prev == NULL)
			*lst = curr;
		else
			prev->next = curr;
	}
	return (curr->buffer);
}

// delete_buffer() frees and removes an entry with the given fd from the linked
// list lst.
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

// write_buffer() fills buffer with the contents of an open file and terminates
// it with a NUL character. write_buffer() frees *str and set it to NULL if any
// read error occurs.
static ssize_t	write_buffer(int fd, char **str, char *buffer)
{
	ssize_t	rcnt;

	rcnt = read(fd, buffer, BUFFER_SIZE);
	if (rcnt >= 0)
		buffer[rcnt] = '\0';
	else if (rcnt < 0)
	{
		free(str);
		*str = NULL;
	}
	return (rcnt);
}

// read_buffer() appends a line from buffer to *str, which can be NULL. If no
// line feed character is found in the buffer, all the buffer contents are moved
// to *str.
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

// get_next_line() returns the next line from an open file for the given fd.
// It returns NULL if there is no more bytes to be read or any error occurs.
// get_next_line() can handle alternated reads to different files.
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
