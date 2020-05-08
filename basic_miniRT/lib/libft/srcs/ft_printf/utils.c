/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgarcia- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/23 16:22:38 by mgarcia-          #+#    #+#             */
/*   Updated: 2020/03/12 15:44:23 by mgarcia-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int			ft_stoi(const char **str)
{
	int i;

	i = 0;
	while (ft_isdigit(**str))
		i = i * 10 + (*((*str)++) - '0');
	return (i);
}

int			nbrlen(size_t nb, int base)
{
	int		len;

	len = 1;
	while (nb > (size_t)(base - 1))
	{
		nb /= base;
		len++;
	}
	return (len);
}

void		putchar_buff(char c, char *buf, t_flags *f)
{
	if (f->i < BUFFSIZE)
		buf[f->i++] = c;
	else
	{
		write(1, buf, BUFFSIZE);
		f->idx += BUFFSIZE;
		f->i = 0;
		putchar_buff(c, buf, f);
	}
}

void		putstr_buff(char *s, char *buf, t_flags *f)
{
	while (*s && f->i < BUFFSIZE)
		buf[f->i++] = *s++;
	if (f->i == BUFFSIZE)
	{
		write(1, buf, BUFFSIZE);
		f->idx += BUFFSIZE;
		f->i = 0;
		putstr_buff(s, buf, f);
	}
}

void		putnbr_buff(size_t nb, int len, char *buf, t_flags *f)
{
	if (nb > (size_t)(f->base - 1))
		putnbr_buff(nb / f->base, len + 1, buf, f);
	else if (f->flags & FLAG_PRECISION)
	{
		while (f->precision > len++)
			putchar_buff('0', buf, f);
	}
	if (nb % f->base < 10)
		putchar_buff(nb % f->base + 48, buf, f);
	else
		putchar_buff((
			f->flags & FLAG_UPPERCASE ? 'A' : 'a') + nb % f->base - 10, buf, f);
	if (f->flags & FLAG_SEP && (len - 1) % 3 == 0 && len != 1)
		putchar_buff(',', buf, f);
}
