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
	double	disc;
	t_p3	d_x_a;
	double	k1;
	double	k2;

	d_x_a = cross(d, lst->fig.cy.nv);
	k1 = dot(d_x_a, cross(vsubstract(lst->fig.cy.c, o), lst->fig.cy.nv));
	k2 = dot(d_x_a, d_x_a);
	disc = (k2 * pow(lst->fig.cy.r, 2)) - (dot(lst->fig.cy.nv, lst->fig.cy.nv)
						* pow(dot(vsubstract(lst->fig.cy.c, o), d_x_a), 2));
	x[0] = (k1 + sqrt(disc)) / k2;
	x[1] = (k1 - sqrt(disc)) / k2;
	if (disc < 0 || mod(d_x_a) == 0)
		return (0);
	return (1);
}

t_p3		calc_cy_normal(double x2[2], t_p3 o, t_p3 d, t_figures *lst)
{
	double	dist;
	double	x;

	if ((lst->fig.cy.dist1 > 0 && lst->fig.cy.dist1 < lst->fig.cy.h)
			&& (lst->fig.cy.dist2 > 0 && lst->fig.cy.dist2 < lst->fig.cy.h))
	{
		dist = x2[0] < x2[1] ? lst->fig.cy.dist1 : lst->fig.cy.dist2;
		x = x2[0] < x2[1] ? x2[0] : x2[1];
	}
	else if (lst->fig.cy.dist1 > 0 && lst->fig.cy.dist1 < lst->fig.cy.h)
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
