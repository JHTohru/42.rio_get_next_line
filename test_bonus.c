#include "get_next_line_bonus.h"
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main()
{
    int     fd1;
    int     fd2;
    char    *ln1;
    char    *ln2;

    fd1 = open("alpha.txt", O_RDONLY);
    if (fd1 < 0)
        return (1);
    fd2 = open("bravo.txt", O_RDONLY);
    if (fd2 < 0)
        return (1);
    while (1)
    {
        ln1 = get_next_line(fd1);
        if (ln1 != NULL)
            printf("%s", ln1);
        ln2 = get_next_line(fd2);
        if (ln2 != NULL)
            printf("%s", ln2);
        if (ln1 == NULL && ln2 == NULL)
            break ;
    }
    // todo: close files
    return (0);
}
