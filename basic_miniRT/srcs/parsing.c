/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgarcia- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/21 11:31:29 by mgarcia-          #+#    #+#             */
/*   Updated: 2020/03/12 21:31:36 by mgarcia-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void		in_range(double nb, double min, double max, char *function)
{
	char	error_message[100];

	if (nb < min || nb > max)
	{
		ft_strncpy(error_message, function, 76);
		ft_strcat(error_message, " parameter out of range\n");
		scene_error(error_message);
	}
}

void		next(char **str)
{
	while (**str == 32 || (**str >= 9 && **str <= 13))
		(*str)++;
}


void		comma(char **str)
{
	if (**str != ',')
		scene_error("parameters bad formatted\n");
	(*str)++;
}

t_p3		parse_p3(char **str)
{
	t_p3	p;

	p.x = stof(str);
	comma(str);
	p.y = stof(str);
	comma(str);
	p.z = stof(str);
	next(str);
	return (p);
}

int			parse_color(char **str)
{
	int	r;
	int	g;
	int	b;

	r = 0;
	g = 0;
	b = 0;
	r |= stoi(str);
	in_range(r, 0, 255, "color must be in range [0, 255],");
	r <<= 16;
	comma(str);
	g |= stoi(str);
	in_range(g, 0, 255, "color must be in range [0, 255],");
	g <<= 8;
	comma(str);
	b |= stoi(str);
	in_range(b, 0, 255, "color must be in range [0, 255],");
	return (r | g | b);
}

void		parse_res(t_scene *data, char **str, int *init)
{
	if (*init > 0)
		scene_error("Resolution (R) can only be declared once in the scene\n");
	else
		*init = 1;

	next(str);
	data->xres = stoi(str);
	in_range(data->xres, 1, INFINITY, "resolution");
	next(str);
	data->yres = stoi(str);
	in_range(data->yres, 1, INFINITY, "resolution");
}

void		parse_ambient_light(t_scene *data, char **str, int *init)
{
	if (*init > 0)
		scene_error("Ambient lightning (A) can only be declared once in the scene\n");
	else
		*init = 1;

	data->al_color = 0;
	next(str);
	data->ambient_light = stof(str);
	in_range(data->ambient_light, 0, 1, "ambient lightning");
	next(str);
	data->al_color = parse_color(str);
}

void		parse_camera(t_minilibx *mlx, char **str, int *init)
{
	t_camera	*elem;
	t_camera	*list;
	t_camera	*begin;
	int			prev_idx;

	if (*init == 0)
		*init = 1;
	prev_idx = 0;
	begin = mlx->cam;
	list = mlx->cam;
	elem = ec_malloc(sizeof(t_camera));
	elem->next = NULL;
	if (list)
	{
		while (list->next)
			list = list->next;
		prev_idx = list->idx;	
		list->next = elem;
	}
	else
	{
		list = elem;
		begin = elem;
	}
	next(str);
	elem->idx = *init == 0 ? 1 : prev_idx + 1;
	elem->o = parse_p3(str);
	elem->nv = normalize(parse_p3(str));
	elem->fov = stoi(str);
	in_range(elem->fov, 0, 180, "camera");
	mlx->cam = begin;
}

void		parse_cylinder(t_figures **elem, t_figures **begin, char **str)
{
	t_figures	*lst;
	
	ft_addnewlst_back(elem, begin);
	lst = *elem;
	while (lst->next)
		lst = lst->next;
	lst->flag = 0;
	lst->flag |= CY;
	next(str);
	lst->fig.cy.c = parse_p3(str);
	lst->fig.cy.nv = normalize(parse_p3(str));
	lst->fig.cy.r = stof(str) / 2;
	in_range(lst->fig.cy.r, 0, INFINITY, "cylinder");
	next(str);
	lst->fig.cy.h = stof(str);
	in_range(lst->fig.cy.h, 0, INFINITY, "cylinder");
	next(str);
	lst->color = parse_color(str); 
}

void		parse_light(t_scene **data, char **str)
{
	t_light	*elem;
	t_light	*list;
	t_light *begin;

	begin = (*data)->l;
	list = (*data)->l;
	elem = ec_malloc(sizeof(t_light));
	elem->next = NULL;
	if (list)
	{
		while (list->next)
			list = list->next;
		list->next = elem;
		list = list->next;
	}
	else
	{
		list = elem;
		begin = elem;
	}
	next(str);
	list->o = parse_p3(str);
	list->br = stof(str);
	in_range(list->br, 0, 1, "light");
	next(str);
	list->color = parse_color(str);
	(*data)->l = begin;
}

void		parse_sphere(t_figures **elem, t_figures **begin, char **str)
{
	t_figures   *lst;

	ft_addnewlst_back(elem, begin);
	lst = *elem;
	while (lst->next)
		lst = lst->next;
	//lst->flag = 0;
	lst->flag = SP;
	next(str);
	lst->fig.sp.c = parse_p3(str);
	lst->fig.sp.r = stof(str) / 2;
	in_range(lst->fig.sp.r, 0, INFINITY, "sphere");
	next(str);
	lst->color = parse_color(str);
}

void		parse_square(t_figures **elem, t_figures **begin, char **str)
{
	t_figures   *lst;
	
	ft_addnewlst_back(elem, begin);
	lst = *elem;
	while (lst->next)
		lst = lst->next;
	lst->flag = 0;
	lst->flag |= SQ;
	next(str);
	lst->fig.sq.c = parse_p3(str);
	lst->normal = normalize(parse_p3(str));
	lst->fig.sq.side = stof(str);
	in_range(lst->fig.sq.side, 0, INFINITY, "square");
	next(str);
	lst->color = parse_color(str);
}

void		parse_plane(t_figures **elem, t_figures **begin, char **str)
{
	t_figures   *lst;
	
	ft_addnewlst_back(elem, begin);
	lst = *elem;
	while (lst->next)
		lst = lst->next;
	lst->flag = 0;
	lst->flag |= PL;
	next(str);
	lst->fig.pl.p = parse_p3(str);
	lst->normal = normalize(parse_p3(str));
	lst->color = parse_color(str);
}

void		parse_triangle(t_figures **elem, t_figures **begin, char **str)
{
	t_figures	*lst;

	ft_addnewlst_back(elem, begin);
	lst = *elem;
	while (lst->next)
		lst = lst->next;
	lst->flag = 0;
	lst->flag |= TR;
	next(str);
	lst->fig.tr.p1 = parse_p3(str);
	lst->fig.tr.p2 = parse_p3(str);
	lst->fig.tr.p3 = parse_p3(str);
	lst->normal = cross(vsubstract(lst->fig.tr.p3, lst->fig.tr.p1),
			vsubstract(lst->fig.tr.p2, lst->fig.tr.p1));
	lst->color = parse_color(str);
}

void	parse_elems(t_minilibx *mlx, t_scene *data, t_figures **lst, t_figures **begin, char *str)
{
	int		res_init = 0;
	int		al_init = 0;
	int		cam_init = 0;
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
