/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgarcia- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/20 08:39:55 by mgarcia-          #+#    #+#             */
/*   Updated: 2020/03/12 19:38:34 by mgarcia-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_p3		set_camera(int n, t_rss rss, t_minilibx mlx)
{
	double	img_asp_ratio;
	double	correct_fov;
	double	x_offset;
	double	y_offset;
	t_p3	p;

	x_offset = ((n % 3) * 0.5);
	y_offset = ((n / 3) * 0.5);
	img_asp_ratio = (double)(rss.xres) / (double)(rss.yres);
	correct_fov = tan((mlx.cam->fov * M_PI / 180) / 2);
	p.x =
	((2 * ((rss.i + x_offset) / rss.xres)) - 1) * img_asp_ratio * correct_fov;
	p.y = (1 - (2 * ((rss.j + y_offset) / rss.yres))) * correct_fov;
	p.x *= -1;
	p.z = 1;
	normalize(p);
	return (p);
}

t_p3		look_at(t_p3 d, t_p3 cam_nv)
{
	t_p3	x_axis;
	t_p3	y_axis;
	t_p3	z_axis;
	t_p3	tmp;
	t_p3	rotated;

	tmp = vdefine(0, 1, 0);
	z_axis = cam_nv;
	if (cam_nv.y == 1 || cam_nv.y == -1)
		x_axis = cam_nv.y == 1 ? vdefine(1, 0, 0) : vdefine(-1, 0, 0);
	else
		x_axis = cross(tmp, z_axis);
	y_axis = cross(z_axis, x_axis);
	rotated.x = d.x * x_axis.x + d.y * x_axis.y + d.z * x_axis.z;
	rotated.y = d.x * y_axis.x + d.y * y_axis.y + d.z * y_axis.z;
	rotated.z = d.x * z_axis.x + d.y * z_axis.y + d.z * z_axis.z;
	return (rotated);
}

int			apply_t(int texture, t_inter inter)
{
	int		black;
	int		white;
	t_p3	coords;
	t_p3	val;
	int		party_mix;

	texture += 1;
	black = 0x000000;
	white = 0xffffff;
	coords.x = abs((int)floor(inter.p.x));
	coords.y = abs((int)floor(inter.p.y));
	coords.z = abs((int)floor(inter.p.z));
	val.x = (int)coords.x % 2;
	val.y = (int)coords.y % 2;
	val.z = (int)coords.z % 2;
	party_mix = ((int)val.x ^ (int)val.y) ^ (int)val.z;
	return (party_mix ? black : white);
}

int			trace_ray(t_p3 o, t_p3 d, t_wrapper *w, int depth)
{
	t_v3		ray;
	t_figures	cl_fig;
	t_inter		inter;
	double		closest_intersection;
	double		r;

	ray.o = o;
	ray.d = d;
	inter.color = 0x000000;
	inter.ref_color = 0x000000;
	w->data.bgr = 0x202020;
	closest_intersection = INFINITY;
	cl_fig.flag = -1;
	try_all_intersections(ray, w->lst, &cl_fig, &closest_intersection);
	inter.p = vadd(o, scal_x_vec(closest_intersection, d));
	calc_normal(inter.p, d, &(inter.normal), cl_fig);
	inter.color = cl_fig.flag != -1 ? cl_fig.color : w->data.bgr;
	inter.color = cl_fig.texture ? apply_t(cl_fig.texture, inter) : inter.color;
	compute_light(ray, &inter, w->data, w->lst);
	r = cl_fig.flag != -1 ? cl_fig.refl_idx : 0;
	if (r <= 0 || depth <= 0)
		return (inter.color);
	inter.ref_vec = reflect_ray(scal_x_vec(-1, d), inter.normal);
	inter.ref_color = trace_ray(inter.p, inter.ref_vec, w, depth - 1);
	return (cadd(cproduct(inter.color, 1 - r), cproduct(inter.ref_color, r)));
}

int			calc_ray(int n, t_rss rss, t_wrapper *w)
{
	t_p3	d;
	int		color;

	d = set_camera(n, rss, w->mlx);
	d = look_at(d, w->mlx.cam->nv);
	color = trace_ray(w->mlx.cam->o, d, w, REFLECTION_LIMIT);
	return (color);
}

int			average_supersampled_color(int *color)
{
	int		ss_color[3];
	int		mask;
	int		n;

	ft_memset(ss_color, 0, sizeof(int) * 3);
	mask = 255;
	n = 0;
	while (n < 4)
	{
		ss_color[0] += (color[n] & (mask << 16)) >> 16;
		ss_color[1] += (color[n] & (mask << 8)) >> 8;
		ss_color[2] += color[n] & mask;
		n++;
	}
	ss_color[0] = ss_color[0] / 4;
	ss_color[1] = ss_color[1] / 4;
	ss_color[2] = ss_color[2] / 4;
	return ((ss_color[0] << 16) | (ss_color[1] << 8) | ss_color[2]);
}

int			supersample_first_corner(int *color, int center,
												t_rss rss, t_wrapper *w)
{
	t_rss	tmp;
	int		*subsquare;
	int		col;

	subsquare = (int *)ec_malloc(sizeof(int) * 4);
	subsquare[0] = color[0];
	subsquare[1] = calc_ray(1, rss, w);
	subsquare[2] = calc_ray(3, rss, w);
	subsquare[3] = center;
	tmp.limit = rss.limit - 1;
	tmp.i = rss.i * 2 - 1;
	tmp.j = rss.j * 2 - 1;
	tmp.xres = rss.xres * 2;
	tmp.yres = rss.yres * 2;
	col = supersample(subsquare, tmp, w);
	return (col);
}

int			supersample_second_corner(int *color, int center,
												t_rss rss, t_wrapper *w)
{
	t_rss	tmp;
	int		*subsquare;
	int		col;

	subsquare = (int *)ec_malloc(sizeof(int) * 4);
	subsquare[0] = calc_ray(3, rss, w);
	subsquare[1] = color[1];
	subsquare[2] = center;
	subsquare[3] = calc_ray(5, rss, w);
	tmp.limit = rss.limit - 1;
	tmp.i = rss.i * 2;
	tmp.j = rss.j * 2 - 1;
	tmp.xres = rss.xres * 2;
	tmp.yres = rss.yres * 2;
	col = supersample(subsquare, tmp, w);
	return (col);
}

int			supersample_third_corner(int *color, int center,
												t_rss rss, t_wrapper *w)
{
	t_rss	tmp;
	int		*subsquare;
	int		col;

	subsquare = (int *)ec_malloc(sizeof(int) * 4);
	subsquare[0] = calc_ray(3, rss, w);
	subsquare[1] = center;
	subsquare[2] = color[2];
	subsquare[3] = calc_ray(7, rss, w);
	tmp.limit = rss.limit - 1;
	tmp.i = rss.i * 2 - 1;
	tmp.j = rss.j * 2;
	tmp.xres = rss.xres * 2;
	tmp.yres = rss.yres * 2;
	col = supersample(subsquare, tmp, w);
	return (col);
}

int			supersample_fourth_corner(int *color, int center,
												t_rss rss, t_wrapper *w)
{
	t_rss	tmp;
	int		*subsquare;
	int		col;

	subsquare = (int *)ec_malloc(sizeof(int) * 4);
	subsquare[0] = center;
	subsquare[1] = calc_ray(5, rss, w);
	subsquare[2] = calc_ray(7, rss, w);
	subsquare[3] = color[3];
	tmp.limit = rss.limit - 1;
	tmp.i = rss.i * 2;
	tmp.j = rss.j * 2;
	tmp.xres = rss.xres * 2;
	tmp.yres = rss.yres * 2;
	col = supersample(subsquare, tmp, w);
	return (col);
}

int			supersample(int *color, t_rss rss, t_wrapper *w)
{
	int		center;
	int		n;

	center = calc_ray(4, rss, w);
	n = 0;
	while (n < 4)
	{
		if (!color_difference(color[n], center) || rss.limit == 0)
			color[n] = average(color[n], center);
		else
		{
			if (n == 0)
				color[0] = supersample_first_corner(color, center, rss, w);
			else if (n == 1)
				color[1] = supersample_second_corner(color, center, rss, w);
			else if (n == 2)
				color[2] = supersample_third_corner(color, center, rss, w);
			else
				color[3] = supersample_fourth_corner(color, center, rss, w);
		}
		n++;
	}
	return (average_supersampled_color(color));
}

int			*sample_first_column(int *edge_color, int last[2],
												t_rss rss, t_wrapper *w)
{
	int		*color;

	color = (int *)ec_malloc(sizeof(int) * 4);
	if (rss.j == w->data.yres / NUM_THREADS * w->tid)
	{
		color[0] = calc_ray(0, rss, w);
		color[1] = calc_ray(2, rss, w);
		color[2] = calc_ray(6, rss, w);
		color[3] = calc_ray(8, rss, w);
		last[0] = color[3];
		last[1] = color[1];
		edge_color[0] = color[2];
	}
	else
	{
		color[0] = edge_color[0];
		color[1] = edge_color[1];
		color[2] = calc_ray(6, rss, w);
		color[3] = calc_ray(8, rss, w);
		last[0] = color[3];
		edge_color[0] = color[2];
	}
	return (color);
}

int			*sample_last_column(int *edge_color, int last[2],
												t_rss rss, t_wrapper *w)
{
	int		*color;

	color = (int *)ec_malloc(sizeof(int) * 4);
	if (rss.j == w->data.yres / NUM_THREADS * w->tid)
	{
		color[0] = last[1];
		color[1] = calc_ray(2, rss, w);
		color[2] = last[0];
		color[3] = calc_ray(8, rss, w);
		edge_color[rss.i] = color[2];
		edge_color[rss.i + 1] = color[3];
	}
	else
	{
		color[0] = edge_color[rss.i];
		color[1] = edge_color[rss.i + 1];
		color[2] = last[0];
		color[3] = calc_ray(3, rss, w);
		edge_color[rss.i] = color[2];
		edge_color[rss.i + 1] = color[3];
	}
	return (color);
}

int			*sample_centered_pixel(int *edge_color, int last[2],
												t_rss rss, t_wrapper *w)
{
	int		*color;

	color = (int *)ec_malloc(sizeof(int) * 4);
	if (rss.j == w->data.yres / NUM_THREADS * w->tid)
	{
		color[0] = last[1];
		color[1] = calc_ray(2, rss, w);
		color[2] = last[0];
		color[3] = calc_ray(8, rss, w);
		last[0] = color[3];
		last[1] = color[1];
		edge_color[rss.i] = color[2];
	}
	else
	{
		color[0] = edge_color[rss.i];
		color[1] = edge_color[rss.i + 1];
		color[2] = last[0];
		color[3] = calc_ray(4, rss, w);
		last[0] = color[3];
		edge_color[rss.i] = color[2];
	}
	return (color);
}

int			*sample_pixel(int *edge_color, int last[2],
											t_rss rss, t_wrapper *w)
{
	int		*color;

	if (rss.i == 0)
		color = sample_first_column(edge_color, last, rss, w);
	else if (rss.i == w->data.xres - 1)
		color = sample_last_column(edge_color, last, rss, w);
	else
		color = sample_centered_pixel(edge_color, last, rss, w);
	return (color);
}

int			calc_pixel_color(int *edge_color, int last[2], t_wrapper *w)
{
	t_rss	rss;
	int		*color;

	rss.limit = 3;
	rss.xres = w->data.xres;
	rss.yres = w->data.yres;
	rss.i = w->i;
	rss.j = w->j;
	color = sample_pixel(edge_color, last, rss, w);
	if (color_difference(color[0], color[3])
		|| color_difference(color[1], color[2]))
		return (supersample(color, rss, w));
	return (average_supersampled_color(color));
}

void		render_scene(t_wrapper *w)
{
	int		edge_color[w->data.xres + 2];
	int		last[3];
	int		color;
	int		n;

	n = w->data.yres / NUM_THREADS;
	w->j = n * w->tid;
	while (w->j < (n * (w->tid + 1)))
	{
		w->i = 0;
		while (w->i < w->data.xres)
		{
			color = calc_pixel_color(edge_color, last, w);
			w->mlx.cam->px_img[w->j * w->data.xres + w->i] = color;
			w->i++;
		}
		if (w->tid == 0)
			ft_printf("\rRendering scene... (cam %d/%d) [%d%%]",
			w->mlx.cam->idx, w->data.cam_nb, 100 * w->j / (n * (w->tid + 1)));
		w->j++;
	}
	if (w->tid == 0)
		ft_printf("\rRendering scene... (cam %d/%d) [100%%]\n",
											w->mlx.cam->idx, w->data.cam_nb);
}

void		*render_thread(void *ptr)
{
	t_wrapper	*w;

	w = (t_wrapper *)ptr;
	while (w->mlx.cam)
	{
		render_scene(w);
		w->mlx.cam = w->mlx.cam->next;
	}
	return (NULL);
}

void		wrapp_data(
			t_minilibx mlx, t_scene data, t_figures *lst, t_wrapper *wrapper)
{
	int	i;

	i = 0;
	while (i < NUM_THREADS)
	{
		wrapper[i].mlx = mlx;
		wrapper[i].data = data;
		wrapper[i].lst = lst;
		wrapper[i].tid = i;
		i++;
	}
}

void		multithreaded_render(t_wrapper wrapper[NUM_THREADS])
{
	pthread_t	threads[NUM_THREADS];
	int			i;

	i = 0;
	while (i < NUM_THREADS)
	{
		pthread_create(&threads[i], NULL, render_thread, &wrapper[i]);
		i++;
	}
	i = 0;
	while (i < NUM_THREADS)
	{
		if (i > 0)
			ft_printf("waiting thread %d to finish...\n", i + 1);
		pthread_join(threads[i++], NULL);
	}
}

int			main(int ac, char **av)
{
	t_wrapper	wrapper[NUM_THREADS];
	t_minilibx	mlx;
	t_scene		data;
	t_figures	*lst;

	if (ac < 2 || ac > 3)
		usage(av[0]);
	if (ac == 3 && ft_strcmp(av[2], "--save"))
		scene_error("invalid argument\n");
	parse_scene(&mlx, &data, &lst, av);
	init_mlx(&mlx, &data);
	wrapp_data(mlx, data, lst, wrapper);
	multithreaded_render(wrapper);
	if (ac == 3)
		do_the_bmp_thing(mlx, data, av[1]);
	success_message(ac);
	graphic_loop(mlx);
	return (0);
}
