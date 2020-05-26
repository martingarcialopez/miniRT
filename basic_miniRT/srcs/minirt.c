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

t_p3	set_camera(int i, int j, t_minilibx mlx, t_scene data)
{
	double	img_asp_ratio;
	double	correct_fov;
	t_p3	p;

	img_asp_ratio = (double)data.xres / (double)data.yres;
	correct_fov = tan((mlx.cam->fov * M_PI / 180) / 2);
	p.x = ((2 * ((i + 0.5) / data.xres)) - 1) * img_asp_ratio * correct_fov;
	p.y = (1 - (2 * ((j + 0.5) / data.yres))) * correct_fov;
	p.x *= -1;
	p.z = 1;
	normalize(p);
	return (p);
}

t_p3	look_at(t_p3 d, t_p3 cam_nv)
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

int		trace_ray(t_p3 o, t_p3 d, t_scene data, t_figures *lst)
{
	t_v3		ray;
	t_figures	closest_figure;
	double		closest_intersection;
	t_inter		inter;

	ray.o = o;
	ray.d = d;
	inter.color = 0x000000;
	data.bgr = 0x202020;
	closest_intersection = INFINITY;
	closest_figure.flag = -1;
	try_all_intersections(ray, lst, &closest_figure, &closest_intersection);
	inter.color = closest_figure.flag != -1 ? closest_figure.color : data.bgr;
	inter.p = vadd(o, scal_x_vec(closest_intersection, d));
	calc_normal(inter.p, d, &(inter.normal), closest_figure);
	compute_light(&inter, data, lst);
	return (inter.color);
}

void	render_scene(t_minilibx mlx, t_scene data, t_figures *lst)
{
	t_p3	d;
	int		i;
	int		j;
	int		color;

	j = 0;
	while (j < data.yres)
	{
		i = 0;
		while (i < data.xres)
		{
			d = set_camera(i, j, mlx, data);
			d = look_at(d, mlx.cam->nv);
			color = trace_ray(mlx.cam->o, d, data, lst);
			mlx.cam->px_img[j * (mlx.cam->size_line / 4) + i] = color;
			i++;
		}
		ft_printf("\rRendering scene... (cam %d/%d) [%d%%]",
							mlx.cam->idx, data.cam_nb, 100 * j / data.yres);
		j++;
	}
	ft_printf("\rRendering scene... (cam %d/%d) [100%%]\n",
												mlx.cam->idx, data.cam_nb);
}

int		main(int ac, char **av)
{
	t_minilibx	mlx;
	t_scene		data;
	t_figures	*lst;

	if (ac < 2 || ac > 3)
		usage(av[0]);
	if (ac == 3 && ft_strcmp(av[2], "--save"))
		scene_error("invalid argument\n");
	parse_scene(&mlx, &data, &lst, av);
	init_mlx(&mlx, &data);
	while (mlx.cam)
	{
		render_scene(mlx, data, lst);
		mlx.cam = mlx.cam->next;
	}
	mlx.cam = mlx.begin;
	if (ac == 3)
	{
		do_the_bmp_thing(mlx, data, av[1]);
		success_message(ac);
		exit(EXIT_SUCCESS);
	}
	success_message(ac);
	graphic_loop(mlx);
	return (0);
}
