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

void		next(char **str)
{
	while (**str == 32 || (**str >= 9 && **str <= 13))
		(*str)++;
}


void		comma(char **str)
{
	if (**str != ',')
	{
		perror("Scene Error: aprende a hacer una escena en condiciones gilipollas\n");
		exit(0);
	}	
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
	int	color;

	color = 0;
	color |= stoi(str) << 16;
	comma(str);
	color |= stoi(str) << 8;
	comma(str);
	color |= stoi(str);
	return (color);
}

void		parse_res(t_scene *data, char **str)
{
	next(str);
	data->xres = stoi(str);
	next(str);
	data->yres = stoi(str);
}

void		parse_ambient_light(t_scene *data, char **str)
{
	data->al_color = 0;

	next(str);
	data->ambient_light = stof(str);
	next(str);
	data->al_color = parse_color(str);
}

void		parse_camera(t_scene *data, char **str)
{
	t_camera	*elem;
	t_camera	*list;
	t_camera	*begin;

	begin = data->cam;
	list = data->cam;
	elem = ec_malloc(sizeof(t_camera));
	elem->next = NULL;
	if (list)
	{
		while (list->next)
			list = list->next;
		list->next = elem;
	}
	else
	{
		list = elem;
		begin = elem;
	}
	next(str);
	elem->o = parse_p3(str);
	elem->vec = parse_p3(str);
	elem->fov = stoi(str);
	data->cam = begin;
}

void		parse_cylinder(t_scene *data, t_figures **elem, t_figures **begin, char **str)
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
	lst->fig.cy.nv = parse_p3(str);
	lst->fig.cy.r = stof(str) / 2;
	next(str);
	lst->fig.cy.h = stof(str);
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
	next(str);
	list->color = parse_color(str);
	(*data)->l = begin;
}

void		parse_sphere(t_scene *data, t_figures **elem, t_figures **begin, char **str)
{
	t_figures   *lst;

	ft_addnewlst_back(elem, begin);
	lst = *elem;
	while (lst->next)
		lst = lst->next;
	lst->flag = 0;
	lst->flag |= SP;
	next(str);
	lst->fig.sp.c = parse_p3(str);
	lst->fig.sp.r = stof(str) / 2;
	next(str);
	lst->color = parse_color(str);
}

void		parse_square(t_scene *data, t_figures **elem, t_figures **begin, char **str)
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
	lst->fig.sq.nv = parse_p3(str);
	lst->fig.sq.side = stof(str);
	next(str);
	lst->color = parse_color(str);
}

void		parse_plane(t_scene *data, t_figures **elem, t_figures **begin, char **str)
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
	lst->fig.pl.nv = parse_p3(str);
	lst->color = parse_color(str);
}

void		parse_triangle(t_scene *data, t_figures **elem, t_figures **begin, char **str)
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
	lst->color = parse_color(str);
}

void	parse_elems(t_scene *data, t_figures **lst, t_figures **begin, char *str)
{
	while (*str)
	{
		if (*str == '#')
		{
			while (*str && *str != '\n')
				str++;
		}
		else if (*str == 'R' && *(str++))
			parse_res(data, &str);
		else if (*str == 'A' && *(str++))
			parse_ambient_light(data, &str);
		else if (*str == 'c' && (*(str + 1) == 32 || *(str + 1) == 9) && *(str++))
			parse_camera(data, &str);
		else if (*str == 'c' && *(str + 1) == 'y' && *(str++) && *(str++))
			parse_cylinder(data, lst, begin, &str);
		else if (*str == 'l' && (*(str + 1) == 32 || *(str + 1) == 9) && *(str++))
			parse_light(&data, &str);
		else if (*str == 's' && *(str + 1) == 'p' && *(str++) && *(str++))
			parse_sphere(data, lst, begin, &str);
		else if (*str == 's' && *(str + 1) == 'q' && *(str++) && *(str++))
			parse_square(data, lst, begin, &str);
		else if (*str == 'p' && *(str + 1) == 'l' && *(str++) && *(str++))
			parse_plane(data, lst, begin, &str);
		else if (*str == 't' && *(str + 1) == 'r' && *(str++) && *(str++))
			parse_triangle(data, lst, begin, &str);
		str++;
	}
}

void	parse_scene(t_scene *data, t_figures **lst, int ac, char **av)
{
	t_figures	*begin;
	char		*str;
	int			fd;

	if (ac != 2)
		usage(av[0]);
	*lst = NULL;
	data->l = NULL;
	data->cam = NULL;
	str = (char *)ec_malloc(sizeof(char) * (BUFSIZE + 1));
	if ((fd = open(av[1], 0)) == -1)
		fatal("while opening file");
	str = readfile(str, fd);
	parse_elems(data, lst, &begin, str);
	*lst = begin;
}
