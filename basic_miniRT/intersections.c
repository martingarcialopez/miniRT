/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersections.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgarcia- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/10 13:02:52 by mgarcia-          #+#    #+#             */
/*   Updated: 2020/03/10 02:17:20 by mgarcia-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

double		plane_intersection(t_p3 o, t_p3 d, t_p3 plane_p, t_p3 plane_nv)
{
	double	x;
	double	denom;

	denom  = dot(plane_nv, d);

	if (denom == 0)
		return (INFINITY);

	//x = (dot(lst->fig.pl.nv, lst->fig.pl.p) - dot(lst->fig.pl.nv, d)) / denom;
	x = (dot(plane_nv, substract_vectors(plane_p, o))) / denom;

	return (x > 0 ? x : INFINITY);
}

double		sphere_intersection(t_p3 o, t_p3 d, t_figures *lst)
{
	//return type int, 1 si corta loksea y 0 si no corta ni bakalaoo
	double	disc;
	t_p3	oc;
	double	k1;
	double 	k2;
	double	k3;
	double	x1;
	double	x2;

	oc = substract_vectors(o, lst->fig.sp.c);
	k1 = dot(d, d);
	k2 = 2 * dot(d, oc);
	k3 = dot(oc, oc) - lst->fig.sp.r * lst->fig.sp.r;

	//disc =  dot(data.nv, oc) * dot(data.nv, oc) - (dot(oc, oc) - (lst->fig.sp.r * lst->fig.sp.r));
	disc = k2 * k2 - (4 * k1 * k3);

	x1 = (-k2 + sqrt(disc)) / (2 * k1);
	x2 = (-k2 - sqrt(disc)) / (2 * k1);

	if (disc < 0)
		return (INFINITY);
	else if (disc == 0)
		return (x1);
	else
		return (x1 < x2 ? x1 : x2);
}


double		vec_cos(t_p3 a, t_p3 b)
{
	return (dot(a, b) / (mod(a) * mod(b)));
}

double		square_intersection(t_p3 o, t_p3 d, t_figures *lst)
{
	t_p3	ip;
	double	id;
	t_p3	half_size;
	t_p3	floor;
	t_p3	center_to_ip;
	double	cos1;
	double	limit;
	
	id	= plane_intersection(o, d, lst->fig.sq.c, lst->fig.sq.nv);
	ip = add_vectors(o, scal_x_vec(id, d));
	floor = define_vect(0, 1, 0);
	half_size = cross_product(lst->fig.sq.nv, floor);
	center_to_ip = substract_vectors(ip, lst->fig.sq.c);
	cos1 = fabs(vec_cos(half_size, center_to_ip));
	if (cos1 < sqrt(2) / 2)
		cos1 = cos(M_PI_2 - acos(cos1));
	limit = (lst->fig.sq.side / 2) / cos1;
	if (mod(center_to_ip) <= limit)
		return (id);
	return (INFINITY);
}
