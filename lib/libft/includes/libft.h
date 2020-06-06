/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgarcia- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/16 21:15:32 by mgarcia-          #+#    #+#             */
/*   Updated: 2019/10/18 18:16:52 by mgarcia-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stdlib.h>
# include <unistd.h>

typedef struct		s_list
{
	void			*content;
	struct s_list	*next;
}					t_list;

int					ft_printf(const char *format, ...);

int					ft_atoi(const char *str);

size_t				ft_strlen(const char *str);

char				*ft_strcat(char *dest, const char *src);

char				*ft_strncat(char *s1, const char *s2, size_t n);

size_t				ft_strlcat(char *dst, const char *src, size_t size);

int					ft_strcmp(const char *str1, const char *str2);

int					ft_strncmp(const char *str1, const char *str2, size_t n);

char				*ft_strcpy(char *dest, const char *src);

char				*ft_strncpy(char *dst, const char *src, size_t len);

size_t				ft_strlcpy(char *st, const char *src, size_t dstsize);

char				*ft_strstr(const char *haystack, const char *needle);

char				*ft_strnstr(const char *haystack, const char *needle,
						size_t len);

char				*ft_strdup(const char *s1);

char				*ft_strchr(const char *s, int c);

char				*ft_strrchr(const char *s, int c);

int					ft_isalpha(int c);

int					ft_isdigit(int c);

int					ft_isalnum(int c);

int					ft_isascii(int c);

int					ft_isprint(int c);

int					ft_toupper(int c);

int					ft_tolower(int c);

char				*ft_strrev(char *str);

void				*ft_calloc(size_t count, size_t size);

void				*ft_memset(void *b, int c, size_t len);

void				ft_bzero(void *s, size_t n);

void				*ft_memcpy(void *dst, const void *src, size_t n);

void				*ft_memccpy(void *dst, const void *src, int c, size_t n);

void				*ft_memchr(const void *s, int c, size_t n);

int					ft_memcmp(const void *s1, const void *s2, size_t n);

void				*ft_memmove(void *dst, const void *src, size_t len);

char				*ft_strjoin(char const *s1, char const *s2);

void				ft_putchar(char c);

void				ft_putchar_fd(char c, int fd);

void				ft_putstr(char *s);

void				ft_putstr_fd(char *s, int fd);

void				ft_putendl(char *s);

void				ft_putendl_fd(char *s, int fd);

void				ft_putnbr(int n);

void				ft_putnbr_fd(int n, int fd);

char				*ft_strmapi(char const *s, char (*f)(unsigned int, char));

char				**ft_split(char const *s, char c);

char				*ft_itoa(int n);

char				*ft_substr(char const *s, unsigned int start, size_t len);

char				*ft_strtrim(char const *s1, char const *set);

t_list				*ft_lstnew(void *content);

void				ft_lstadd_front(t_list **alst, t_list *new);

int					ft_lstsize(t_list *lst);

t_list				*ft_lstlast(t_list *lst);

void				ft_lstadd_back(t_list **alst, t_list *new);

void				ft_lstdelone(t_list *lst, void (*del)(void *));

void				ft_lstclear(t_list **lst, void (*del)(void *));

void				ft_lstiter(t_list *lst, void (*f)(void *));

t_list				*ft_lstmap(t_list *lst, void *(*f)(void *),
						void (*del)(void *));

#endif
