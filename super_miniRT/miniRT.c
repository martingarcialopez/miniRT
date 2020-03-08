/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgarcia- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/20 08:39:55 by mgarcia-          #+#    #+#             */
/*   Updated: 2020/03/03 13:35:03 by mgarcia-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void		set_camera(t_scn *data)
{
	double	fov_angle;

	fov_angle = ((double)(data->fov) / (double)2) * ((double)PI / (double)180);
	data->O.x = 0;
	data->O.y = 0;
	//data->O.z = (1 / tan(fov_angle)) - 1;
	data->O.z = 0;
	printf("original fov is %d, value of O.z is %f and fov_angle is %f\n", data->fov, data->O.z, fov_angle);
	
	/*data->fr.x = 1;
	data->fr.y = 1;
	data->fr.ulcorner.x = -0.5;
	data->fr.ulcorner.y = 0.5;
	data->fr.ulcorner.z = 1;*/

}

t_p3		define_vector(int i, int j, t_scn data)
{
	t_p3	p;
	double	pixel_to_frame_x;
	double	pixel_to_frame_y;

	//p.z = data->fr.ulcorner.z;
	//p.x = (xratio + 0.5/data->xres) * i + data->fr.ulcorner.x;
	//p.y = (yratio + 0.5/data->yres) * j - data->fr.ulcorner.y;

	//pixel_to_frame_x = (2.0 * (((double)i + 0.5) / (double)data->xres)) - 1.0;
	//pixel_to_frame_y = (2.0 * (((double)j + 0.5) / (double)data->yres)) - 1.0;

	p.x = ((2 * (((double)i + 0.5) / (double)data.xres)) - 1) * (double)data.xres/(double)data.yres;
	//p.x = (2.0 * pixel_to_frame_x - 1.0) * (double)data->xres / (double)data->yres;
	p.y = 1 - (2 * (((double)j + 0.5) / (double)data.yres));
	//p.y = 1.0 - 2.0 * pixel_to_frame_y;
	
	p.z = 1;

	p = vec_substract(p, data.O);

	//p = normalize(p);

	return (p);
}

t_p3	LookAt(t_p3 d, t_p3 cam_nv, t_p3 cam)
{
	//int		rotation[3][3] = { 0 };
	t_p3	x_axis = {1, 0, 0};
	t_p3	y_axis = {0, 1, 0};
	t_p3	z_axis = {0, 0 ,1};
	t_p3	tmp    = {0, 1, 0};
	t_p3	rotated;

	z_axis = normalize(cam_nv);
	//printf("z_axis iss %f, %f, %f\n", z_axis.x, z_axis.y, z_axis.z);
	x_axis = cross_product(tmp, z_axis);
	//printf("x_axis iss %d, %d, %d\n", x_axis.x, x_axis.y, x_axis.z);
	y_axis = cross_product(z_axis, x_axis);
	//printf("y_axis iss %f, %f, %f\n", y_axis.x, y_axis.y, y_axis.z);

	rotated.x = d.x * x_axis.x + d.y * x_axis.y + d.z * x_axis.z;
	rotated.y = d.x * y_axis.x + d.y * y_axis.y + d.z * y_axis.z;
	rotated.z = d.x * z_axis.x + d.y * z_axis.y + d.z * z_axis.z;
	//rotated.z *= -1;
	return (rotated);
}

void	try_all_intersections(t_p3 O, t_p3 d, double min, double max, t_lst *lst, t_lst *closest_figure, double *closest_intersection)
{
	double	in;

	while (lst)
	{
		if (lst->flag & SP)
			in = sphere_intersection(O, d, lst);
		else if (lst->flag & PL)
			in = plane_intersection(O, d, lst);
		/*else if (lst->flag & SQ)
			in = square_intersection;
		else if (lst->flag & CY)
			in = cylinder_intersection;
		else if (lst->flag & TR)
			in = triangle_intersection;
		*/
		if (in > min && in < *closest_intersection)
		{
			*closest_figure = *lst;
			*closest_intersection = in;
		}

		lst = lst->next;
	}
}

int		average_color(int col_a, int col_b)
{
	int mask = 255;
	int	r;
	int	g;
	int	b;

	r = (((col_a & (mask << 16)) + (col_b & (mask << 16))) / 2) >> 16;
	g = (((col_a & (mask << 8)) + (col_b & (mask << 8))) / 2) >> 8;
	b = ((col_a & mask) + (col_b & mask)) / 2;
	
	r = r > 255 ? 255 : r;
	g = g > 255 ? 255 : g;
	b = b > 255 ? 255 : b;

	return ((r << 16) | (g << 8) | b);
}

int	trace_ray(t_p3 O, t_p3 d, double min, t_scn data, t_lst *lst, int depth)
{
    int	    local_color;
    int	    reflected_color;
    t_lst   closest_figure;
    double  closest_intersection;
    t_p3    normal;
    t_p3    ip;
    t_p3    reflected;
    double  r;


    data.background = 0x000000;
    local_color = 0x000000;
    reflected_color = 0x000000;

    closest_intersection = INFINITY;
    closest_figure.flag = 0;
    try_all_intersections(O, d, min, INFINITY, lst, &closest_figure, &closest_intersection);	
    local_color = closest_figure.flag != 0 ? closest_figure.color : data.background;
    ip = vec_add(data.cam, scal_x_vec(closest_intersection, normalize(d)));
    
    normal = calc_normal(ip, closest_figure);
    
    local_color = color_x_light(local_color, compute_light(ip, normal, data, lst));

    r = closest_figure.reflective;
    if (depth <= 0 || r <= 0.0)
	return (local_color);

    reflected = reflect_ray(scal_x_vec(-1, d), normal);
    reflected_color = trace_ray(ip, reflected, 0.001, data, lst, depth - 1);

    return (color_x_light(local_color, (1 - r)) + color_x_light(reflected_color, r));
}


void	render_scene(void *mlx_ptr, void *win_ptr, t_scn data, t_lst *lst,
			int *pixel_tab, int t /*void * t*/)
{
	int			n = data.yres / NUM_THREADS;

	int 		i;
	int 		j;
	int		color;

	
	t_p3		d;

	//set_camera(&data);

	j = n * t;
	while (/*j < data.yres */   j < (n * (t + 1)))
	{
		i = 0;
		while (i < data.xres)
		{
			d = define_vector(i, j, data);
			d = LookAt(d, data.cam_nv, data.cam);	
			
			//data.O = data.cam;

			color = trace_ray(data.cam, d, 1, data, lst, 3);
			//color = trace_ray(O, data.nv, 1, data, lst, 3);

			//mlx_pixel_put(mlx_ptr, win_ptr, i, j, color);
			if (i % TIMES_RES == 0 && j % TIMES_RES == 0)
				pixel_tab[i/TIMES_RES + (j/TIMES_RES * data.xres/TIMES_RES)] = color;
			else
				pixel_tab[i/TIMES_RES + (j/TIMES_RES * data.xres/TIMES_RES)] = average_color(color,
						pixel_tab[i/TIMES_RES + (j/TIMES_RES * data.xres/TIMES_RES)]);
				
			//	pixel_tab[i + (j * data.xres)] = color;

			i++;
		}
		j++;
	}
}


void		*do_the_thing(void *jeje)
{
	t_thngs		*cosas;

	cosas = (t_thngs*)jeje;
	render_scene(cosas->mlx_ptr, cosas->win_ptr, cosas->data, cosas->lst, cosas->pixel_tab, cosas->i);

}

int main(int ac, char **av)
{
	pthread_t	threads[NUM_THREADS];
	int			i;

	void		*mlx_ptr;
	void		*win_ptr;
	void		*img_ptr;
	
	int			bits_per_pixel;
	int			size_line;
	int			endian;
	int			*pixel_tab;

	t_scn		data;
	t_lst		*lst;
	t_lst		*begin;
	t_thngs		cosas[NUM_THREADS];

	
	parse_scene(&data, &lst, ac, av);
	set_camera(&data);

	mlx_ptr = mlx_init();
	win_ptr = mlx_new_window(mlx_ptr, data.xres, data.yres, "miniRT v1");

	img_ptr = mlx_new_image (mlx_ptr, data.xres, data.yres);
		
	pixel_tab = (int *)mlx_get_data_addr(img_ptr, &bits_per_pixel, &size_line, &endian);

	//data.xres *= TIMES_RES;
	//data.yres *= TIMES_RES;

	i = 0;
	while (i < NUM_THREADS)
	{	
		cosas[i].mlx_ptr = mlx_ptr;
		cosas[i].win_ptr = win_ptr;
		cosas[i].data = data;
		cosas[i].lst = lst;
		cosas[i].i = i;
		cosas[i].pixel_tab = pixel_tab;
		i++;
	}
	i = 0;
	while (i < NUM_THREADS)
	{
		pthread_create(&threads[i], NULL, do_the_thing, (&cosas[i]));
		i++;
	}
	i = 0;
	while (i < NUM_THREADS)
		pthread_join(threads[i++], NULL);

	mlx_put_image_to_window (mlx_ptr, win_ptr, img_ptr, 0, 0);
	
	mlx_hook(win_ptr, 17, 1L << 17, exit, &data);
	mlx_loop(mlx_ptr);
	
	pthread_exit(NULL);

	return (0);
}
