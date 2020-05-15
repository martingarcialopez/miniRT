/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgarcia- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/21 11:31:29 by mgarcia-          #+#    #+#             */
/*   Updated: 2020/05/15 13:59:18 by mgarcia-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	parse_elems(t_minilibx *mlx, t_scene *data, t_figures **lst,
												t_figures **begin, char *str)
{
	int		res_init;
	int		al_init;
	int		cam_init;

	res_init = 0;
	al_init = 0;
	cam_init = 0;
	while (*str)
	{
		if (*str == '#')
		{
			while (*str && *str != '\n')
				str++;
		}
		else if (*str == 'R' && *(str++))
			parse_res(data, &str, &res_init);
		else if (*str == 'A' && *(str++))
			parse_ambient_light(data, &str, &al_init);
		else if (*str == 'c' && (*(str + 1) == 32 || *(str + 1) == 9) && *(str++))
			parse_camera(mlx, &str, &cam_init);
		else if (*str == 'c' && *(str + 1) == 'y' && *(str++) && *(str++))
			parse_cylinder(lst, begin, &str);
		else if (*str == 'l' && (*(str + 1) == 32 || *(str + 1) == 9) && *(str++))
			parse_light(&data, &str);
		else if (*str == 's' && *(str + 1) == 'p' && *(str++) && *(str++))
			parse_sphere(lst, begin, &str);
		else if (*str == 's' && *(str + 1) == 'q' && *(str++) && *(str++))
			parse_square(lst, begin, &str);
		else if (*str == 'p' && *(str + 1) == 'l' && *(str++) && *(str++))
			parse_plane(lst, begin, &str);
		else if (*str == 't' && *(str + 1) == 'r' && *(str++) && *(str++))
			parse_triangle(lst, begin, &str);
		str++;
	}
	if (res_init == 0 || al_init == 0 || cam_init == 0)
		scene_error("Not enough elements to render a scene\n");
}

void	parse_scene(t_minilibx *mlx, t_scene *data, t_figures **lst, char **av)
{
	t_figures	*begin;
	char		*str;
	int			fd;

	*lst = NULL;
	data->l = NULL;
	mlx->cam = NULL;
	write(1, "Parsing scene...\n", 17);
	str = (char *)ec_malloc(sizeof(char) * (BUFSIZE + 1));
	if ((fd = open(av[1], 0)) == -1)
		fatal("while opening file");
	str = readfile(str, fd);
	parse_elems(mlx, data, lst, &begin, str);
	*lst = begin;
}
