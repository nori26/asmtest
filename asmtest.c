#include "asmtest.h"

int g_koflag;

t_list	*ft_lstnew(void *content)
{
	t_list *lst;

	if (!(lst = malloc(sizeof(t_list))))
		return (NULL);
	lst->data = content;
	lst->next = NULL;
	return (lst);
}

void	ft_lstadd_front(t_list **lst, t_list *new)
{
	if (!lst)
		return ;
	if (new)
		new->next = *lst;
	*lst = new;
}

void delone(void *lst)
{
	free(((t_list *)lst)->data);
	free(lst);
}

t_list  *free_set(t_list *lst, t_list *next)
{
	delone(lst);
	return (next);
}

void 	free_lst(t_list *lst)
{
	while (lst)
		lst = free_set(lst, lst->next);
}

t_list *make_list(char *s)
{
	t_list	*lst;
	char	*data;

	lst = NULL;
	while(*s)
	{
		data = strdup((char []){*s++, '\0'});
		ft_lstadd_front(&lst, ft_lstnew(data));
	}
	return (lst);
}

char *rand_str()
{
	int		i = 0;
	int		len;
	char	*s;
	struct timespec ts;

	timespec_get(&ts, TIME_UTC);
	srand((unsigned int)ts.tv_nsec);
	len = rand() % 500;
	s = malloc(1000);
	i = 0;
	while (i < len)
		s[i++] = rand() % 126 + 1;
	s[i] = 0;
	return (s);
}

void rand_test(int (*func)(), int (*cmp_f)())
{
	int		i = 0;

	while (i++ < 10000)
	{
		if (!func(cmp_f))
		{
			if (!g_koflag++)
				printf("\x1b[31mKO\n\n\x1b[39m");
			printf(" line %3d : random test KO\n", __LINE__);
			break ;
		}
	}
}

int  rand_1str(int (*cmp_f)())
{
	char *s;
	int  ret;

	s = rand_str();
	ret = cmp_f(s);
	free(s);
	return (ret);
}

int  rand_2str(int (*cmp_f)())
{
	char *s1;
	char *s2;
	int  ret;

	s1 = rand_str();
	s2 = rand_str();
	ret = cmp_f(s1, s2);
	free(s1);
	free(s2);
	return (ret);
}

int  rand_size(int (*cmp_f)())
{
	char *s;
	int  ret;
	t_list *lst;

	s = rand_str();
	lst = make_list(s);
	ret = cmp_f(lst, strlen(s));
	free(s);
	return (ret);
}

int  rand_push(int (*cmp_f)())
{
	char *s;
	int  ret;
	t_list *lst;

	s = rand_str();
	lst = make_list(s);
	ret = cmp_f(lst, s);
	free(s);
	return (ret);
}

int char_cmp(const void *c1, const void *c2)
{
	return (*(const char *)c1 - *(const char *)c2);
}

int rand_sort(int (*cmp_f)())
{
	char *s;
	int ret;
	t_list *lst;

	s = rand_str();
	lst = make_list(s);
	qsort(s, strlen(s), 1, char_cmp);
	ret = cmp_f(lst, s);
	free(s);
	return (ret);
}

void lst_to_str(t_list *lst, char *res)
{
	int i = 0;
	while (lst)
	{
		// printf("%c\n", *(char *)lst->data);
		*res++ = *(char *)lst->data;
		lst = lst->next;
	}
	*res = 0;
}

int lst_cmp(char *c1, char *c2)
{
	// char c;
	// char d;
	// c = *c1;
	// d = *c2;
	// printf("%c %c\n", c, d);
	return (*c1 - *c2);
}

#ifdef FT_LEN
int len(char *s)
{
	return (ft_strlen(s) == strlen(s));
}
#endif

#ifdef FT_CPY
int cpy(char *dest, char *src)
{
	char s[1000] = {};

	strcpy(s, src);
	return (ft_strcpy(dest, src) == dest && !strcmp(dest, s));
}
#endif

#ifdef FT_CMP
int cmp(char *s1, char *s2)
{
	#ifndef SANITIZE
	return (ft_strcmp(s1, s2) == strcmp(s1, s2));
	#else
	int	ft;
	int	lib;

	ft = ft_strcmp(s1, s2);
	lib = strcmp(s1, s2);
	return (!((ft ^ lib) >> (sizeof(int) * CHAR_BIT - 1) || (--ft ^ --lib) >> (sizeof(int) * CHAR_BIT - 1)));
	#endif
}
#endif

#ifdef FT_WR
int wr_test(char *s, int ft, int lib)
{
	int				err1;
	int				err2;
	ssize_t 		ret1;
	ssize_t 		ret2;
	static size_t	len;

	if (s)
		len = strlen(s);
	errno = 0;
	ret1 = ft_write(ft, s, len);
	err1 = errno;
	ret2 = write(lib, s, len);
	err2 = errno;
	if (!s)
		len = 1;
	// printf("%zd\n%zd\n", ret1, ret2);
	return (!system("diff res_ft.txt res_lib.txt") && ret1 == ret2 && err1 == err2);
}

int wr(char *s)
{
	int ft;
	int lib;
	int ret;

	ft = open("res_ft.txt",
	O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU | S_IRWXG | S_IRWXO);
	lib = open("res_lib.txt",
	O_WRONLY | O_CREAT| O_TRUNC, S_IRWXU | S_IRWXG | S_IRWXO);
	ret = wr_test(s, ft, lib) && wr_test(s, -1, -1);
	close(ft);
	close(lib);
	return (ret);
}
#endif

#ifdef FT_RD
int rd(int fd, char *ft, char *lib)
{
	int		err1;
	int		err2;
	ssize_t	lret;
	ssize_t	fret;

	errno = 0;
	fret = ft_read(fd, ft, 100000);
	err1 = errno;
	lseek(fd, 0, 0);
	lret = read(fd, lib, 100000);
	err2 = errno;
	lseek(fd, 0, 0);
	// printf("%zd\n%zd\n", fret, lret);
	return ((ft ? !strcmp(ft, lib) : 1) && fret == lret && err1 == err2);
}

int rd_test(char *s)
{
	int		fd;
	int		ret;
	char	ft[100001] = {};
	char	lib[100001] = {};

	fd = open(s, O_RDONLY);
	ret = rd(fd, ft, lib) && rd(fd, NULL, NULL);
	close(fd);
	return (ret);
}
#endif

#ifdef FT_DUP
int  rdup(char *s)
{
	char *ft;
	char *lib;
	int  ret;

	ft = ft_strdup(s);
	lib = strdup(s);
	ret = !strcmp(ft, lib);
	free(ft);
	free(lib);
	return (ret);
}
#endif

#ifdef FT_ATOI
int  atoib(char *s, char *base, int res)
{
	// printf("res : %d\n", ft_atoi_base(s, base));
	return (res == ft_atoi_base(s, base));
}
#endif

#ifdef FT_SIZE
int lstsize(t_list *lst, int res)
{
	int ret;

	ret = ft_list_size(lst) == res;
	free_lst(lst);
	return (ret);
}
#endif

#ifdef FT_PUSH
int push(t_list *lst, char *s)
{
	int  ret;
	int  i = 0;
	size_t len;
	char *c = rand_str();
	char res[1000] = {};

	ft_list_push_front(&lst, strdup((char []){*c, '\0'}));
	lst_to_str(lst, res);
	i = 0;
	len = strlen(s);
	while (len)
	{
		if (s[len - 1] != res[++i])
			break ;
		len--;
	}
	ret = res[0] == *c && !len && i == strlen(s);
	free(c);
	free_lst(lst);
	return (ret);
}
#endif

#ifdef FT_SORT
int sort(t_list *lst, char *s)
{
	char res[1000] = {};

	ft_list_sort(&lst, strcmp);
	lst_to_str(lst, res);
	free_lst(lst);
	return (!strcmp(res, s));
}
#endif

#ifdef FT_REMOVE
int remif(t_list *lst, char *del, char *res)
{
	char buf[1000] = {};

	ft_list_remove_if(&lst, del, strcmp, delone);
	lst_to_str(lst, buf);
	free_lst(lst);
	// printf("%s\n", buf);
	return (!strcmp(buf, res));
}
#endif

void strlen_test()
{
	#ifdef FT_LEN
	TEST(len(""));
	TEST(len("\n"));
	TEST(len("0123456789"));
	TEST(len("hoge"));
	TEST(len("0123456789abcde"));
	rand_test(rand_1str, len);
	#else
	g_koflag = -1;
	#endif
}

void strcpy_test()
{
	#ifdef FT_CPY
	char dest[1000] = "aiueo1234567890";

	TEST(cpy(dest, dest));
	TEST(cpy(dest, ""));
	TEST(cpy(dest, "1234"));
	TEST(cpy(dest, "\n"));
	TEST(cpy(dest, "adsfahr"));
	TEST(cpy(dest, " "));
	TEST(cpy(dest, "afdssssssssspiyghuio2v-9q20q4@8h"));
	TEST(cpy(dest, "\0aaaaa"));
	TEST(cpy(dest, "aaa\0aaasdfas"));
	rand_test(rand_2str, cpy);
	#else
	g_koflag = -1;
	#endif
}

void strcmp_test()
{
	#ifdef FT_CMP
	char *s1;
	char *s2;
	char *str = "Foo Bar Hoge";

	s1 = "";
	s2 = s1;
	TEST(cmp(s1, s2));
	TEST(cmp(str + 4, "Bar"));
	TEST(cmp("\200\179", "ab"));
	TEST(cmp("aiueo","12345"));
	TEST(cmp("",""));
	TEST(cmp("","1"));
	TEST(cmp("1",""));
	TEST(cmp("1","2"));
	TEST(cmp("2","1"));
	TEST(cmp("1234","1234"));
	TEST(cmp("1234","12345"));
	TEST(cmp("12345","1234"));
	TEST(cmp("", "a"));
	TEST(cmp("a", ""));
	TEST(cmp("abc", "abc"));
	TEST(cmp("abc", "abcd"));
	TEST(cmp("abcd", "abc"));
	TEST(cmp("abcde", "abc"));
	TEST(cmp("abe", "abefrg"));
	TEST(cmp("abc", "def"));
	TEST(cmp("abc", "aef"));
	TEST(cmp("abc", "abf"));
	TEST(cmp("abc", "abfg"));
	rand_test(rand_2str, cmp);
	#else
	g_koflag = -1;
	#endif
}

void write_test()
{
	#ifdef FT_WR
	TEST(wr(""));
	TEST(wr("aiueo"));
	TEST(wr("asrhgasdh"));
	TEST(wr("1234546577587979871235324"));
	TEST(wr("\0\0\0\0\0\0\0\0\0\0\0\0"));
	TEST(wr(NULL));
	TEST(wr(NULL));
	remove("res_ft.txt");
	remove("res_lib.txt");
	#else
	g_koflag = -1;
	#endif
}

void read_test()
{
	#ifdef FT_RD
	TEST(rd_test("main.c"));
	TEST(rd_test("ft_strlen.s"));
	TEST(rd_test("ft_strcpy.s"));
	TEST(rd_test("ft_strcmp.s"));
	TEST(rd_test("ft_write.s"));
	TEST(rd_test("ft_read.s"));
	TEST(rd_test("ft_strdup.s"));
	TEST(rd_test(NULL));
	#else
	g_koflag = -1;
	#endif
}

void strdup_test()
{
	#ifdef FT_DUP
	TEST(rdup(""));
	TEST(rdup("\n"));
	TEST(rdup("0123456789"));
	TEST(rdup("hoge"));
	TEST(rdup("0123456789abcde"));
	TEST(rdup("1234"));
	TEST(rdup("adsfahr"));
	TEST(rdup(" "));
	TEST(rdup("afdssssssssspiyghuio2v-9q20q4@8h"));
	TEST(rdup("\0aaaaa"));
	TEST(rdup("aaa\0aaasdfas"));
	rand_test(rand_2str, rdup);
	#else
	g_koflag = -1;
	#endif
}

void atoi_base_test()
{
	#ifdef FT_ATOI
	int expect;
	TEST(atoib("1234", "0123456789", expect=1234));
	TEST(atoib("++1234", "0123456789", expect=1234));
	TEST(atoib("--1234", "0123456789", expect=1234));
	TEST(atoib("---1234", "0123456789", expect=-1234));
	TEST(atoib("---01234", "0123456789", expect=-1234));
	TEST(atoib(" \t--1234", "0123456789", expect=1234));
	TEST(atoib("-+-1234", "0123456789", expect=1234));
	TEST(atoib("-+- 1234", "0123456789", expect=0));
	TEST(atoib("-+-\t1234", "0123456789", expect=0));
	TEST(atoib("ff", "0123456789abcdef", expect=255));
	TEST(atoib("80", "0123456789abcdef", expect=128));
	TEST(atoib("", "", expect=0));
	TEST(atoib("123663", "", expect=0));
	TEST(atoib("", "1234", expect=0));
	TEST(atoib("11", "11", expect=0));
	TEST(atoib("11", "121", expect=0));
	TEST(atoib("11", "a2a", expect=0));
	TEST(atoib("22", "a2a", expect=0));
	TEST(atoib("1", "", expect=0));
	TEST(atoib("11", "01", expect=3));
	TEST(atoib("100", "01", expect=4));
	TEST(atoib("?!!", "!?", expect=4));
	TEST(atoib("+1", "+1", expect=0));
	TEST(atoib("+1", "-1", expect=0));
	TEST(atoib(" 1", " 1", expect=0));
	TEST(atoib("1", "1 ", expect=0));
	TEST(atoib("1", "1-", expect=0));
	TEST(atoib("1", "1+", expect=0));
	TEST(atoib("1", "\t1", expect=0));
	TEST(atoib("1", "\r1", expect=0));
	#else
	g_koflag = -1;
	#endif
}

void list_push_front_test()
{
	#ifdef FT_PUSH
	TEST(push(NULL, ""));
	TEST(push(make_list(""), ""));
	TEST(push(make_list("1"), "1"));
	rand_test(rand_push, push);
	#else
	g_koflag = -1;
	#endif
}

void list_size_test()
{
	#ifdef FT_SIZE
	int size;

	TEST(lstsize(make_list(""), size=0));
	TEST(lstsize(make_list("1"), size=1));
	TEST(lstsize(make_list("123"), size=3));
	TEST(lstsize(make_list("1234"), size=4));
	TEST(lstsize(make_list("1123"), size=4));
	TEST(lstsize(make_list("aiueo"), size=5));
	TEST(lstsize(make_list("01234567890123456789"), size=20));
	rand_test(rand_size, lstsize);
	#else
	g_koflag = -1;
	#endif
}

void list_sort_test()
{
	#ifdef FT_SORT
	TEST(sort(make_list("12345"), "12345"));
	TEST(sort(make_list("43152"), "12345"));
	TEST(sort(make_list("431520"), "012345"));
	TEST(sort(make_list("4315206"), "0123456"));
	TEST(sort(make_list("4213306"), "0123346"));
	TEST(sort(make_list("42133067"), "01233467"));
	TEST(sort(NULL, ""));
	rand_test(rand_sort, sort);
	#else
	g_koflag = -1;
	#endif
}

void list_remove_if_test()
{
	t_list *lst = make_list("a");
	#ifdef FT_REMOVE
	TEST(remif(make_list("a|i|u|e|o"), "|", "oeuia"));
	TEST(remif(make_list("||||"), "|", ""));
	TEST(remif(make_list("||||a"), "|", "a"));
	TEST(remif(make_list("a"), "|", "a"));
	TEST(remif(make_list("a"), "a", ""));
	TEST(remif(make_list("|aiueo"), "|", "oeuia"));
	TEST(remif(make_list("|aiueo"), "|", "oeuia"));
	TEST(remif(NULL, "|", ""));
	TEST(remif(NULL, "", ""));
	ft_list_remove_if(NULL, "",strcmp, delone);
	ft_list_remove_if(&lst, "", NULL, delone);
	ft_list_remove_if(&lst, "", strcmp, NULL);
	free_lst(lst);
	#else
	g_koflag = -1;
	#endif
}

void puts_res(char *name, void (*func_test)())
{
	g_koflag = 0;
	printf("%s", name);
	fflush(stdout);
	func_test();
	if (g_koflag > 0)
		puts("");
	else if (!g_koflag)
		puts("\x1b[32mOK\x1b[39m");
	else
		puts("\x1b[33mEMPTY\x1b[39m");
}

int print(void *lst)
{
	printf("dbg : %zu\n", (size_t)(lst));
	return (0);
}

int main()
{
	puts_res("strlen : ", strlen_test);
	puts_res("strcpy : ", strcpy_test);
	puts_res("strcmp : ", strcmp_test);
	puts_res("write  : ", write_test);
	puts_res("read   : ", read_test);
	puts_res("strdup : ", strdup_test);

	#ifdef BONUS
	puts("\n\n===BONUS===\n");
	puts_res("atoi   : ", atoi_base_test);
	puts_res("push   : ", list_push_front_test);
	puts_res("size   : ", list_size_test);
	puts_res("sort   : ", list_sort_test);
	puts_res("remove : ", list_remove_if_test);
	#endif

}
