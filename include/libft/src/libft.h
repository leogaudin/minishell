/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgaudin <lgaudin@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 16:50:27 by lgaudin           #+#    #+#             */
/*   Updated: 2023/07/18 15:40:23 by lgaudin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stdarg.h>
# include <stdbool.h>
# include <stdlib.h>
# include <unistd.h>

// Original libft
int					ft_atoi(const char *str);
void				ft_bzero(void *s, size_t n);
void				*ft_calloc(size_t nelem, size_t elsize);
int					ft_isalnum(int c);
int					ft_isalpha(int c);
int					ft_isascii(int c);
int					ft_isdigit(int c);
int					ft_isprint(int c);
char				*ft_itoa(int n);
void				*ft_memchr(const void *s, int c, size_t n);
int					ft_memcmp(const void *s1, const void *s2, size_t n);
void				*ft_memcpy(void *dest, const void *src, size_t n);
void				*ft_memmove(void *dest, const void *src, size_t n);
void				*ft_memset(void *s, int c, size_t n);
void				ft_putchar_fd(char c, int fd);
void				ft_putendl_fd(char *s, int fd);
void				ft_putnbr_fd(int n, int fd);
void				ft_putstr_fd(char *s, int fd);
char				**ft_split(char const *s, char c);
char				*ft_strchr(const char *s, int c);
char				*ft_strdup(char *src);
void				ft_striteri(char *s, void (*f)(unsigned int, char *));
char				*ft_strjoin(char const *s1, char const *s2);
size_t				ft_strlcat(char *dest, char *src, size_t size);
size_t				ft_strlcpy(char *dest, char *src, size_t size);
size_t				ft_strlen(const char *str);
char				*ft_strmapi(char const *s, char (*f)(unsigned int, char));
int					ft_strncmp(const char *s1, const char *s2, size_t n);
char				*ft_strnstr(const char *big, const char *little,
						size_t len);
char				*ft_strrchr(const char *s, int c);
char				*ft_strtrim(char const *s1, char const *set);
char				*ft_substr(char const *s, unsigned int start, size_t len);
int					ft_tolower(int c);
int					ft_toupper(int c);
typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}					t_list;
t_list				*ft_lstnew(void *content);
void				ft_lstadd_front(t_list **lst, t_list *new);
int					ft_lstsize(t_list *lst);
t_list				*ft_lstlast(t_list *lst);
void				ft_lstadd_back(t_list **lst, t_list *new);
void				ft_lstdelone(t_list *lst, void (*del)(void *));
void				ft_lstclear(t_list **lst, void (*del)(void *));
void				ft_lstiter(t_list *lst, void (*f)(void *));
t_list				*ft_lstmap(t_list *lst, void *(*f)(void *),
						void (*del)(void *));

// get_next_line
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 50
# endif

int					get_nl_index(char *stash);
char				*get_next_line(int fd);

// ft_printf
int					ft_printf(const char *format, ...);
int					ft_print_char(int c);
int					ft_print_string(char *str);
int					ft_print_hexa(unsigned long long n, char c);
int					ft_print_pointer(unsigned long long p);
int					ft_print_int(int n);
int					ft_print_unsigned(unsigned int n);
int					ft_print_percent(void);

// Custom add-ons
void				ft_free_tab(char **tab);
int					ft_tab_len(char **tab);
char				*ft_strnstr_rev(const char *big, const char *little,
						size_t len);
char				**ft_split_str(char const *s, char *token);
char				**ft_split_str_once(char const *s, char *token);
int					ft_strcmp(const char *s1, const char *s2);
char				*ft_strndup(char *str, int n);
char				*ft_get_bef_char(char *str, char c);
char				*ft_get_aft_char(char *str, char c);
char				*ft_get_bef_char_rev(char *str, char c);
void				*ft_malloc_error(void);
bool				ft_isspace(char c);
char				*ft_strip_quotes(char *str);
int					ft_strnstr_index(const char *big, const char *little,
						size_t len);
bool				is_between_parenthesis(char *str, int index);
char				*strip_parenthesis(char *str);
bool				exists_out_parenthesis(char *command, char *token);

#endif
