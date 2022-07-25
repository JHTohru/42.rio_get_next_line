/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmenezes <jmenezes@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 18:16:42 by jmenezes          #+#    #+#             */
/*   Updated: 2022/07/25 18:17:20 by jmenezes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdlib.h>
#include <unistd.h>

char	*ft_strchr(const char *str, int c)
{
	while (1)
	{
		if ((unsigned char)*str == (unsigned char)c)
			return ((char *)str);
		if (*str == '\0')
			return (NULL);
		str++;
	}
}

char	*strnjoin(const char *str1, const char *str2, size_t str2len)
{
	char	*jstr;
	size_t	str1len;
	size_t	i;

	str1len = 0;
	if (str1 != NULL)
		while (str1[str1len] != '\0')
			str1len++;
	jstr = malloc(str1len + str2len + 1);
	if (jstr == NULL)
		return (NULL);
	i = 0;
	while (i < str1len)
	{
		jstr[i] = str1[i];
		i++;
	}
	i = 0;
	while (i < str2len)
	{
		jstr[i + str1len] = str2[i];
		i++;
	}
	jstr[str1len + str2len] = '\0';
	return (jstr);
}

size_t	get_line(char **str, const char *buffer, size_t buflen)
{
	char	*lf;
	char	*tmp;
	size_t	n;

	lf = ft_strchr(buffer, '\n');
	if (lf == NULL)
		n = buflen;
	else
		n = lf - buffer + 1;
	tmp = strnjoin(*str, buffer, n);
	free(*str);
	*str = tmp;
	return (n);
}

char	*get_next_line(int fd)
{
	static char		buffer[BUFFER_SIZE];
	static ssize_t	buflen;
	static size_t	bufidx;
	char			*str;

	str = NULL;
	while (1)
	{
		if (bufidx == (size_t)buflen)
		{
			bufidx = 0;
			buflen = read(fd, buffer, sizeof(buffer));
			if (buflen == 0)
				break ;
			if (buflen == -1)
			{
				free(str);
				return (NULL);
			}
		}
		bufidx += get_line(&str, buffer + bufidx, (size_t)buflen - bufidx);
		if (str == NULL || ft_strchr(str, '\n') != NULL)
			break ;
	}
	return (str);
}
