/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgarcia- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/20 08:39:55 by mgarcia-          #+#    #+#             */
/*   Updated: 2020/03/09 11:28:49 by mgarcia-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

t_p3		define_vector(int i, int j, t_scene data)
{
	double	ImgAspRatio;
	double	correct_fov;
	t_p3	p;

	ImgAspRatio = (double)data.xres / (double)data.yres;
	correct_fov = tan((data.cam->fov * PI / 180) / 2);
	p.x = ((2 * ((i + 0.5) / data.xres)) - 1) * ImgAspRatio * correct_fov;
	p.y = (1 - (2 * ((j + 0.5) / data.yres))) * correct_fov;
	p.z = -1;
	//normalize(p);
	return (p);
}

t_p3	LookAt(t_p3 d, t_p3 cam_nv, t_p3 cam)
{
	t_p3	x_axis;
	t_p3	y_axis;
	t_p3	z_axis;
	t_p3	tmp;
	t_p3	rotated;

	tmp.x = 0;
	tmp.y = 1;
	tmp.z = 0;

	z_axis = normalize(cam_nv);
	//printf("z_axis iss %f, %f, %f\n", z_axis.x, z_axis.y, z_axis.z);
	x_axis = cross_product(tmp, z_axis);
	//printf("x_axis iss %d, %d, %d\n", x_axis.x, x_axis.y, x_axis.z);
	y_axis = cross_product(z_axis, x_axis);
	//printf("y_axis iss %f, %f, %f\n", y_axis.x, y_axis.y, y_axis.z);

	rotated.x = d.x * x_axis.x + d.y * x_axis.y + d.z * x_axis.z;
	rotated.y = d.x * y_axis.x + d.y * y_axis.y + d.z * y_axis.z;
	rotated.z = d.x * z_axis.x + d.y * z_axis.y + d.z * z_axis.z;
	rotated.z *= -1;
	return (rotated);
}


void	try_all_intersections(t_p3 O, t_p3 d, double min, double max, t_figures *lst, t_figures *closest_figure, double *closest_intersection)
{
	double	intersection_distance;

	while (lst)
	{
		if (lst->flag & SP)
			intersection_distance = sphere_intersection(O, d, lst);
		else if (lst->flag & PL)
			intersection_distance = plane_intersection(O, d, lst);
	/*	else if (lst->flag & SQ)
			intersection_distance = square_intersection;
		else if (lst->flag & CY)
			intersection_distance = cylinder_intersection;
		else if (lst->flag & TR)
			intersection_distance = triangle_intersection;*/
		if (intersection_distance > min && intersection_distance < *closest_intersection)
		{
			*closest_figure = *lst;
			*closest_intersection = intersection_distance;
		}
		lst = lst->next;
	}
}

int	trace_ray(t_p3 O, t_p3 d, double min, t_scene data, t_figures *lst)
{
	int		color;
	t_figures	closest_figure;
	double	closest_intersection;
	t_p3	normal;
	t_p3	intersection_point;

	data.background = 0x000000;
	color = 0x000000;

	closest_intersection = INFINITY;
	closest_figure.flag = 0;
	try_all_intersections(O, d, min, INFINITY, lst, &closest_figure, &closest_intersection);
	color = closest_figure.flag != 0 ? closest_figure.color : data.background;
	intersection_point = add_vectors(O, scal_x_vec(closest_intersection, normalize(d)));
	normal = calc_normal(intersection_point, closest_figure);
	color = color_x_light(color, compute_light(intersection_point, normal, data, lst));
    return (color);

}

void	render_scene(t_scene data, t_figures *lst)
{
	t_p3		d;
	int 		i;
	int 		j;
	int		color;

	j = 0;
	while (j < data.yres)
	{
		i = 0;
		while (i < data.xres)
		{
			d = define_vector(i, j, data);
			//printf("for x %d and y %d the vector d is %f, %f, %f\n", i, j, d.x, d.y, d.z);
			d = LookAt(d, data.cam->vec, data.cam->o);	
			//puts("After LookAt function");
			//printf("for x %d and y %d the vector d is %f, %f, %f\n", i, j, d.x, d.y, d.z);
			//puts("\n");
			color = trace_ray(data.cam->o, d, 0, data, lst);
			
			data.cam->px_img[j * data.xres + i] = color;
			i++;
		}
		j++;
	}
}

void		init_mlx(t_minilibx *mlx, t_scene *data)
{
	t_camera	*cam_begin;

	mlx->mlx_ptr = mlx_init();
	mlx->win_ptr = mlx_new_window(mlx->mlx_ptr, data->xres, data->yres, "basic miniRT");
	cam_begin = data->cam;
	while (data->cam)
	{
		data->cam->mlx_ptr = mlx->mlx_ptr;
		data->cam->win_ptr = mlx->win_ptr;
		data->cam->img_ptr = mlx_new_image(mlx->mlx_ptr, data->xres, data->yres);
		data->cam->px_img = (int *)mlx_get_data_addr(data->cam->img_ptr, &mlx->bits_per_pixel, &mlx->size_line, &mlx->endian);
		data->cam->begin = cam_begin;
		data->cam = data->cam->next;
	}
	data->cam = cam_begin;
}

int			next_cam(int keycode, t_scene *data)
{
	if (keycode != 49)
		return (1);
	if (data->cam->next)
	{
		data->cam = data->cam->next;
		mlx_put_image_to_window(data->cam->mlx_ptr, data->cam->win_ptr, data->cam->img_ptr, 0, 0);
	}
	else
	{
		data->cam = data->cam->begin;
		mlx_put_image_to_window(data->cam->mlx_ptr, data->cam->win_ptr, data->cam->img_ptr, 0, 0);
	}
	return (1);
}

int			main(int ac, char **av)
{
	t_minilibx	mlx;
	t_scene		data;
 	t_figures	*lst;
	
	parse_scene(&data, &lst, ac, av);
	init_mlx(&mlx, &data);
	while (data.cam->next)
	{
		render_scene(data, lst);
		data.cam = data.cam->next;
	}
	render_scene(data, lst);
	data.cam = data.cam->begin;

	mlx_put_image_to_window (mlx.mlx_ptr, mlx.win_ptr, data.cam->img_ptr, 0, 0);
	mlx_hook(mlx.win_ptr, 17, 1L << 17, exit, (void*)0);
	mlx_hook(mlx.win_ptr, 2, 0, next_cam, &data);
	mlx_loop(mlx.mlx_ptr);

	return (0);
}