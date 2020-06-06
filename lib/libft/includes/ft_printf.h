/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgarcia- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/20 16:52:56 by mgarcia-          #+#    #+#             */
/*   Updated: 2020/03/12 15:45:26 by mgarcia-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <unistd.h>
# include <float.h>
# include <locale.h>
# include "libft.h"

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
# define DEF_FLOAT_PREC 6

# define BUFFSIZE 64

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

int				ft_printf(const char *format, ...);

int				ft_vprintf(const char *format, va_list ap);

/*
**		argument parsing
*/

void			eval_flags(const char **format, t_flags *f);

void			eval_width(const char **format, t_flags *f, va_list ap);

void			eval_precision(const char **format, t_flags *f, va_list ap);

void			eval_length(const char **format, t_flags *f);

/*
** 		format types functions
*/

void			format_character(char c, char *buf, t_flags *f);

void			format_string(t_flags *f, char *buf, va_list ap);

void			format_integer(char fmt, char *buf, t_flags *f, va_list ap);

void			format_number(size_t nb, int negative, char *buf, t_flags *f);

void			format_float(long double value, char *buf, t_flags *f);

void			format_address(char *buf, t_flags *f, va_list ap);

void			write_ptr(t_flags *f, va_list ap);

/*
**		Data type conversions
*/

ssize_t			signed_cast(t_flags *f, va_list ap);

size_t			unsigned_cast(t_flags *f, va_list ap);

void			*pointer_cast(t_flags *f, va_list ap);

long
double			float_cast(t_flags *f, va_list ap);

/*
**      help functions
*/

int				ft_stoi(const char **str);

int				nbrlen(size_t nb, int base);

void			putchar_buff(char c, char *buf, t_flags *f);

void			putstr_buff(char *s, char *buf, t_flags *f);

void			putnbr_buff(size_t nb, int len, char *buf, t_flags *f);

size_t			pwr(int base, int exp);

int				valid_double(long double value, char *buf, t_flags *f);

void			set_fflags(long double *value, t_float *fl, t_flags *f);

#endif
