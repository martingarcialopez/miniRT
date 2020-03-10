/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgarcia- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/20 08:42:41 by mgarcia-          #+#    #+#             */
/*   Updated: 2020/03/10 18:11:25 by mgarcia-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _MINIRT_H_
# define _MINIRT_H_

#include <OpenGL/gl3.h>
#include "mlx.h"
#include "libft.h"
#include "figures.h"
#include <fcntl.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

#include <unistd.h>
#include <stdio.h>

#define BUFSIZE	32

#define PI 3.14159265

#define SP (1 << 0)
#define PL (1 << 1)
#define SQ (1 << 2)
#define CY (1 << 3)
#define TR (1 << 4)

typedef struct		s_camera
{
    t_p3			o;
    t_p3			vec;
    int				fov;
    void			*mlx_ptr;
	void			*win_ptr;
	void			*img_ptr;
    int				*px_img;
	struct s_camera	*begin;
    struct s_camera	*next;
}			t_camera;

typedef struct		s_light
{
	t_p3			o;
	double			br;
	int				color;
	struct s_light	*next;
}					t_light;

typedef struct		s_scene
{
	int				xres;
	int				yres;
	t_camera		*cam;
	t_light			*l;
	double			ambient_light;
	int				al_color;
	int				background;
}					t_scene;

t_p3			normalize(t_p3 p);

typedef struct		s_figures
{
	int				flag;
	union figures	fig;
	int				color;
	struct s_figures*next;
}					t_figures;

typedef struct		s_minilibx
{
	void			*mlx_ptr;
	void			*win_ptr;
	void			*img_ptr;
	int				bits_per_pixel;
	int				size_line;
	int				endian;
	int				*pixel_tab;
}					t_minilibx;

char			*readfile(char *str, int fd);

int				stoi(char **str);

double			stof(char **str);

void			ft_addnewlst_back(t_figures **alst, t_figures **begin);

double 			dot(t_p3 a, t_p3 b);

double			mod(t_p3 v);

t_p3			scal_x_vec(double n, t_p3 p);

t_p3			add_vectors(t_p3 a, t_p3 b);

t_p3			substract_vectors(t_p3 a, t_p3 b);

void			parse_scene(t_scene *data, t_figures **lst, int ac, char **av);

double			sphere_intersection(t_p3 O, t_p3 d, t_figures *lst);

double			plane_intersection(t_p3 O, t_p3 d, t_p3 plane_p, t_p3 plane_nv);

void			compute_light(int *color, t_p3 p, t_p3 normal, t_scene data, t_figures *lst);

int 			color_x_light(int color, double rgb[3]);

t_p3			calc_normal(t_p3 p, t_figures lst);

double			distance(t_p3 p1, t_p3 p2);

int				is_lit(t_p3 O, t_p3 d, t_figures *lst);

t_p3			cross_product(t_p3 a, t_p3 b);

double			vec_cos(t_p3 a, t_p3 b);

t_p3			define_vect(double x, double y, double z);

double			square_intersection(t_p3 o, t_p3 d, t_figures *lst);

double			triangle_intersection(t_p3 o, t_p3 d, t_figures *lst);

double			cylinder_intersection(t_p3 o, t_p3 d ,t_figures *lst);

int				p_is_outside(t_p3 p1, t_p3 p2, t_p3 p3, t_p3 ip);

int				next_cam(int keycode, t_scene *data);

int				ft_close(void *param);

#endif
