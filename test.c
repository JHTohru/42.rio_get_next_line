#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main()
{
    int     fd;
    char    *line;

    fd = open("foo.txt", O_RDONLY);
    if(fd < 0)
        return 1;
    while (1)
    {
        line = get_next_line(fd);
        if (line == NULL)
            break ;
        write(1, line, strlen(line));
    }
}
