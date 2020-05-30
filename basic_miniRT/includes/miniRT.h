/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgarcia- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/20 08:42:41 by mgarcia-          #+#    #+#             */
/*   Updated: 2020/05/25 21:24:20 by mgarcia-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include "mlx.h"
# include "ggl_mlx_define.h"
# include "libft.h"
# include "libvct.h"
# include "figures.h"
# include <fcntl.h>
# include <stdlib.h>
# include <math.h>
# include <pthread.h>
# include <stdio.h>

# ifdef MACOS
#  define OS_NAME 1
# endif

# ifdef LINUX
#  define OS_NAME 2
# endif

# define BUFSIZE	32

# define SP 0
# define PL 1
# define SQ 2
# define TR 3
# define CY 4

# define EPSILON 0.00001
# define FILE_CREATE_FLAGS O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR

typedef struct		s_v3
{
	t_p3	o;
	t_p3	d;
}					t_v3;

typedef struct		s_sq
{
	t_p3	half_size;
	t_p3	floor;
	t_p3	center_to_ip;
}					t_sq;

typedef struct		s_camera
{
	int				init;
	int				idx;
	t_p3			o;
	t_p3			nv;
	int				fov;
	void			*img_ptr;
	int				*px_img;
	int				bits_per_pixel;
	int				size_line;
	int				endian;
	struct s_camera	*next;
}					t_camera;

typedef struct		s_light
{
	t_p3			o;
	double			br;
	int				color;
	struct s_light	*next;
}					t_light;

typedef struct		s_scene
{
	int				res_init;
	int				xres;
	int				yres;
	int				cam_nb;
	t_light			*l;
	int				al_init;
	double			ambient_light;
	int				al_color;
	int				bgr;
}					t_scene;

typedef struct		s_figures
{
	int				flag;
	union u_figures	fig;
	int				color;
	t_p3			normal;
	struct s_figures*next;
}					t_figures;

typedef struct		s_minilibx
{
	void			*mlx_ptr;
	void			*win_ptr;
	t_camera		*cam;
	t_camera		*begin;
}					t_minilibx;

typedef struct		s_inter
{
	int				color;
	t_p3			normal;
	t_p3			p;
}					t_inter;

typedef struct		s_bmp_header
{
	char			type[2];
	unsigned int	size;
	unsigned int	reserved;
	unsigned int	offset;
}					t_bmphead;

typedef struct		s_dib_header
{
	unsigned int	size;
	int				width;
	int				height;
	unsigned short	colplanes;
	unsigned short	bpp;
	unsigned int	compression;
	unsigned int	img_size;
	int				x_ppm;
	int				y_ppm;
	unsigned int	color_number;
	unsigned int	important_color;
}					t_dibhead;

/*
**			 	Parsing functions
*/

void				parse_scene(t_minilibx *mlx, t_scene *data, t_figures **lst,
																	char **av);

void				parse_res(t_scene *data, char **str);

void				parse_ambient_light(t_scene *data, char **str);

void				parse_camera(t_minilibx *mlx, t_scene *data, char **str);

void				parse_light(t_scene **data, char **str);

void				parse_sphere(t_figures **elem, char **str);

void				parse_plane(t_figures **elem, char **str);

void				parse_square(t_figures **elem, char **str);

void				parse_triangle(t_figures **elem, char **str);

void				parse_cylinder(t_figures **elem, char **str);

/*
**				Parsing help functions
*/

char				*readfile(char *str, int fd);

int					stoi(char **str);

double				stof(char **str);

void				in_range(double nb, double min, double max, char *function);

void				next(char **str);

void				comma(char **str);

t_p3				parse_p3(char **str);

int					parse_color(char **str);

void				ft_addnewlst_back(t_figures **alst);

/*
**				Intersection functions
*/
double				sphere_intersection(t_p3 o, t_p3 d, t_figures *lst);

double				pl_intersection(t_p3 o, t_p3 d, t_p3 plane_p,
														t_p3 plane_nv);

double				plane_intersection(t_p3 o, t_p3 d, t_figures *lst);

double				square_intersection(t_p3 o, t_p3 d, t_figures *lst);

double				triangle_intersection(t_p3 o, t_p3 d, t_figures *lst);

double				cylinder_intersection(t_p3 o, t_p3 d, t_figures *lst);

/*
**				Intersections help functions
*/

void				try_all_intersections(t_v3 ray, t_figures *lst,
											t_figures *clfig, double *clint);

int					p_is_outside(t_p3 p1, t_p3 p2, t_p3 p3, t_p3 ip);

int					solve_cylinder(double x[2], t_p3 o, t_p3 d, t_figures *l);

t_p3				calc_cy_normal(double x2[2], t_p3 o, t_p3 d, t_figures *l);

/*
**				Ray tracing help functions functions
*/

void				calc_normal(t_p3 p, t_p3 d, t_p3 *normal, t_figures lst);

int					is_lit(t_p3 o, t_p3 d, t_figures *lst);

void				compute_light(t_inter *inter, t_scene data, t_figures *l);

int					color_x_light(int color, double rgb[3]);

/*
**				Error handling functions and success message
*/

void				*ec_malloc(unsigned int size);

void				fatal(char *message);

void				scene_error(char *message);

void				usage(char *program_name);

void				success_message(int ac);

/*
**				Minilibx functions
*/

void				init_mlx(t_minilibx *mlx, t_scene *data);

void				graphic_loop(t_minilibx mlx);

int					next_cam(int keycode, t_minilibx *mlx);

int					close_program(void *param);

/*
**				Bmp exporter
*/

void				do_the_bmp_thing(t_minilibx mlx, t_scene data, char *name);

#endif
