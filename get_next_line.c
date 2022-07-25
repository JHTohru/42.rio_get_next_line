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

char	*get_next_line(int fd)
{
	static char		buffer[BUFFER_SIZE];
	static ssize_t	buflen;
	static size_t	bufidx;
	char			*str;
	size_t			strlen;
	char			*lf;
	char			*tmp;
	size_t			n;
	size_t			i;

	str = NULL;
	strlen = 0;
	while (1)
	{
		if (bufidx == (size_t)buflen)
		{
			bufidx = 0;
			buflen = read(fd, buffer, BUFFER_SIZE);
			if (buflen == 0)
				break ;
			if (buflen == -1)
				goto error;
		}
		lf = ft_strchr(buffer + bufidx, '\n');
		if (lf == NULL)
			n = buflen - bufidx;
		else
			n = (lf - buffer) - bufidx + 1;
		tmp = malloc(strlen + n + 1);
		if (tmp == NULL)
			goto error;
		i = 0;
		while (i < strlen)
		{
			tmp[i] = str[i];
			i++;
		}
		i = 0;
		while (i < n)
		{
			tmp[i + strlen] = buffer[i + bufidx];
			i++;
		}
		tmp[i + strlen] = '\0';
		strlen += n;
		bufidx += n;
		free(str);
		str = tmp;
		if (lf != NULL)
			break ;
	}
	return (str);

error:
	free(str);
	return (NULL);
}
