/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersections.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgarcia- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/10 13:02:52 by mgarcia-          #+#    #+#             */
/*   Updated: 2020/03/12 19:54:20 by mgarcia-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

double		plane_intersection(t_p3 o, t_p3 d, t_figures *lst)
{
	return (pl_intersection(o, d, lst->fig.pl.p, lst->normal));
}

double		sphere_intersection(t_p3 o, t_p3 d, t_figures *lst)
{
	double	disc;
	t_p3	oc;
	double	k[3];
	double	x1;
	double	x2;

	oc = vsubstract(o, lst->fig.sp.c);
	k[0] = dot(d, d);
	k[1] = 2 * dot(d, oc);
	k[2] = dot(oc, oc) - lst->fig.sp.r * lst->fig.sp.r;
	disc = k[1] * k[1] - (4 * k[0] * k[2]);
	x1 = (-k[1] + sqrt(disc)) / (2 * k[0]);
	x2 = (-k[1] - sqrt(disc)) / (2 * k[0]);
	if (disc < 0)
		return (INFINITY);
	else if (disc == 0)
		return (x1);
	else
		return (x1 < x2 ? x1 : x2);
}

double		square_intersection(t_p3 o, t_p3 d, t_figures *lst)
{
	t_p3	ip;
	t_sq	sq;
	double	id;
	double	cos1;
	double	limit;

	id = pl_intersection(o, d, lst->fig.sq.c, lst->normal);
	ip = vadd(o, scal_x_vec(id, d));
	sq.floor = vdefine(0, 1, 0);
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

double		triangle_intersection(t_p3 o, t_p3 d, t_figures *lst)
{
	double	id;
	t_p3	ip;

	id = pl_intersection(o, d, lst->fig.tr.p1, lst->normal);
	ip = vadd(o, scal_x_vec(id, d));
	if (p_is_outside(lst->fig.tr.p1, lst->fig.tr.p2, lst->fig.tr.p3, ip))
		return (INFINITY);
	if (p_is_outside(lst->fig.tr.p2, lst->fig.tr.p3, lst->fig.tr.p1, ip))
		return (INFINITY);
	if (p_is_outside(lst->fig.tr.p3, lst->fig.tr.p1, lst->fig.tr.p2, ip))
		return (INFINITY);
	return (id);
}

double		cylinder_intersection(t_p3 o, t_p3 d, t_figures *lst)
{
	double	x2[2];

	if (solve_cylinder(x2, o, d, lst) == 0)
		return (INFINITY);
	lst->fig.cy.dist1 = dot(lst->fig.cy.nv, vsubstract(scal_x_vec(x2[0], d),
												vsubstract(lst->fig.cy.c, o)));
	lst->fig.cy.dist2 = dot(lst->fig.cy.nv, vsubstract(scal_x_vec(x2[1], d),
												vsubstract(lst->fig.cy.c, o)));
	if (!((lst->fig.cy.dist1 > 0 && lst->fig.cy.dist1 < lst->fig.cy.h)
			|| (lst->fig.cy.dist2 > 0 && lst->fig.cy.dist2 < lst->fig.cy.h)))
		return (INFINITY);
	lst->normal = calc_cy_normal(x2, o, d, lst);
	return (x2[0]);
}
