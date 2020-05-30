#include "minirt.h"

t_p3	reflect_ray(t_p3 ray, t_p3 normal)
{
	return (vsubstract(scal_x_vec(2 * dot(normal, ray), normal), ray));
}

int		cproduct(int color, double coef)
{
	int mask;
	int r;
	int	g;
	int	b;

	mask = 255;
	r = coef * ((color & (mask << 16)) >> 16);
	g = coef * ((color & (mask << 8)) >> 8);
	b = coef * (color & mask);
	r = r > 255 ? 255 : r;
	g = g > 255 ? 255 : g;
	b = b > 255 ? 255 : b;
	return ((r << 16) | (g << 8) | b);
}

int		cadd(int color_a, int color_b)
{
	int mask;
	int	r;
	int	g;
	int b;

	mask = 255;
	r = ((color_a & (mask << 16)) + (color_b & (mask << 16))) & (mask << 16);
	g = ((color_a & (mask << 8)) + (color_b & (mask << 8))) & (mask << 8);
	b = ((color_a & mask) + (color_b & mask)) & mask;
	return (r | g | b);
}

int		average(int color1, int color2)
{
	int		average[3];
	int		mask;
	int		color[2];
	int		i;

	mask = 255;
	ft_memset(average, 0, 3 * sizeof(int));
	color[0] = color1;
	color[1] = color2;
	i = 0;
	while (i < 2)
	{
		average[0] += (color[i] & (mask << 16)) >> 16;
		average[1] += (color[i] & (mask << 8)) >> 8;
		average[2] += color[i] & mask;
		i++;
	}
	average[0] = average[0] / 2;
	average[1] = average[1] / 2;
	average[2] = average[2] / 2;
	return ((average[0] << 16) | (average[1] << 8) | average[2]);
}

int		color_difference(int color1, int color2)
{
	int		mask;
	int		r[2];
	int		g[2];
	int		b[2];
	int		distance_exp2;

	mask = 255;
	r[0] = (color1 & (mask << 16)) >> 16;
	g[0] = (color1 & (mask << 8)) >> 8;
	b[0] = color1 & mask;
	r[1] = (color2 & (mask << 16)) >> 16;
	g[1] = (color2 & (mask << 8)) >> 8;
	b[1] = color2 & mask;
	distance_exp2 =
	pow((r[1] - r[0]), 2) + pow((g[1] - g[0]), 2) + pow((b[1] - b[0]), 2);
	return (distance_exp2 > 1000);
}

int		color_x_light(int color, double rgb[3])
{
	unsigned int	mask;
	unsigned int	r;
	unsigned int	g;
	unsigned int	b;

	mask = 255 << 16;
	r = rgb[0] * ((color & mask) >> 16);
	mask >>= 8;
	g = rgb[1] * ((color & mask) >> 8);
	mask >>= 8;
	b = rgb[2] * (color & mask);
	r = r > 255 ? 255 : r;
	g = g > 255 ? 255 : g;
	b = b > 255 ? 255 : b;
	return ((r << 16) | (g << 8) | b);
}
