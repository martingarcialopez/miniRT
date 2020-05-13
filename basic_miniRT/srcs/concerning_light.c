/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   concerning_light.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgarcia- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/10 13:06:14 by mgarcia-          #+#    #+#             */
/*   Updated: 2020/03/12 20:01:04 by mgarcia-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void				add_coeficient(double (*rgb)[3], double coef, int color)
{
	unsigned int	mask;

	mask = 255 << 16;
	(*rgb)[0] += coef * ((color & mask) >> 16) / 255;
	mask >>= 8;
	(*rgb)[1] += coef * ((color & mask) >> 8) / 255;
	mask >>= 8;
	(*rgb)[2] += coef * (color & mask) / 255;
}

void				compute_light(t_inter *inter, t_scene data, t_figures *lst,
					double (**fun_ptr)())
{
	double			light;
	double			rgb[3];
	t_p3			direction;

	light = 0.0;
	ft_memset(rgb, 0, 3 * sizeof(double));
	add_coeficient(&rgb, data.ambient_light, data.al_color);
	while (data.l)
	{
		direction = vsubstract(data.l->o, inter->p);
		if (is_lit(inter->p, direction, lst, fun_ptr)
				&& dot(inter->normal, direction) > 0)
		{
			light = data.l->br * vcos(inter->normal, direction);
			add_coeficient(&rgb, light, data.l->color);
		}
		data.l = data.l->next;
	}
	inter->color = color_x_light(inter->color, rgb);
}

void				calc_normal(t_p3 p, t_p3 d, t_p3 *normal, t_figures l)
{
	if ((l.flag == PL) || (l.flag == SQ) || (l.flag == TR) || (l.flag == CY))
		*normal = vcos(d, l.normal) > 0 ? scal_x_vec(-1, l.normal) : l.normal;
	else if (l.flag == SP)
	{
		*normal = normalize(vsubstract(p, l.fig.sp.c));
		*normal = vcos(d, l.normal) > 0 ? scal_x_vec(-1, *normal) : *normal;
	}
//	else if (l.flag == CY)
//	*normal = l.normal;
}

int					is_lit(t_p3 o, t_p3 d, t_figures *lst,
		double (*fun_ptr[NUM_FIGS])(t_p3, t_p3, t_figures *))
{
	double			in;

	while (lst)
	{
		in = (fun_ptr[lst->flag])(o, d, lst);
		if (in > 0.000001 && in < 1)
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
