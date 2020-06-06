/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   integer_format.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgarcia- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/22 15:41:58 by mgarcia-          #+#    #+#             */
/*   Updated: 2019/12/02 17:45:16 by mgarcia-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void		set_flags(size_t nb, int negative, int *len, t_flags *f)
{
	*len = nbrlen(nb, f->base);
	if (f->base == 8 && f->flags & FLAG_PRECISION && *len < f->precision)
		f->flags &= ~FLAG_HASH;
	*len = *len < f->precision ? f->precision : *len;
	if (!nb && f->flags & FLAG_PRECISION && f->precision == 0)
		*len = 0;
	if (!nb && ((!(f->flags & FLAG_PRECISION) && f->base == 8)
		|| ((f->base == 10 || f->base == 16) && !(f->flags & FLAG_HEX_ADDR))))
		f->flags &= ~FLAG_HASH;
	if (f->width && (negative || f->flags & (FLAG_PLUS | FLAG_SPACE)))
		f->width--;
	if (f->width && (f->flags & FLAG_HASH) && (f->base == 16 || f->base == 8))
	{
		if (f->base == 16)
			f->width -= (f->width == 1) ? 1 : 2;
		else if (f->base == 8)
			f->width -= 1;
	}
}

static void		add_prefix(int negative, char *buf, t_flags *f)
{
	if (negative || f->flags & (FLAG_PLUS | FLAG_SPACE))
	{
		if (negative)
			putchar_buff('-', buf, f);
		else if (f->flags & FLAG_PLUS)
			putchar_buff('+', buf, f);
		else if (f->flags & FLAG_SPACE)
			putchar_buff(' ', buf, f);
	}
	else if (f->flags & FLAG_HASH)
	{
		putchar_buff('0', buf, f);
		if (f->flags & FLAG_UPPERCASE)
			putchar_buff('X', buf, f);
		else if (f->base == 16)
			putchar_buff('x', buf, f);
	}
}

static void		add_padding(int len, char *buf, t_flags *f)
{
	char pad;

	pad = f->flags & FLAG_ZEROPAD ? '0' : 32;
	while (len++ < f->width)
		putchar_buff(pad, buf, f);
}

void			format_number(size_t nb, int negative, char *buf, t_flags *f)
{
	int		len;

	set_flags(nb, negative, &len, f);
	if (!(f->flags & FLAG_LEFT))
	{
		if (f->flags & FLAG_ZEROPAD)
			add_prefix(negative, buf, f);
		add_padding(len, buf, f);
	}
	if (!(f->flags & FLAG_ZEROPAD))
		add_prefix(negative, buf, f);
	if (nb || !(f->flags & FLAG_PRECISION) || f->precision != 0)
		putnbr_buff(nb, 1, buf, f);
	if (f->flags & FLAG_LEFT)
		add_padding(len, buf, f);
}

void			format_integer(char fmt, char *buf, t_flags *f, va_list ap)
{
	ssize_t		val;
	size_t		uval;

	if (fmt == 'x' || fmt == 'X')
		f->base = 16;
	else if (fmt == 'o')
		f->base = 8;
	else
		f->base = 10;
	if (fmt == 'X')
		f->flags |= FLAG_UPPERCASE;
	if (f->flags & FLAG_PRECISION || f->flags & FLAG_LEFT)
		f->flags &= ~FLAG_ZEROPAD;
	if (fmt == 'i' || fmt == 'd')
	{
		f->flags &= ~FLAG_HASH;
		val = signed_cast(f, ap);
		format_number((val > 0 ? val : -val), val < 0, buf, f);
	}
	else
	{
		f->flags &= ~(FLAG_PLUS | FLAG_SPACE);
		uval = unsigned_cast(f, ap);
		format_number(uval, 0, buf, f);
	}
}
