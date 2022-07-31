/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmenezes <jmenezes@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/30 11:11:13 by jmenezes          #+#    #+#             */
/*   Updated: 2022/07/30 11:20:24 by jmenezes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i] != '\0')
		i++;
	return (i);
}

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	i;

	i = 0;
	if (size > 0)
	{
		while (src[i] != '\0' && i < size - 1)
		{
			dst[i] = src[i];
			i++;
		}
		dst[i] = '\0';
	}
	while (src[i] != '\0')
		i++;
	return (i);
}

char	*ft_strjoin(const char *str1, const char *str2)
{
	char	*jstr;
	size_t	size;

	size = ft_strlen(str1) + ft_strlen(str2) + 1;
	jstr = (char *)malloc(size);
	if (jstr != NULL)
		ft_strlcpy(jstr + ft_strlcpy(jstr, str1, size), str2, size);
	return (jstr);
}

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

void	*ft_memmove(void *dst, const void *src, size_t n)
{
	size_t	i;

	if (dst != src)
	{
		if (dst < src)
		{
			i = 0;
			while (i < n)
			{
				((char *)dst)[i] = ((char *)src)[i];
				i++;
			}
		}
		else
			while (n-- > 0)
				((char *)dst)[n] = ((char *)src)[n];
	}
	return (dst);
}
