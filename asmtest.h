#ifndef ASMTEST_H
# define ASMTEST_H

# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <stdio.h>
# include <stdint.h>
# include <unistd.h>
# include <errno.h>
# include <string.h>
# include <stdlib.h>
# include <limits.h>
# include <time.h>

# define  TEST(x) \
	if (!x){\
		if (!g_koflag++) {printf("\x1b[31mKO\n\n\x1b[39m");}\
		printf(" line %3d : %s\n", __LINE__, #x);}

typedef struct s_list t_list;
struct	s_list
{
	void	*data;
	t_list	*next;
};

size_t	test(char *s);
ssize_t	ft_write(int fd, const void *s, size_t size);
ssize_t	ft_read(int fd, void *buf, size_t size);
char	*ft_strdup(const char *s);
char	*ft_strcpy(char *dest, const char *src);
size_t	ft_strlen(const char *s);
int		ft_strcmp(char *s1, char *s2);
void	puts_res(char *s, void (*func_test)());
int		len(char *s);
int		cmp(char *s1, char *s2);
int		cpy(char *dest, char *src);
int		wr(char *s);
int		wr_test(char *s, int ft, int lib);
int		rd(int fd, char *ft, char *lib);
int		rd_test(char *s);
int 	rdup(char *s);
int		atoib(char *s, char *base, int res);
int		lstsize(t_list *lst, int res);
void	strlen_test();
void	strcpy_test();
void	strcmp_test();
void	write_test();
void	read_test();
void	strdup_test();
int		ft_isspace(int c);
int		ft_atoi_base(char *str, char *base);
void	ft_list_sort(t_list **begin_list, int (*cmp)());
void	ft_list_remove_if(t_list **lst, void *data_ref, int (*cmp)(), void (*free_fct)(void *));
int		ft_list_size(t_list *lst);
void	ft_list_push_front(t_list **lst, void *data);
#endif