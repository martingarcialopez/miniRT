/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memccpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgarcia- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/09 17:45:43 by mgarcia-          #+#    #+#             */
/*   Updated: 2019/10/16 21:06:26 by mgarcia-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memccpy(void *dst, const void *src, int c, size_t n)
{
	unsigned char		*s;
	unsigned char		*d;

	s = (unsigned char*)src;
	d = (unsigned char*)dst;
	while (n-- > 0)
	{
		*d++ = *s++;
		if (*(s - 1) == (unsigned char)c)
			return ((void*)d);
	}
	return (NULL);
}
