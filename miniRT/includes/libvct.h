/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libvct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgarcia- <mgarcia-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/30 19:08:23 by mgarcia-          #+#    #+#             */
/*   Updated: 2020/05/30 19:08:28 by mgarcia-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBVCT_H
# define LIBVCT_H

# include <math.h>

typedef struct	s_p3
{
	double		x;
	double		y;
	double		z;
}				t_p3;

t_p3			vdefine(double x, double y, double z);

t_p3			vadd(t_p3 a, t_p3 b);

t_p3			vsubstract(t_p3 a, t_p3 b);

double			dot(t_p3 a, t_p3 b);

t_p3			cross(t_p3 a, t_p3 b);

double			mod(t_p3 v);

t_p3			normalize(t_p3 p);

double			vsin(t_p3 a, t_p3 b);

double			vcos(t_p3 a, t_p3 b);

t_p3			scal_x_vec(double n, t_p3 p);

double			distance(t_p3 p1, t_p3 p2);

#endif
