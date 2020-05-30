/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   supersampling.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgarcia- <mgarcia-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/30 18:18:31 by mgarcia-          #+#    #+#             */
/*   Updated: 2020/05/30 18:18:32 by mgarcia-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static int			supersample_first_corner(int *color, int center,
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

static int			supersample_second_corner(int *color, int center,
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

static int			supersample_third_corner(int *color, int center,
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

static int			supersample_fourth_corner(int *color, int center,
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

int					supersample(int *color, t_rss rss, t_wrapper *w)
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
