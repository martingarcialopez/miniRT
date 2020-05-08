/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   text_format.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgarcia- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/22 11:51:44 by mgarcia-          #+#    #+#             */
/*   Updated: 2019/12/02 15:46:44 by mgarcia-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void			format_string(t_flags *f, char *buf, va_list ap)
{
	char	*str;
	int		len;
	int		i;

	str = va_arg(ap, char *);
	if (!str)
		str = "(null)";
	len = ft_strlen(str);
	if (f->flags & FLAG_PRECISION)
		len = f->precision > len ? len : f->precision;
	if (!(f->flags & FLAG_LEFT))
	{
		i = 0;
		while (i++ < (f->width - len))
			putchar_buff(32, buf, f);
	}
	i = 0;
	while (*str && i++ < len)
		putchar_buff(*str++, buf, f);
	if (f->flags & FLAG_LEFT)
	{
		i = 0;
		while (i++ < (f->width - len))
			putchar_buff(32, buf, f);
	}
}

void			format_character(char c, char *buf, t_flags *f)
{
	char pad;

	pad = f->flags & FLAG_ZEROPAD ? '0' : 32;
	if (!(f->flags & FLAG_LEFT))
	{
		while (--(f->width) > 0)
			putchar_buff(pad, buf, f);
	}
	putchar_buff(c, buf, f);
	if (f->flags & FLAG_LEFT)
	{
		while (--(f->width) > 0)
			putchar_buff(32, buf, f);
	}
}

void			write_ptr(t_flags *f, va_list ap)
{
	int		*ptr;

	ptr = pointer_cast(f, ap);
	*ptr = f->idx + f->i;
}

void			format_address(char *buf, t_flags *f, va_list ap)
{
	f->flags |= FLAG_HEX_ADDR;
	if (f->flags & FLAG_PRECISION || f->flags & FLAG_LEFT)
		f->flags &= ~FLAG_ZEROPAD;
	f->flags &= ~(FLAG_PLUS | FLAG_SPACE);
	f->flags |= FLAG_HASH;
	f->base = 16;
	format_number(va_arg(ap, size_t), 0, buf, f);
}
