/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_operations2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgarcia- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/10 17:46:49 by mgarcia-          #+#    #+#             */
/*   Updated: 2020/03/10 18:06:51 by mgarcia-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

double		mod(t_p3 v)
{
	return (sqrt(dot(v, v)));
}

t_p3		normalize(t_p3 p)
{
	t_p3	nv;
	double	mod;

	mod = sqrt(p.x * p.x + p.y * p.y + p.z * p.z);
	nv.x = p.x / mod;
	nv.y = p.y / mod;
	nv.z = p.z / mod;
	return (nv);
}

double		vec_cos(t_p3 a, t_p3 b)
{
	return (dot(a, b) / (mod(a) * mod(b)));
}

t_p3		scal_x_vec(double n, t_p3 p)
{
	t_p3	v;

	v.x = n * p.x;
	v.y = n * p.y;
	v.z = n * p.z;
	return (v);
}

double		distance(t_p3 p1, t_p3 p2)
{
	double d;

	d = sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2) + pow(p2.z - p1.z, 2));
	return (d);
}

int			p_is_outside(t_p3 p1, t_p3 p2, t_p3 p3, t_p3 ip)
{
	t_p3	v1;
	t_p3	v2;
	t_p3	vp;

	v1 = substract_vectors(p2, p1);
	v2 = substract_vectors(p3, p1);
	vp = substract_vectors(ip, p1);
	if (vec_cos(cross_product(v1, v2), cross_product(v1, vp)) < 0)
		return (1);
	return (0);
}
