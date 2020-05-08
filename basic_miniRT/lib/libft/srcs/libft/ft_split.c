/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgarcia- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/09 11:09:27 by mgarcia-          #+#    #+#             */
/*   Updated: 2019/10/18 14:54:50 by mgarcia-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int		ft_custom_strlen(const char *str, char sep)
{
	int i;

	i = 0;
	while (str[i] && str[i] != sep)
		i++;
	return (i);
}

static int		ft_count_words(const char *str, char sep)
{
	int words;

	words = 0;
	while (*str)
	{
		while (*str && *str == sep)
			str++;
		if (*str && *str != sep)
			words++;
		while (*str && *str != sep)
			str++;
	}
	return (words);
}

static char		*ft_custom_strdup(const char *src, int len, char sep)
{
	char	*dup;
	int		i;

	if (!(dup = (char*)malloc(sizeof(char) * (len + 1))))
		return (NULL);
	i = 0;
	while (*src && *src != sep)
		dup[i++] = *src++;
	dup[i] = '\0';
	return (dup);
}

char			**ft_split(char const *s, char c)
{
	char	**tab;
	int		words;
	int		len;
	int		i;

	if (!s)
		return (NULL);
	words = ft_count_words(s, c);
	if (!(tab = (char **)malloc(sizeof(char *) * (words + 1))))
		return (NULL);
	i = 0;
	while (i < words)
	{
		while (*s && *s == c)
			s++;
		len = ft_custom_strlen(s, c);
		tab[i] = ft_custom_strdup(s, len, c);
		while (*s && *s != c)
			s++;
		i++;
	}
	tab[i] = 0;
	return (tab);
}
