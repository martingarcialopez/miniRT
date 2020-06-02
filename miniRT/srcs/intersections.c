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

#include "minirt.h"

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
	sq.floor = fabs(lst->normal.y) == 1 ? vdefine (1, 0, 0) : vdefine(0, 1, 0);
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

double		cube_intersection(t_p3 o, t_p3 d, t_figures *lst)
{
	int			i = 0;
	t_figures	sq;
	t_p3		center;
	double		cl_inter;
	t_p3		cl_normal;
	double		inter;
	t_p3		normal[6] = { {1,0,0},
							{-1,0,0},
							{0,1,0},
							{0,-1,0},
							{0,0,1},
							{0,0,-1} };
	center = lst->fig.sq.c;
	sq.fig.sq.side = lst->fig.sq.side;
	cl_inter = INFINITY;
	while (i < 6)
	{
		sq.fig.sq.c = vadd(center, scal_x_vec(lst->fig.sq.side / 2, normal[i]));
		sq.normal = normal[i];
		inter = square_intersection(o, d, &sq);
		if (inter > EPSILON && inter < cl_inter)
		{
			cl_inter = inter;
			cl_normal = sq.normal;
		}
		i++;
	}
	lst->normal = cl_normal;
	return (cl_inter);
}

double		pyramid_intersection(t_p3 o, t_p3 d, t_figures *lst)
{
	int			i;
	t_figures	sq;
	t_figures	tr;
	double		cl_inter;
	t_p3		cl_normal;
	t_p3		corner[4];
	double		inter;
	t_p3		tr_center;
	t_p3		normal[5] = { {1,0,0},
							{0,0,1},
							{-1,0,0},
							{0,0,-1},
   							{1,0,0}	};

	cl_inter = INFINITY;
	tr_center = vadd(lst->fig.sq.c, scal_x_vec(lst->fig.sq.side, (t_p3){0, 1, 0}));
	sq.fig.sq.side = lst->fig.sq.side;
	sq.fig.sq.c = lst->fig.sq.c;
	sq.normal = (t_p3) {0, -1, 0};
	/*inter = square_intersection(o, d, &sq);
	if (inter > EPSILON && inter < cl_inter)
	{
		cl_inter = inter;
		cl_normal = (t_p3){0,1,0};
	}*/
	i = 0;
	while (i < 4)
	{
		corner[i] = vadd(lst->fig.sq.c, scal_x_vec(lst->fig.sq.side / 2, normal[i]));
		corner[i] = vadd(corner[i], scal_x_vec(lst->fig.sq.side / 2, normal[i + 1]));
		i++;
	}
	
	

	i = 0;
	tr.fig.tr.p3 = tr_center;
	while (i < 4)
	{
		tr.fig.tr.p1 = corner[i];
		tr.fig.tr.p2 = i < 3 ? corner[i + 1] : corner[0];
		tr.normal = cross(vsubstract(tr.fig.tr.p3, tr.fig.tr.p1), vsubstract(tr.fig.tr.p2, tr.fig.tr.p1));
		inter = triangle_intersection(o, d, &tr);
		if (inter > EPSILON && inter < cl_inter)
		{
			cl_inter = inter;;
			cl_normal = tr.normal;
		}
		i++;
	}
	lst->normal = cl_normal;
	return (cl_inter);
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

double		cy_intersection(t_p3 o, t_p3 d, t_p3 *normal, t_figures *lst)
{
	double	x2[2];

	if (solve_cylinder(x2, o, d, lst) == 0)
		return (INFINITY);
	lst->fig.cy.dist1 = dot(lst->fig.cy.nv, vsubstract(scal_x_vec(x2[0], d),
												vsubstract(lst->fig.cy.c, o)));
	lst->fig.cy.dist2 = dot(lst->fig.cy.nv, vsubstract(scal_x_vec(x2[1], d),
												vsubstract(lst->fig.cy.c, o)));
	if (!((lst->fig.cy.dist1 >= 0 && lst->fig.cy.dist1 <= lst->fig.cy.h
					&& x2[0] > EPSILON) || (lst->fig.cy.dist2 >= 0
					&& lst->fig.cy.dist2 <= lst->fig.cy.h && x2[0] > EPSILON)))
		return (INFINITY);
	*normal = calc_cy_normal(x2, o, d, lst);
	return (x2[0]);
}

double		cylinder_intersection(t_p3 o, t_p3 d, t_figures *lst)
{
	double	cylinder_inter;
	double	caps_inter;
	t_p3	cy_normal;

	cylinder_inter = cy_intersection(o, d, &cy_normal, lst);
	caps_inter = caps_intersection(o, d, lst);
	//caps_inter = INFINITY;
	if (cylinder_inter < INFINITY || caps_inter < INFINITY)
	{
		if (cylinder_inter < caps_inter)
		{
			lst->normal = cy_normal;
			return (cylinder_inter);
		}
		lst->normal = lst->fig.cy.nv;
		return (caps_inter);
	}
	return (INFINITY);
}


