/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   procedural_textures.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgarcia- <mgarcia-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/30 18:18:07 by mgarcia-          #+#    #+#             */
/*   Updated: 2020/05/30 18:18:09 by mgarcia-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static int	checkerboard(t_inter *inter)
{
	int		black;
	int		white;
	t_p3	coords;
	t_p3	val;
	int		party_mix;

	black = 0x000000;
	white = 0xffffff;
	coords.x = abs((int)floor(inter->p.x));
	coords.y = abs((int)floor(inter->p.y));
	coords.z = abs((int)floor(inter->p.z));
	val.x = (int)coords.x % 2;
	val.y = (int)coords.y % 2;
	val.z = (int)coords.z % 2;
	party_mix = ((int)val.x ^ (int)val.y) ^ (int)val.z;
	return (party_mix ? black : white);
}

static t_p3	sinwave(t_inter *inter, t_figures *lst)
{
	double	sinn;
	double	scal;

	scal = lst->wavelength;
	sinn = sin(inter->p.z * scal) + sin(inter->p.y * scal);
	return (x_axis_rotation(inter->normal, sinn));
}

void		apply_texture(int texture, t_inter *inter, t_figures *lst)
{
	if (texture == 1)
		inter->color = checkerboard(inter);
	else if (texture == 2)
		inter->normal = sinwave(inter, lst);
	else if (texture == 3)
		inter->color = rainbow(inter);
}
