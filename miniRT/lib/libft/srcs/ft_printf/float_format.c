/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   float_format.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgarcia- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/26 14:04:55 by mgarcia-          #+#    #+#             */
/*   Updated: 2019/12/02 20:49:33 by mgarcia-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void			add_prefix(t_float fl, char *buf, t_flags *f)
{
	if (fl.neg || f->flags & (FLAG_PLUS | FLAG_SPACE))
	{
		if (fl.neg)
			putchar_buff('-', buf, f);
		else if (f->flags & FLAG_PLUS)
			putchar_buff('+', buf, f);
		else if (f->flags & FLAG_SPACE)
			putchar_buff(' ', buf, f);
	}
}

static void			add_padding(t_float fl, char *buf, t_flags *f)
{
	int		i;
	char	pad;

	i = 0;
	pad = f->flags & FLAG_ZEROPAD ? '0' : 32;
	while ((i++ + nbrlen(fl.whole, 10) + f->precision +
			((f->precision || f->flags & FLAG_HASH) ? 1 : 0)) < f->width)
		putchar_buff(pad, buf, f);
}

static void			format_frac(t_float fl, char *buf, t_flags *f)
{
	int		n;
	int		i;

	i = 0;
	if (!fl.frac)
	{
		while (i++ < f->precision)
			putchar_buff('0', buf, f);
	}
	else
	{
		n = f->precision - nbrlen(fl.frac, 10);
		while (i++ < n)
			putchar_buff('0', buf, f);
		putnbr_buff(fl.frac, 1, buf, f);
	}
}

static void			print_float(t_float fl, char *buf, t_flags *f)
{
	if (f->width && (fl.neg || f->flags & (FLAG_PLUS | FLAG_SPACE)))
		f->width--;
	if (!(f->flags & FLAG_LEFT))
	{
		if (f->flags & FLAG_ZEROPAD)
			add_prefix(fl, buf, f);
		add_padding(fl, buf, f);
	}
	if (!(f->flags & FLAG_ZEROPAD))
		add_prefix(fl, buf, f);
	putnbr_buff(fl.whole, 1, buf, f);
	f->flags &= ~FLAG_SEP;
	if (f->precision || f->flags & FLAG_HASH)
		putchar_buff('.', buf, f);
	if (nbrlen(fl.frac, 10) != f->precision)
		format_frac(fl, buf, f);
	else
		putnbr_buff(fl.frac, 1, buf, f);
	if (f->flags & FLAG_LEFT)
		add_padding(fl, buf, f);
}

void				format_float(long double value, char *buf, t_flags *f)
{
	t_float		fl;

	f->base = 10;
	if (!valid_double(value, buf, f))
		return ;
	set_fflags(&value, &fl, f);
	fl.whole = (ssize_t)value;
	fl.tmp = (value - fl.whole) * pwr(10, f->precision);
	fl.frac = (size_t)fl.tmp;
	fl.diff = fl.tmp - fl.frac;
	if (fl.diff > 0.5)
	{
		fl.frac++;
		if (nbrlen(fl.frac, f->base) > f->precision)
		{
			fl.frac = 0;
			fl.whole++;
		}
	}
	if (!f->precision && (fl.diff == 0.5) && (fl.whole & 1))
		fl.whole++;
	print_float(fl, buf, f);
}
