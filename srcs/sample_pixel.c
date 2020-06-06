/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sample_pixel.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgarcia- <mgarcia-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/30 18:18:19 by mgarcia-          #+#    #+#             */
/*   Updated: 2020/05/30 18:18:20 by mgarcia-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static int			*sample_first_column(int *edge_color, int last[2],
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

static int			*sample_last_column(int *edge_color, int last[2],
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

static int			*sample_centered_pixel(int *edge_color, int last[2],
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

int					*sample_pixel(int *edge_color, int last[2],
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
