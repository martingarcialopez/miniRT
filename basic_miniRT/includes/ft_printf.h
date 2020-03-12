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
# include "structs.h"

# define DEFAULT_FLOAT_PRECISION 6
# define BUFFSIZE 64

int		ft_printf(const char *format, ...) __attribute__((format(printf,1,2)));

int		ft_vprintf(const char *format, va_list ap);

/*
**		argument parsing
*/

void	eval_flags(const char **format, t_flags *f);

void	eval_width(const char **format, t_flags *f, va_list ap);

void	eval_precision(const char **format, t_flags *f, va_list ap);

void	eval_length(const char **format, t_flags *f);

/*
** 		format types functions
*/

void	format_character(char c, char *buf, t_flags *f);

void	format_string(t_flags *f, char *buf, va_list ap);

void	format_integer(char fmt, char *buf, t_flags *f, va_list ap);

void	format_number(size_t nb, int negative, char *buf, t_flags *f);

void	format_float(long double value, char *buf, t_flags *f);

void	format_address(char *buf, t_flags *f, va_list ap);

void	write_ptr(t_flags *f, va_list ap);

/*
**		Data type conversions
*/

ssize_t	signed_cast(t_flags *f, va_list ap);

size_t	unsigned_cast(t_flags *f, va_list ap);

void	*pointer_cast(t_flags *f, va_list ap);

/*
**      help functions
*/

int		ft_stoi(const char **str);

int		nbrlen(size_t nb, int base);

void	putchar_buff(char c, char *buf, t_flags *f);

void	putstr_buff(char *s, char *buf, t_flags *f);

void	putnbr_buff(size_t nb, int len, char *buf, t_flags *f);

size_t	pwr(int base, int exp);

int		valid_double(long double value, char *buf, t_flags *f);

void	set_fflags(long double *value, t_float *fl, t_flags *f);

#endif
