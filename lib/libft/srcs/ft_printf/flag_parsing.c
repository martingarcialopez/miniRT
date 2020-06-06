/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flag_parsing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgarcia- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 17:32:11 by mgarcia-          #+#    #+#             */
/*   Updated: 2020/03/12 15:45:02 by mgarcia-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	eval_flags(const char **format, t_flags *f)
{
	f->flags = 0U;
	while (**format == '#' || **format == '+' || **format == '-' ||
			**format == ' ' || **format == '0' || **format == '\'')
	{
		if (**format == '#')
			f->flags |= FLAG_HASH;
		else if (**format == '+')
			f->flags |= FLAG_PLUS;
		else if (**format == '-')
			f->flags |= FLAG_LEFT;
		else if (**format == ' ')
			f->flags |= FLAG_SPACE;
		else if (**format == '0')
			f->flags |= FLAG_ZEROPAD;
		else if (**format == '\'')
			f->flags |= FLAG_SEP;
		(*format)++;
	}
}

void	eval_width(const char **format, t_flags *f, va_list ap)
{
	f->width = 0;
	if (ft_isdigit(**format))
		f->width = ft_stoi(format);
	else if (**format == '*')
	{
		f->width = va_arg(ap, int);
		if (f->width < 0)
		{
			f->width = -(f->width);
			f->flags |= FLAG_LEFT;
		}
		(*format)++;
	}
}

void	eval_precision(const char **format, t_flags *f, va_list ap)
{
	f->precision = 0;
	if (**format == '.')
	{
		f->flags |= FLAG_PRECISION;
		(*format)++;
		if (ft_isdigit(**format))
			f->precision = ft_stoi(format);
		else if (**format == '*')
		{
			f->precision = va_arg(ap, int);
			if (f->precision < 0)
				f->flags &= ~FLAG_PRECISION;
			(*format)++;
		}
	}
}

void	eval_length(const char **format, t_flags *f)
{
	if (**format == 'l')
	{
		f->flags |= FLAG_LONG;
		(*format)++;
		if (**format == 'l')
		{
			f->flags |= FLAG_LONG_LONG;
			(*format)++;
		}
	}
	else if (**format == 'h')
	{
		f->flags |= FLAG_SHORT;
		(*format)++;
		if (**format == 'h')
		{
			f->flags |= FLAG_CHAR;
			(*format)++;
		}
	}
	else if (**format == 'L')
	{
		f->flags |= FLAG_LONG_DBL;
		(*format)++;
	}
}
