/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   figures.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgarcia- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/21 10:48:18 by mgarcia-          #+#    #+#             */
/*   Updated: 2020/03/10 14:56:01 by mgarcia-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _FIGURES_H_
# define _FIGURES_H_

#include "miniRT.h"

typedef struct	s_sphere
{
	t_p3	c;
	double	r;
}				t_sphere;

typedef struct	s_plane
{
	t_p3	p;
}				t_plane;

typedef struct	s_square
{
	t_p3	c;
	double	side;

}				t_square;

typedef struct	s_cylinder
{
	t_p3	c;
	t_p3	nv;
	double	r;
	double	h;
	double	dist1;
	double	dist2;
}				t_cylinder;

typedef struct	s_triangle
{
	t_p3	p1;
	t_p3	p2;
	t_p3	p3;
}				t_triangle;

union 		figures
{
	t_sphere	sp;
	t_plane		pl;
	t_square	sq;
	t_cylinder	cy;
	t_triangle	tr;
};

/*
typedef struct	s_lst
{
	int				color;
	int				flag;
	union figures	fig;
	struct s_lst	*next;
}				t_lst;
*/

#endif
