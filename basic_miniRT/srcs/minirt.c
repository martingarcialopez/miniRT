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
	p.z = -1;
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
		x_axis = vdefine(1, 0, 0);
	else
		x_axis = cross(tmp, z_axis);
	y_axis = cross(z_axis, x_axis);
	rotated.x = d.x * x_axis.x + d.y * x_axis.y + d.z * x_axis.z;
	rotated.y = d.x * y_axis.x + d.y * y_axis.y + d.z * y_axis.z;
	rotated.z = d.x * z_axis.x + d.y * z_axis.y + d.z * z_axis.z;
	rotated.x *= -1;
	rotated.z *= -1;
	return (rotated);
}

void	try_all_intersections(t_p3 o, t_p3 d, t_figures *lst, t_figures *closest_figure, double
		*closest_intersection, double (*fun_ptr[NUM_FIGS])(t_p3, t_p3, t_figures *))
{
	double	intersection_distance;

	while (lst)
	{
		intersection_distance = (fun_ptr[lst->flag])(o, d, lst);
		if (intersection_distance < *closest_intersection)
		{
			*closest_figure = *lst;
			*closest_intersection = intersection_distance;
		}
		lst = lst->next;
	}
}

int		trace_ray(t_p3 o, t_p3 d, t_scene data, t_figures *lst)
{
	double		(*fun_ptr[NUM_FIGS])(t_p3, t_p3, t_figures *) = {
				&sphere_intersection,
				&plane_intersection,
				&square_intersection,
				&triangle_intersection,
				&cylinder_intersection };
	
	t_figures	closest_figure;
	double		closest_intersection;
	t_inter		inter;

	inter.color = 0x000000;
	data.background = 0x202020;
	closest_intersection = INFINITY;
	closest_figure.flag = -1;
	try_all_intersections(o, d, lst, &closest_figure, &closest_intersection, fun_ptr);
	inter.color = closest_figure.flag != -1 ? closest_figure.color : data.background;
	inter.p = vadd(o, scal_x_vec(closest_intersection, d));
	calc_normal(inter.p, d, &(inter.normal), closest_figure);
	compute_light(&inter, data, lst, fun_ptr);
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
		if (j % 10 == 0 && mlx.cam->idx == 1)
			ft_printf( "\rRendering scene... [%d%%]", 100 * j / data.yres);
		else if ( j % 10 == 0) 
			ft_printf("\rRendering cam %d... [%d%%]",mlx.cam->idx, 100 * j / data.yres);
		j++;
	}
	if (mlx.cam->idx == 1)
		ft_printf("\rRendering scene... [100%%]\n");
	else
		ft_printf("\rRendering cam %d... [100%%]\n", mlx.cam->idx);
}

void	init_mlx(t_minilibx *mlx, t_scene *data)
{
	t_camera	*cam_begin;
	int			x_displayres;
	int			y_displayres;

	mlx->mlx_ptr = mlx_init();
	mlx_get_screen_size(mlx->mlx_ptr, &x_displayres, &y_displayres);
	data->xres = data->xres < x_displayres ? data->xres : x_displayres;
	data->yres = data->yres < y_displayres ? data->yres : y_displayres;
	mlx->win_ptr = mlx_new_window(mlx->mlx_ptr, data->xres, data->yres, "basic miniRT");
	cam_begin = mlx->cam;
	mlx->begin = mlx->cam;
	while (mlx->cam)
	{
		mlx->cam->img_ptr = mlx_new_image(mlx->mlx_ptr, data->xres, data->yres);
		mlx->cam->px_img = (int *)mlx_get_data_addr(mlx->cam->img_ptr,
				&mlx->cam->bits_per_pixel, &mlx->cam->size_line, &mlx->cam->endian);
		mlx->cam = mlx->cam->next;
	}
	mlx->cam = cam_begin;
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
		ft_printf("Scene successfully rendered and saved to bmp\n");
		exit(EXIT_SUCCESS);
	}
	ft_printf("Scene successfully rendered, press ESC at any moment to close the program.\nIf the scene has several cameras, press space to change between them\n");
	mlx_put_image_to_window (mlx.mlx_ptr, mlx.win_ptr, mlx.cam->img_ptr, 0, 0);
	mlx_hook(mlx.win_ptr, DESTROYNOTIFY, STRUCTURENOTIFYMASK, ft_close, (void *)0);
	mlx_hook(mlx.win_ptr, KEYPRESS, KEYPRESSMASK, next_cam, &mlx);
	mlx_loop(mlx.mlx_ptr);
	
	return (0);

}
