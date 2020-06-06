/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgarcia- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/07 14:11:18 by mgarcia-          #+#    #+#             */
/*   Updated: 2019/12/02 21:44:45 by mgarcia-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_atoi(const char *str)
{
	long	nb;
	int		neg;
	int		ovf;

	nb = 0;
	neg = 1;
	ovf = 1;
	while (*str == 32 || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			neg = -1;
		str++;
	}
	while ((*str >= '0' && *str <= '9') && ovf == 1)
	{
		nb = (nb * 10) + *str++ - 48;
		if (nb < 0)
			ovf = (neg == 1 ? -1 : 0);
	}
	return (ovf != 1 ? ovf : (int)(nb * neg));
}
