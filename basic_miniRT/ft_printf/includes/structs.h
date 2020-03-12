/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgarcia- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/27 15:06:15 by mgarcia-          #+#    #+#             */
/*   Updated: 2019/12/02 19:46:42 by mgarcia-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

# define FLAG_ZEROPAD   1
# define FLAG_LEFT      2
# define FLAG_PLUS     	4
# define FLAG_SPACE     8
# define FLAG_HASH      16
# define FLAG_UPPERCASE 32
# define FLAG_CHAR      64
# define FLAG_SHORT     128
# define FLAG_LONG      256
# define FLAG_LONG_LONG 512
# define FLAG_PRECISION 1024
# define FLAG_LONG_DBL	2048
# define FLAG_HEX_ADDR	4096
# define FLAG_SEP		8192

typedef struct	s_flags
{
	unsigned int	i;
	size_t			idx;
	int				base;
	unsigned int	flags;
	int				width;
	int				precision;
	struct lconv	*lc;
}				t_flags;

typedef struct	s_float
{
	ssize_t			neg;
	ssize_t			whole;
	long double		diff;
	long double		tmp;
	size_t			frac;
}				t_float;

long double		float_cast(t_flags *f, va_list ap);

#endif
