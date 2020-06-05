/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane_intersection.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgarcia- <mgarcia-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/05 19:29:55 by mgarcia-          #+#    #+#             */
/*   Updated: 2020/06/05 19:29:56 by mgarcia-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

double			solve_plane(t_p3 o, t_p3 d, t_p3 plane_p, t_p3 plane_nv)
{
	double	x;
	double	denom;

	denom = dot(plane_nv, d);
	if (denom == 0)
		return (INFINITY);
	x = (dot(plane_nv, vsubstract(plane_p, o))) / denom;
	return (x > 0 ? x : INFINITY);
}

double			plane_intersection(t_p3 o, t_p3 d, t_figures *lst)
{
	return (solve_plane(o, d, lst->fig.pl.p, lst->normal));
}

double			square_intersection(t_p3 o, t_p3 d, t_figures *lst)
{
	t_p3	ip;
	t_sq	sq;
	double	id;
	double	cos1;
	double	limit;

	id = solve_plane(o, d, lst->fig.sq.c, lst->normal);
	ip = vadd(o, scal_x_vec(id, d));
	sq.floor = fabs(lst->normal.y) == 1 ? vdefine(1, 0, 0) : vdefine(0, 1, 0);
	sq.half_size = cross(lst->normal, sq.floor);
	sq.center_to_ip = vsubstract(ip, lst->fig.sq.c);
	cos1 = fabs(vcos(sq.half_size, sq.center_to_ip));
	if (cos1 < sqrt(2) / 2)
		cos1 = cos(M_PI_2 - acos(cos1));
	limit = (lst->fig.sq.side / 2) / cos1;
	if (mod(sq.center_to_ip) <= limit)
		return (id);
	return (INFINITY);
}

static int		p_is_outside(t_p3 p1, t_p3 p2, t_p3 p3, t_p3 ip)
{
	t_p3	v1;
	t_p3	v2;
	t_p3	vp;

	v1 = vsubstract(p2, p1);
	v2 = vsubstract(p3, p1);
	vp = vsubstract(ip, p1);
	if (vcos(cross(v1, v2), cross(v1, vp)) < 0)
		return (1);
	return (0);
}

double			triangle_intersection(t_p3 o, t_p3 d, t_figures *lst)
{
	double	id;
	t_p3	ip;

	id = solve_plane(o, d, lst->fig.tr.p1, lst->normal);
	ip = vadd(o, scal_x_vec(id, d));
	if (p_is_outside(lst->fig.tr.p1, lst->fig.tr.p2, lst->fig.tr.p3, ip))
		return (INFINITY);
	if (p_is_outside(lst->fig.tr.p2, lst->fig.tr.p3, lst->fig.tr.p1, ip))
		return (INFINITY);
	if (p_is_outside(lst->fig.tr.p3, lst->fig.tr.p1, lst->fig.tr.p2, ip))
		return (INFINITY);
	return (id);
}
