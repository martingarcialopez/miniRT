/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersections.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgarcia- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/10 13:02:52 by mgarcia-          #+#    #+#             */
/*   Updated: 2020/03/10 17:45:32 by mgarcia-         ###   ########.fr       */
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

	x = (dot(plane_nv, substract_vectors(plane_p, o))) / denom;

	return (x > 0 ? x : INFINITY);
}

double		sphere_intersection(t_p3 o, t_p3 d, t_figures *lst)
{
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

double		triangle_intersection(t_p3 o, t_p3 d, t_figures *lst)
{
	double	id;
	t_p3	ip;

	lst->fig.tr.nv = cross_product(substract_vectors(lst->fig.tr.p3, lst->fig.tr.p1),
			substract_vectors(lst->fig.tr.p2, lst->fig.tr.p1));
	id = plane_intersection(o, d, lst->fig.tr.p1, lst->fig.tr.nv);
	ip = add_vectors(o, scal_x_vec(id, d));
	
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
	double	disc;
	t_p3	d_x_a;
	double	k1;
	double	k2;
	double	r;
	double	x1;
	double	x2;

	r = lst->fig.cy.r;
	d_x_a = cross_product(d, lst->fig.cy.nv);
	k1 = dot(d_x_a, cross_product(lst->fig.cy.c, lst->fig.cy.nv));
	k2 = dot(d_x_a, d_x_a);
	disc = (k2 * pow(r, 2)) - (dot(lst->fig.cy.nv, lst->fig.cy.nv) * pow(dot(lst->fig.cy.c, d_x_a), 2));

	x1 = (k1 + sqrt(disc)) / k2;
	x2 = (k1 - sqrt(disc)) / k2;

	//if (mod(cross_product(d, lst->fig.cy.nv)) == 0)
	//	return (x1);
	if (disc < 0)
		return (INFINITY);
	else
		return (x1 < x2 ? x1 : x2);
}
