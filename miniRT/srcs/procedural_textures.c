/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   procedural_textures.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgarcia- <mgarcia-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/30 18:18:07 by mgarcia-          #+#    #+#             */
/*   Updated: 2020/05/30 18:18:09 by mgarcia-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_p3		x_axis_rotation(t_p3 vec, double angle)
{
	t_p3	row1;
	t_p3	row2;
	t_p3	row3;
	t_p3	rotated;
	double	rad_angle;

	rad_angle = angle * M_PI / 180;
	row1 = (t_p3){1, 0, 0};
	row2 = (t_p3){0, cos(rad_angle), -sin(rad_angle)};
	row3 = (t_p3){0, sin(rad_angle), cos(rad_angle)};
	rotated.x = vec.x * row1.x + vec.y * row1.y + vec.z * row1.z;
	rotated.y = vec.x * row2.x + vec.y * row2.y + vec.z * row2.z;
	rotated.z = vec.x * row3.x + vec.y * row3.y + vec.z * row3.z;
	return (rotated);
}

t_p3		y_axis_rotation(t_p3 vec, double angle)
{
	t_p3	row1;
	t_p3	row2;
	t_p3	row3;
	t_p3	rotated;
	double	rad_angle;

	rad_angle = angle * M_PI / 180;
	row1 = (t_p3){cos(rad_angle), 0, sin(rad_angle)};
	row2 = (t_p3){0, 1, 0};
	row3 = (t_p3){-sin(rad_angle), 0, cos(rad_angle)};
	rotated.x = vec.x * row1.x + vec.y * row1.y + vec.z * row1.z;
	rotated.y = vec.x * row2.x + vec.y * row2.y + vec.z * row2.z;
	rotated.z = vec.x * row3.x + vec.y * row3.y + vec.z * row3.z;
	return (rotated);
}

t_p3	z_axis_rotation(t_p3 vec, double angle)
{
	t_p3	row1;
	t_p3	row2;
	t_p3	row3;
	t_p3	rotated;
	double	rad_angle;

	rad_angle = angle * M_PI / 180;
	row1 = (t_p3){cos(rad_angle), -sin(rad_angle), 0};
	row2 = (t_p3){sin(rad_angle), cos(rad_angle), 0};
	row3 = (t_p3){0, 0, 1};
	rotated.x = vec.x * row1.x + vec.y * row1.y + vec.z * row1.z;
	rotated.y = vec.x * row2.x + vec.y * row2.y + vec.z * row2.z;
	rotated.z = vec.x * row3.x + vec.y * row3.y + vec.z * row3.z;
	return (rotated);
}

static int	checkerboard(t_inter *inter)
{
	int		black;
	int		white;
	t_p3	coords;
	t_p3	val;
	int		party_mix;

	black = 0x000000;
	white = 0xffffff;
	coords.x = abs((int)floor(inter->p.x));
	coords.y = abs((int)floor(inter->p.y));
	coords.z = abs((int)floor(inter->p.z));
	val.x = (int)coords.x % 2;
	val.y = (int)coords.y % 2;
	val.z = (int)coords.z % 2;
	party_mix = ((int)val.x ^ (int)val.y) ^ (int)val.z;
	return (party_mix ? black : white);
}

static t_p3	sinwave(t_inter *inter)
{
	double	sinn;
	double	scal;

	scal = 10;
	sinn = sin(inter->p.z * scal) + sin(inter->p.x * scal) + sin(inter->p.y * scal);
	return (x_axis_rotation(inter->normal, sinn));
}

void		apply_texture(int texture, t_inter *inter)
{
	if (texture == 1)
		inter->color = checkerboard(inter);
	else if (texture == 2)
		inter->normal = sinwave(inter);
	else if (texture == 3)
		inter->color = rainbow(inter);
}
