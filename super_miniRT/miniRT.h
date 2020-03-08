/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgarcia- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/20 08:42:41 by mgarcia-          #+#    #+#             */
/*   Updated: 2020/03/03 11:57:08 by mgarcia-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _MINIRT_H_
# define _MINIRT_H_

#include <OpenGL/gl3.h>
#include "mlx.h"
#include "figures.h"
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

#define PI 3.14159265

#define SP (1 << 0)
#define PL (1 << 1)
#define SQ (1 << 2)
#define CY (1 << 3)
#define TR (1 << 4)

#define	NUM_THREADS 1
#define TIMES_RES   1

/*
typedef struct	s_p3
{
	double	x;
	double	y;
	double	z;
}				t_p3;
*/
typedef struct	s_frame
{
	double	x;
	double	y;
	t_p3	ulcorner;
}				t_frame;

typedef struct	s_light
{
	t_p3	o;
	double	br;
	int		color;
	struct	s_light	*next;
}				t_light;

typedef struct	s_scene
{
	int		xres;
	int		yres;
	int		background;
	t_p3		O;
	t_p3		cam;
	t_p3		cam_nv;
	int		fov;
	t_frame		fr;
	double		al;
	int		acl;
	t_light		*l;
}				t_scn;

t_p3			normalize(t_p3 p);

typedef struct	s_lst
{
	int		flag;
	union figures	fig;
	int		color;
	int		specular;
	double		reflective;
	struct s_lst	*next;
}				t_lst;

typedef struct	s_thngs
{
	void		*mlx_ptr;
	void		*win_ptr;
	t_scn		data;
	t_lst		*lst;
	int			i;
	int			*pixel_tab;
}				t_thngs;

double 			dot(t_p3 a, t_p3 b);

double			mod(t_p3 v);

t_p3			scal_x_vec(double n, t_p3 p);

t_p3			vec_add(t_p3 a, t_p3 b);

t_p3			vec_substract(t_p3 a, t_p3 b);

void			parse_scene(t_scn *data, t_lst **lst, int ac, char **av);

double			sphere_intersection(t_p3 O, t_p3 d, t_lst *lst);

double			plane_intersection(t_p3 O, t_p3 d, t_lst *lst);

double			compute_light(t_p3 p, t_p3 normal, t_scn data, t_lst *lst);

int 			color_x_light(int color, double light);

t_p3			calc_normal(t_p3 p, t_lst lst);

double			distance(t_p3 p1, t_p3 p2);

int			is_lit(t_p3 O, t_p3 d, t_lst *lst);

t_p3			cross_product(t_p3 a, t_p3 b);

t_p3			reflect_ray(t_p3 ray, t_p3 normal);


#endif
