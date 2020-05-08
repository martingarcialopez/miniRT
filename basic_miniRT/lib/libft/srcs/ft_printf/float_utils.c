/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   float_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgarcia- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/30 14:05:12 by mgarcia-          #+#    #+#             */
/*   Updated: 2019/12/02 20:49:33 by mgarcia-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

size_t		pwr(int base, int exp)
{
	size_t	nb;

	nb = 1;
	while (exp--)
		nb *= base;
	return (nb);
}

int			valid_double(long double value, char *buf, t_flags *f)
{
	if (value != value)
	{
		putstr_buff("nan", buf, f);
		return (0);
	}
	if (value < -DBL_MAX)
	{
		putstr_buff("-inf", buf, f);
		return (0);
	}
	if (value > DBL_MAX)
	{
		putstr_buff((f->flags & FLAG_PLUS ? "+inf" : "inf"), buf, f);
		return (0);
	}
	return (1);
}

void		set_fflags(long double *value, t_float *fl, t_flags *f)
{
	if (f->flags & FLAG_LEFT)
		f->flags &= ~FLAG_ZEROPAD;
	if (!(f->flags & FLAG_PRECISION))
		f->precision = DEF_FLOAT_PREC;
	else
		f->flags &= ~FLAG_PRECISION;
	fl->neg = *value < 0 ? 1 : 0;
	*value = *value < 0 ? -(*value) : *value;
}
