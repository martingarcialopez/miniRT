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

#include "miniRT.h"

t_p3		set_camera(int n, int i, int j, t_minilibx mlx, int xres, int yres)
{
	double	img_asp_ratio;
	double	correct_fov;
	double	x_offset;
	double	y_offset;
	t_p3	p;

	x_offset = ((n % 3) * 0.5);
	y_offset = ((n / 3) * 0.5);
	img_asp_ratio = (double)xres / (double)yres;
	correct_fov = tan((mlx.cam->fov * M_PI / 180) / 2);
	p.x = ((2 * ((i + x_offset) / xres)) - 1) * img_asp_ratio * correct_fov;
	p.y = (1 - (2 * ((j + y_offset) / yres))) * correct_fov;
	p.z = -1;
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
		x_axis = vdefine(1, 0, 0);
	else
		x_axis = cross(tmp, z_axis);
	y_axis = cross(z_axis, x_axis);
	rotated.x = d.x * x_axis.x + d.y * x_axis.y + d.z * x_axis.z;
	rotated.y = d.x * y_axis.x + d.y * y_axis.y + d.z * y_axis.z;
	rotated.z = d.x * z_axis.x + d.y * z_axis.y + d.z * z_axis.z;
	rotated.z *= -1;
	return (rotated);
}

void	try_all_intersections(t_p3 o, t_p3 d, double min, t_figures *lst, t_figures *closest_figure, double
		*closest_intersection, double (*fun_ptr[NUM_FIGS])(t_p3, t_p3, t_figures *))
{
	double	intersection_distance;

	while (lst)
	{
		intersection_distance = (fun_ptr[lst->flag])(o, d, lst);
		if (intersection_distance > min && intersection_distance < *closest_intersection)
		{
			*closest_figure = *lst;
			*closest_intersection = intersection_distance;
		}
		lst = lst->next;
	}
}

int		apply_texture(int texture, t_inter inter)
{
		int black = 0x000000;
		int	white = 0xffffff;
		int	x = abs((int)floor(coords.x));
		int	y = abs((int)floor(coords.y));
		int z = abs((int)floor(coords.z));
		int	a;
		int	b;
		int	c;
		int	party_mix;

		a = x % 2;
		b = y % 2;
		c = z % 2;

		party_mix = (a ^ b) ^ c;	

		return (party_mix ? black : white);
}

int	trace_ray(t_p3 o, t_p3 d, double min, t_scene data, t_figures *lst, int depth)
{
	double		(*fun_ptr[NUM_FIGS])(t_p3, t_p3, t_figures *) = {
				&sphere_intersection,
				&plane_intersection,
				&square_intersection,
				&triangle_intersection,
				&cylinder_intersection };
	
	t_figures	closest_figure;
	double		closest_intersection;
	t_p3		normal;
	t_p3		intersection_point;
	int			local_color;
	t_p3		reflected_vec;
	int			reflected_color;
	double		r;

	local_color = reflected_color = 0x000000;
	data.background = 0x202020;
	closest_intersection = INFINITY;
	closest_figure.flag = -1;
	try_all_intersections(o, d, min, lst, &closest_figure, &closest_intersection, fun_ptr);
//	local_color = closest_figure.flag != -1 ? closest_figure.color : data.background;
	intersection_point = vadd(o, scal_x_vec(closest_intersection, d));
	local_color = closest_figure.flag != -1 ? closest_figure.texture == 1 ? apply_texture(intersection_point) : closest_figure.color : data.background;
	calc_normal(intersection_point, d, &normal, closest_figure);
	compute_light(o, &local_color, intersection_point, normal, data, lst, fun_ptr);

	r = closest_figure.refl_idx;	
	if (depth <= 0 || r <= 0)
		return (local_color);

	reflected_vec = reflect_ray(scal_x_vec(-1, d), normal);
	reflected_color = trace_ray(intersection_point, reflected_vec, 0.001, data, lst, depth - 1);

	return (add_colors(color_product(local_color, 1 - r), color_product(reflected_color, r)));
}

int		calc_ray(int n, int i, int j, int xres, int yres, t_minilibx mlx, t_scene data, t_figures *lst)
{
	t_p3	d;
	int		color;

	d = set_camera(n, i, j, mlx, xres, yres);
	d = look_at(d, mlx.cam->nv);
	color = trace_ray(mlx.cam->o, d, 0.0, data, lst, 2);	
	return (color);
}

int		supersample(int limit, int i, int j, int xres, int yres, t_minilibx mlx, t_scene data, t_figures
		*lst, int color[4])
{
	int		center;
	int		subsquare[4];
	int		ss_color[3] = { 0, 0, 0 };
	int		mask = 255;
	int		n;

	center = calc_ray(4, i, j, xres, yres, mlx, data, lst);
	if (!color_difference(color[0], center) || limit == 0)
		color[0] = average(color[0], center);
	else
	{
		subsquare[0] = color[0];
		subsquare[1] = calc_ray(1, i, j, xres, yres, mlx, data, lst);
		subsquare[2] = calc_ray(3, i, j, xres, yres, mlx, data, lst);
		subsquare[3] = center;
		color[0] = supersample(limit - 1, (i * 2) - 1, (j * 2) - 1, xres * 2, yres * 2, mlx, data, lst, subsquare);
	}
	if (!color_difference(color[1], center) || limit == 0)
		color[1] = average(color[1], center);
	else
	{
		subsquare[0] = calc_ray(3, i, j, xres, yres, mlx, data, lst);
		subsquare[1] = color[1];
		subsquare[2] = center;
		subsquare[3] = calc_ray(5, i, j, xres, yres, mlx, data, lst);
		color[1] = supersample(limit - 1, i * 2, (j * 2) - 1, xres * 2, yres * 2, mlx, data, lst, subsquare);
	}
	if (!color_difference(color[2], center) || limit == 0)
		color[2] = average(color[2], center);
	else
	{
		subsquare[0] = calc_ray(3, i, j, xres, yres, mlx, data, lst);
		subsquare[1] = center;
		subsquare[2] = color[2];
		subsquare[3] = calc_ray(7, i, j, xres, yres, mlx, data, lst);
		color[2] = supersample(limit - 1, (i * 2) - 1, j * 2, xres * 2, yres * 2, mlx, data, lst, subsquare);
	}

	if (!color_difference(color[3], center) || limit == 0)
		color[3] = average(color[3], center);
	else
	{
		subsquare[0] = center;
		subsquare[1] = calc_ray(5, i, j, xres, yres, mlx, data, lst);
		subsquare[2] = calc_ray(7, i, j, xres, yres, mlx, data, lst);
		subsquare[3] = color[3];
		color[3] = supersample(limit - 1, i * 2, j * 2, xres * 2, yres * 2, mlx, data, lst, subsquare);
	}

	n = 0;
	while (n < 4)
	{
		ss_color[0] += (color[n] & (mask << 16)) >> 16;
		ss_color[1] += (color[n] & (mask << 8)) >> 8;
		ss_color[2]	+= color[n] & mask;
		n++;
	}
	ss_color[0] = ss_color[0] / 4;
	ss_color[1] = ss_color[1] / 4;
	ss_color[2] = ss_color[2] / 4;
	return ((ss_color[0] << 16) | (ss_color[1] << 8) | ss_color[2]);	
}

int		calc_supersampled_color(int i, int j, int *edge_color, t_minilibx mlx, t_scene data,
		t_figures *lst, int *last, int *last_row1, int tid)
{
	t_p3	d;
	int		color[4];
	int		color2[4];
	int		ss_color[3] = {0, 0, 0};
	int		mask = 255;
	int		tl0 = data.yres / NUM_THREADS * tid; // thread line 0
	int		n;
	n = 0;
	/*
	while (n < 4)
	{
		d = set_camera(n, i, j, mlx, data.xres, data.yres);
		d = look_at(d, mlx.cam->nv);
		color2[n] = trace_ray(mlx.cam->o, d, 0.0, data, lst, 3);
		n++;
	}
	*/
	if (j == tl0)
	{
		if (i == 0)
		{
			color[0] = calc_ray(0, i, j, data.xres, data.yres, mlx, data, lst);
			color[1] = *last_row1 = calc_ray(2, i, j, data.xres, data.yres, mlx, data, lst);
			color[2] = edge_color[0] = calc_ray(6, i, j, data.xres, data.yres, mlx, data, lst);
			color[3] = *last = calc_ray(8, i, j, data.xres, data.yres, mlx, data, lst);
		}
		else if (i == data.xres - 1)
		{
			color[0] = *last_row1;
			color[1] = calc_ray(2, i, j, data.xres, data.yres, mlx, data, lst);
			color[2] = edge_color[i] = *last;
			color[3] = edge_color[i + 1] = calc_ray(8, i, j, data.xres, data.yres, mlx, data, lst);
		}
		else
		{
			color[0] = *last_row1;
			color[1] = *last_row1 = calc_ray(2, i, j, data.xres, data.yres, mlx, data, lst);
			color[2] = edge_color[i] = *last;
			color[3] = *last = calc_ray(8, i, j, data.xres, data.yres, mlx, data, lst);
		}
	}
	else
	{
		if (i == 0)
		{
			color[0] = edge_color[0];
			color[1] = edge_color[1];
			color[2] = edge_color[0] = calc_ray(6, i, j, data.xres, data.yres, mlx, data, lst);
			color[3] = *last = calc_ray(8, i, j, data.xres, data.yres, mlx, data, lst);
		}
		else if (i == data.xres - 1)
		{
			color[0] = edge_color[i];
			color[1] = edge_color[i + 1];
			color[2] = edge_color[i] = *last;
			color[3] = edge_color[i + 1] = calc_ray(3, i, j, data.xres, data.yres, mlx, data, lst);
		}
		else
		{
			color[0] = edge_color[i];
			color[1] = edge_color[i + 1];
			color[2] = edge_color[i] = *last;
			color[3] = *last = calc_ray(4, i, j, data.xres, data.yres, mlx, data, lst);
		}
	}

	if (color_difference(color[0], color[3]) || color_difference(color[1], color[2])) 
		return (supersample(2, i, j, data.xres, data.yres, mlx, data, lst, color));

	n = 0;
	while (n < 4)
	{
		ss_color[0] += (color[n] & (mask << 16)) >> 16;
		ss_color[1] += (color[n] & (mask << 8)) >> 8;
		ss_color[2]	+= color[n] & mask;
		n++;
	}
	ss_color[0] = ss_color[0] / 4;
	ss_color[1] = ss_color[1] / 4;
	ss_color[2] = ss_color[2] / 4;
	return ((ss_color[0] << 16) | (ss_color[1] << 8) | ss_color[2]);	
}

void	render_scene(t_minilibx mlx, t_scene data, t_figures *lst, int tid)
{
	//t_p3	d;
	int		n = data.yres / NUM_THREADS;
	int		i;
	int		j;
	int		supersampled_color;
	int		edge_color[data.xres + 2];
	int		last;
	int		last_row1;

	j = n * tid;
	while (j < (n * (tid + 1))/*j < data.yres*/)
	{
		//i = tid;
		i = 0;
		while (i < data.xres)
		{
			supersampled_color = calc_supersampled_color(i, j, edge_color, mlx, data, lst, &last,
					&last_row1, tid);
			mlx.cam->px_img[j * data.xres + i] = supersampled_color;
			//i += NUM_THREADS;
			i++;
		}
		j++;
	}
}

void		init_mlx(t_minilibx *mlx, t_scene data)
{
	t_camera	*cam_begin;

	mlx->mlx_ptr = mlx_init();
	mlx->win_ptr = mlx_new_window(mlx->mlx_ptr, data.xres, data.yres, "basic miniRT");
	cam_begin = mlx->cam;
	mlx->begin = mlx->cam;
	while (mlx->cam)
	{
		mlx->cam->img_ptr = mlx_new_image(mlx->mlx_ptr, data.xres, data.yres);
		mlx->cam->px_img = (int *)mlx_get_data_addr(mlx->cam->img_ptr,
				&mlx->cam->bits_per_pixel, &mlx->cam->size_line, &mlx->cam->endian);
		mlx->cam = mlx->cam->next;
	}
	mlx->cam = cam_begin;
}

void		*render_thread(void *ptr)
{
	t_wrapper	*w;

	w = (t_wrapper *)ptr;
	while (w->mlx.cam)
	{
		render_scene(w->mlx, w->data, w->lst, w->tid);
		w->mlx.cam = w->mlx.cam->next;
	}
	return (NULL);
}

void			wrapp_data(t_minilibx mlx, t_scene data, t_figures *lst, t_wrapper *wrapper)
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

int			main(int ac, char **av)
{
	pthread_t	threads[NUM_THREADS];
	t_wrapper	wrapper[NUM_THREADS];
	t_minilibx	mlx;
	t_scene		data;
	t_figures	*lst;
	int			i;


	if (ac < 2)
		usage(av[0]);
	
	parse_scene(&mlx, &data, &lst, av);
	init_mlx(&mlx, data);
	wrapp_data(mlx, data, lst, wrapper);

	i = 0;
	while (i < NUM_THREADS)
	{
		pthread_create(&threads[i], NULL, render_thread, &wrapper[i]);
		i++;
	}
	i = 0;
	while (i < NUM_THREADS)
		pthread_join(threads[i++], NULL);
	mlx.cam = mlx.begin;
	if (ac == 3)
	{
		do_the_bmp_thing(mlx, data, av[1]);
		exit(0);
	}
	mlx_put_image_to_window (mlx.mlx_ptr, mlx.win_ptr, mlx.cam->img_ptr, 0, 0);
	mlx_hook(mlx.win_ptr, 17, 1L << 17, ft_close, (void *)0);
	mlx_hook(mlx.win_ptr, 2, 0, next_cam, &mlx);
	mlx_loop(mlx.mlx_ptr);
	return (0);
}
