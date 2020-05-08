/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgarcia- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/10 18:06:59 by mgarcia-          #+#    #+#             */
/*   Updated: 2020/03/12 16:04:47 by mgarcia-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

char		*readfile(char *str, int fd)
{
	char	buf[BUFSIZE + 1];
	char	*ptr;
	int		ret;
	int		bol;

	bol = 0;
	while ((ret = read(fd, buf, BUFSIZE)) > 0)
	{
		ptr = str;
		buf[ret] = '\0';
		if (!(str = ft_strjoin(str, buf)))
			return (NULL);
		free(ptr);
	}
	return (str);
}

int			stoi(char **str)
{
	int i;
	int	neg;

	i = 0;
	neg = 1;
	if (**str == '-' && *((*str)++))
		neg = -1;
	while (ft_isdigit(**str))
		i = i * 10 + (*((*str)++) - '0');
	return (i * neg);
}

double		stof(char **str)
{
	int		w;
	double	d;
	int		neg;

	w = 0;
	neg = 1;
	if (**str == '-' && *((*str)++))
		neg = -1;
	while (ft_isdigit(**str))
		w = w * 10 + (*((*str)++) - '0');
	if (**str == '.')
		(*str)++;
	d = 0.0;
	while(ft_isdigit(**str))
		d = d * 10 + (*((*str)++) - '0');
	while (d > 1)
		d /= 10;
	d += w;
	return (d * neg);
}

void		ft_addnewlst_back(t_figures **alst, t_figures** begin)
{
	t_figures   *elem;
	t_figures   *list;

	list = *alst;
	if (!(elem = malloc(sizeof(t_figures))))
	{
		printf("goorbai\n");
		exit (1);
	}
	elem->next = NULL;
	if (list)
	{
		while (list->next)
			list = list->next;
		list->next = elem;
	}
	else
	{
		*alst = elem;
		*begin = elem;
	}
}

int			next_cam(int keycode, t_minilibx *mlx)
{
	if (keycode == 53)
		exit(0);
	if (keycode != 49)
		return (0);
	if (mlx->cam->next)
	{
		mlx->cam = mlx->cam->next;
		mlx_put_image_to_window(mlx->mlx_ptr, mlx->win_ptr, mlx->cam->img_ptr, 0, 0);
	}
	else
	{
		mlx->cam = mlx->begin;
		mlx_put_image_to_window(mlx->mlx_ptr, mlx->win_ptr, mlx->cam->img_ptr, 0, 0);
	}
	return (1);
}

int			ft_close(void *param)
{
	param = (void *)param;
	exit(EXIT_SUCCESS);
	return (1);
}

void		fatal(char *message)
{
	char error_message[100];
	
	ft_strcpy(error_message, "[!!] Fatal Error ");
	ft_strncat(error_message, message, 83);
	perror(error_message);
	exit(EXIT_FAILURE);
}

void		*ec_malloc(unsigned int size)
{
	void *ptr;
 
	ptr = malloc(size);
	if (ptr == NULL)
		fatal("in malloc() on memory allocation");
	return (ptr);
}

void		usage(char *program_name)
{
	ft_printf("Usage: %s <scene.rt>\n", program_name);
	exit(EXIT_FAILURE);
}

int			p_is_outside(t_p3 p1, t_p3 p2, t_p3 p3, t_p3 ip)
{
	t_p3	v1;
	t_p3	v2;
	t_p3	vp;

	v1 = vsubstract(p2, p1);
	v2 = vsubstract(p3, p1);
	vp = vsubstract(ip, p1);
	if (vcos(cross(v1, v2), cross(v1, vp)) < 0)
		return (1);
	return (0);
}
/*
t_p3		reflect_ray(t_p3 ray, t_p3 normal)
{
    return (vsubstract(scal_x_vec(2 * dot(normal, ray), normal), ray));
}
*/
int		color_product(int color, double coef)
{
	int mask = 255;
	int r;
	int	g;
	int	b;

	r = coef * ((color & (mask << 16)) >> 16);
	g = coef * ((color & (mask << 8)) >> 8);
	b = coef * (color & mask);

	r = r > 255 ? 255 : r;
	g = g > 255 ? 255 : g;
	b = b > 255 ? 255 : b;

	return ((r << 16) | (g << 8) | b);
}

int		add_colors(int color_a, int color_b)
{
	int mask = 255;
	int	r;
	int	g;
	int b;

	r = ((color_a & (mask << 16)) + (color_b & (mask << 16))) & (mask << 16);
	g = ((color_a & (mask << 8)) + (color_b & (mask << 8))) & (mask << 8);
	b = ((color_a & mask) + (color_b & mask)) & mask;

	return (r | g | b);
}
