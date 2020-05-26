/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgarcia- <mgarcia-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/25 21:09:13 by mgarcia-          #+#    #+#             */
/*   Updated: 2020/05/25 21:09:15 by mgarcia-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void		try_all_intersections(t_v3 ray, t_figures *lst,
					t_figures *closest_figure, double *closest_intersection)
{
	double intersection_distance;

	while (lst)
	{
		if (lst->flag == SP)
			intersection_distance = sphere_intersection(ray.o, ray.d, lst);
		else if (lst->flag == PL)
			intersection_distance = plane_intersection(ray.o, ray.d, lst);
		else if (lst->flag == TR)
			intersection_distance = triangle_intersection(ray.o, ray.d, lst);
		else if (lst->flag == SQ)
			intersection_distance = square_intersection(ray.o, ray.d, lst);
		else if (lst->flag == CY)
			intersection_distance = cylinder_intersection(ray.o, ray.d, lst);
		if (intersection_distance < *closest_intersection)
		{
			*closest_figure = *lst;
			*closest_intersection = intersection_distance;
		}
		lst = lst->next;
	}
}

double		pl_intersection(t_p3 o, t_p3 d, t_p3 plane_p, t_p3 plane_nv)
{
	double	x;
	double	denom;

	denom = dot(plane_nv, d);
	if (denom == 0)
		return (INFINITY);
	x = (dot(plane_nv, vsubstract(plane_p, o))) / denom;
	return (x > 0 ? x : INFINITY);
}

int			p_is_outside(t_p3 p1, t_p3 p2, t_p3 p3, t_p3 ip)
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

int			solve_cylinder(double x[2], t_p3 o, t_p3 d, t_figures *lst)
{
	t_p3	v;
	t_p3	u;
	double	a;
	double	b;
	double	c;

	v = scal_x_vec(dot(d, lst->fig.cy.nv), lst->fig.cy.nv);
	v = vsubstract(d, v);
	u = scal_x_vec(dot(vsubstract(o, lst->fig.cy.c), lst->fig.cy.nv),
													lst->fig.cy.nv);
	u = vsubstract(vsubstract(o, lst->fig.cy.c), u);
	a = dot(v, v);
	b = 2 * dot(v, u);
	c = dot(u, u) - pow(lst->fig.cy.r, 2);
	x[0] = (-b + sqrt(pow(b, 2) - 4 * a * c)) / (2 * a);
	x[1] = (-b - sqrt(pow(b, 2) - 4 * a * c)) / (2 * a);
	if (x[0] != x[0] && x[1] != x[1])
		return (0);
	if (x[0] < EPSILON && x[1] < EPSILON)
		return (0);
	return (1);
}

t_p3		calc_cy_normal(double x2[2], t_p3 o, t_p3 d, t_figures *lst)
{
	double	dist;
	double	x;

	if ((lst->fig.cy.dist1 >= 0 && lst->fig.cy.dist1 <= lst->fig.cy.h
				&& x2[0] > EPSILON) && (lst->fig.cy.dist2 >= 0
				&& lst->fig.cy.dist2 <= lst->fig.cy.h && x2[1] > EPSILON))
	{
		dist = x2[0] < x2[1] ? lst->fig.cy.dist1 : lst->fig.cy.dist2;
		x = x2[0] < x2[1] ? x2[0] : x2[1];
	}
	else if (lst->fig.cy.dist1 >= 0 && lst->fig.cy.dist1 <= lst->fig.cy.h
														&& x2[0] > EPSILON)
	{
		dist = lst->fig.cy.dist1;
		x = x2[0];
	}
	else
	{
		dist = lst->fig.cy.dist2;
		x = x2[1];
	}
	x2[0] = x;
	return (normalize(vsubstract(vsubstract(scal_x_vec(x, d),
			scal_x_vec(dist, lst->fig.cy.nv)), vsubstract(lst->fig.cy.c, o))));
}
