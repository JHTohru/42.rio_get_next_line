#include "get_next_line.h"
#include <dlfcn.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void	*(*system_malloc)(size_t) = NULL;
void	(*system_free)(void *) = NULL;
int		alloc_cnt = 0;
int		free_cnt = 0;

void	*malloc(size_t size)
{
	alloc_cnt++;
	return system_malloc(size);
}

void    free(void *s)
{
	free_cnt++;
	system_free(s);
}

typedef struct s_node
{
	void			*content;
	struct s_node	*next;
}	t_node;

t_node *node_new(void *content)
{
	t_node	*node;

	node = system_malloc(sizeof(t_node));
	if (node != NULL)
	{
		node->content = content;
		node->next = NULL;
	}
	return (node);
}

t_node	*node_previous(t_node *node)
{
	t_node	*first;

	if (node != NULL)
	{
		first = node;
		node = node->next;
		while (node->next != first)
			node = node->next;
	}
	return (node);
}

void	node_insert_before(t_node **lst, t_node *node)
{
	if (*lst == NULL)
		*lst = node;
	else
		node_previous(*lst)->next = node;
	node->next = *lst;
}

void	node_delete(t_node **node, void (*del)(void *))
{
	if (del != NULL)
		del((*node)->content);
	system_free(*node);
	*node = NULL;
}

void	node_remove(t_node **node, void (*del)(void *))
{
	t_node	*prev;

	if (*node == NULL)
		return ;
	prev = node_previous(*node);
	if (prev == *node)
		prev = NULL;
	else
		prev->next = (*node)->next;
	node_delete(node, del);
	*node = prev;
}

void	node_delete_all(t_node **lst, void (*del)(void *))
{
	t_node	*node;

	while (*lst != NULL)
	{
		node = (*lst)->next;
		(*lst)->next = node->next;
		del(node);
		if (*lst == node)
			*lst = NULL;
	}
}

t_node	*node_new_int(int n)
{
	int	*content;

	content = system_malloc(sizeof(int));
	if (content == NULL)
		return (NULL);
	*content = n;
	return (node_new((void *)content));
}

t_node	*lst_add_int(t_node **lst, int n)
{
	t_node	*node;

	node = node_new_int(n);
	if (node != NULL)
		node_insert_before(lst, node);
	return (node);
}

void	lst_remove_node(t_node **lst)
{
	node_remove(lst, system_free);
}

void	lst_delete(t_node **lst)
{
	node_delete_all(lst, system_free);
}

int	node_get_int(t_node *node)
{
	return (*((int *)node->content));
}

void	ft_putescapedstr(char *str)
{
	size_t	i;

	i = 0;
	while (str[i] != '\0')
	{
		switch (str[i]) {
			case '\a':
				printf("\\a");
				break ;
			case '\b':
				printf("\\b");
				break ;
			case '\f':
				printf("\\f");
				break ;
			case '\n':
				printf("\\n");
				break ;
			case '\r':
				printf("\\r");
				break ;
			case '\t':
				printf("\\t");
				break ;
			case '\v':
				printf("\\v");
				break ;
			case '"':
				printf("\\\"");
				break ;
			default:
				printf("%c", str[i]);
		}
		i++;
	}
}

int	main(int argc, char *argv[])
{
	char	*ln_got;
	char	ln_want[1028];
	int		failcnt;
	int		fd;
	int		i;
	t_node	*fdslst;
	FILE	*fl_want;

	system_malloc = dlsym(RTLD_NEXT, "malloc");
	system_free = dlsym(RTLD_NEXT, "free");

	if (argc < 3)
	{
		fprintf(stderr, "Error\nInvalid number of arguments.\n");
		return (1);
	}
	fl_want = fopen(argv[1], "r");
	if (fl_want == NULL)
	{
		fprintf(stderr, "Error\nCould not open the file \"%s\".\n", argv[1]);
		return (1);
	}
	fdslst = NULL;
	i = 2;
	while (i < argc)
	{
		fd = open(argv[i], O_RDONLY);
		if (fd < 0)
		{
			fprintf(stderr, "Error\nCould not open the file \"%s\".\n", argv[i]);
			lst_delete(&fdslst);
			return (1);
		}
		if (lst_add_int(&fdslst, fd) == NULL)
		{
			fprintf(stderr, "Error\nCould not allocate memory for the fd node.\n");
			lst_delete(&fdslst);
			return (1);
		}
		i++;
	}
	failcnt = 0;
	while (1)
	{
		fd = node_get_int(fdslst);
		ln_got = get_next_line(fd);
		if (ln_got != NULL)
		{
			fgets(ln_want, (int)sizeof(ln_want) - 1, fl_want);
			if (strcmp(ln_want, ln_got) != 0)
			{
				if (failcnt > 0)
					printf("\n");
				printf("Unexpected result:\nBUFFER_SIZE: %d\nWant: \"", BUFFER_SIZE);
				ft_putescapedstr(ln_want);
				printf("\"\nGot : \"");
				ft_putescapedstr(ln_got);
				printf("\"\n");
				failcnt++;
			}
			system_free(ln_got);
		}
		else
		{
			close(fd);
			lst_remove_node(&fdslst);
		}
		if (fdslst == NULL)
			break ;
		fdslst = fdslst->next;
	}
	if (alloc_cnt != free_cnt)
	{
		if (failcnt > 0)
			printf("\n");
		printf("\nMemory leak:\nBUFFER_SIZE: %d\nmalloc calls: %d\nfree calls  : %d\n",
			BUFFER_SIZE, alloc_cnt, free_cnt);
	}
	return (0);
}
