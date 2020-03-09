/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   concerning_light.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgarcia- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/10 13:06:14 by mgarcia-          #+#    #+#             */
/*   Updated: 2020/03/09 20:04:32 by mgarcia-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void		add_coeficient(double (*rgb)[3], double coeficient, int color)
{
	unsigned int	mask;

	mask = 255 << 16;
	(*rgb)[0] += coeficient * ((color & mask) >> 16) / 255;
	mask >>= 8;
	(*rgb)[1] += coeficient * ((color & mask) >> 8) / 255;
	mask >>= 8;
	(*rgb)[2] += coeficient * (color & mask) / 255;
}

void		compute_light(int *color, t_p3 p, t_p3 normal, t_scene data, t_figures *lst)
{
	double	light;
	double	rgb[3];
	t_p3	direction;
	t_p3	p_to_cam;

	light = 0.0;
	rgb[0] = 0.0;
	rgb[1] = 0.0;
	rgb[2] = 0.0;
	//light += data.ambient_light;
	add_coeficient(&rgb, data.ambient_light, data.al_color);
	while (data.l)
	{
	    direction = substract_vectors(data.l->o, p);
	    if (is_lit(p, direction, lst) && dot(normal, direction) > 0)
		{
			light = (data.l->br * dot(normal, direction)) / (mod(normal) * mod(direction));
			add_coeficient(&rgb, light, data.l->color);
		}
	    data.l = data.l->next;
	}
	*color = color_x_light(*color, &rgb);
}

t_p3		calc_normal(t_p3 p, t_figures lst)
{
	t_p3	normal;

	if (lst.flag & SP)
		normal = normalize(substract_vectors(p, lst.fig.sp.c));
	else if (lst.flag & PL)
		normal = lst.fig.pl.nv;

	return (normal);
}

int		is_lit(t_p3 O, t_p3 d, t_figures *lst)
{
	double in;

	while (lst)
	{
		if (lst->flag & SP)
			in = sphere_intersection(O, d, lst);
		else if (lst->flag & PL)
			in = plane_intersection(O, d, lst);
		/*else if (lst->flag & SQ)
			in = sqare_intersection;*/
		

		if (in > 0.0001 && in < 1)
			return (0);
		lst = lst->next;
	}
	return (1);
}

int					color_x_light(int color, double rgb[3])
{
	unsigned int	mask;
	unsigned int	r;
	unsigned int	g;
	unsigned int	b;

	mask = 255 << 16;
	r = rgb[0] * ((color & mask) >> 16);
	mask >>= 8;
	g = rgb[1] * ((color & mask) >> 8);
	mask >>= 8;
	b = rgb[2] * (color & mask);

	r = r > 255 ? 255 : r;
	g = g > 255 ? 255 : g;
	b = b > 255 ? 255 : b;

	return ((r << 16) | (g << 8) | b);
}
