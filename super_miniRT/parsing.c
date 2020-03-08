/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgarcia- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/21 11:31:29 by mgarcia-          #+#    #+#             */
/*   Updated: 2020/03/03 10:38:10 by mgarcia-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "libft.h"

#define BUFSIZE 32

static char		*readfile(char *str, int fd)
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

void		ft_addnewlst_back(t_lst **alst, t_lst** begin)
{
	t_lst	*elem;
	t_lst	*list;

	list = *alst;
	if (!(elem = malloc(sizeof(t_lst))))
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

void		next(char **str)
{
	while (**str == 32 || (**str >= 9 && **str <= 13))
		(*str)++;
}

void		comma(char **str)
{
	if (**str != ',')
	{
		ft_putstr("Scene Error\naprende a hacer una escena en condiciones gilipollas\n");
		exit(0);
	}	
	(*str)++;
}

void		parse_res(t_scn *data, char **str)
{
	next(str);
	data->xres = stoi(str);
	next(str);
	data->yres = stoi(str);
}

void		parse_alight(t_scn *data, char **str)
{
	data->acl = 0;

	next(str);
	data->al = stof(str);
	next(str);
	data->acl |= stoi(str) << 16;
	comma(str);
	data->acl |= stoi(str) << 8;
	comma(str);
	data->acl |= stoi(str) << 0;
}

void		parse_camera(t_scn *data, char **str)
{
	next(str);
	data->cam.x = stof(str);
	comma(str);
	data->cam.y = stof(str);
	comma(str);
	data->cam.z = stof(str);
	next(str);
	data->cam_nv.x = stof(str);
	comma(str);
	data->cam_nv.y = stof(str);
	comma(str);
	data->cam_nv.z = stof(str);
	next(str);
	data->fov = stoi(str);
}

void		parse_cylinder(t_scn *data, t_lst **elem, t_lst **begin, char **str)
{
	t_lst	*lst;
	
	ft_addnewlst_back(elem, begin);
	lst = *elem;
	while (lst->next)
		lst = lst->next;
	lst->flag = 0;
	lst->flag |= CY;
	lst->color = 0;
	next(str);
	lst->fig.cy.c.x = stof(str);
	comma(str);
	lst->fig.cy.c.y = stof(str);
	comma(str);
	lst->fig.cy.c.z = stof(str);
	next(str);
	lst->fig.cy.nv.x = stof(str);
	comma(str);
	lst->fig.cy.nv.y = stof(str);
	comma(str);
	lst->fig.cy.nv.z = stof(str);
	next(str);
	lst->fig.cy.r = stof(str) / 2;
	next(str);
	lst->fig.cy.h = stof(str);
	next(str);
	lst->color |= stoi(str) << 16;
	comma(str);
	lst->color |= stoi(str) << 8;
	comma(str);
	lst->color |= stoi(str) << 0;
}

void		parse_light(t_scn **data, char **str)
{
	t_light	*elem;
	t_light	*list;
	t_light *begin;

	begin = (*data)->l;
	list = (*data)->l;
	if (!(elem = malloc(sizeof(t_light))))
		exit (1);
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
	
	list->color = 0;
	next(str);
	list->o.x = stof(str);
	comma(str);
	list->o.y = stof(str);
	comma(str);
	list->o.z = stof(str);
	next(str);
	list->br = stof(str);
	next(str);
	list->color |= stoi(str) << 16;
	comma(str);
	list->color |= stoi(str) << 8;
	comma(str);
	list->color |= stoi(str) << 0;

	(*data)->l = begin;
}

void		parse_sphere(t_scn *data, t_lst **elem, t_lst **begin, char **str)
{
	t_lst	*lst;

	ft_addnewlst_back(elem, begin);
	lst = *elem;
	while (lst->next)
		lst = lst->next;
	lst->flag = 0;
	lst->flag |= SP;
	lst->color = 0;
	next(str);
	lst->fig.sp.c.x = stof(str);
	comma(str);
	lst->fig.sp.c.y = stof(str);
	comma(str);
	lst->fig.sp.c.z = stof(str);
	next(str);
	lst->fig.sp.r = stof(str) / 2;
	next(str);
	lst->specular = stoi(str);
	next(str);
	lst->reflective = stof(str);
	next(str);
	lst->color |= stoi(str) << 16;
	comma(str);
	lst->color |= stoi(str) << 8;
	comma(str);
	lst->color |= stoi(str) << 0;
}

void		parse_square(t_scn *data, t_lst **elem, t_lst **begin, char **str)
{
	t_lst	*lst;
	
	ft_addnewlst_back(elem, begin);
	lst = *elem;
	while (lst->next)
		lst = lst->next;
	lst->color = 0;
	lst->flag = 0;
	lst->flag |= SQ;
	next(str);
	lst->fig.sq.c.x = stof(str);
	comma(str);
	lst->fig.sq.c.y = stof(str);
	comma(str);
	lst->fig.sq.c.z = stof(str);
	next(str);
	lst->fig.sq.nv.x = stof(str);
	comma(str);
	lst->fig.sq.nv.y = stof(str);
	comma(str);
	lst->fig.sq.nv.z = stof(str);
	next(str);
	lst->fig.sq.size = stof(str);
	next(str);
	lst->color |= stoi(str) << 16;
	comma(str);
	lst->color |= stoi(str) << 8;
	comma(str);
	lst->color |= stoi(str) << 0;
}

void		parse_plane(t_scn *data, t_lst **elem, t_lst **begin, char **str)
{
	t_lst	*lst;
	
	ft_addnewlst_back(elem, begin);
	lst = *elem;
	while (lst->next)
		lst = lst->next;
	lst->color = 0;
	lst->flag = 0;
	lst->flag |= PL;
	next(str);
	lst->fig.pl.p.x = stof(str);
	comma(str);
	lst->fig.pl.p.y = stof(str);
	comma(str);
	lst->fig.pl.p.z = stof(str);
	next(str);
	lst->fig.pl.nv.x = stof(str);
	comma(str);
	lst->fig.pl.nv.y = stof(str);
	comma(str);
	lst->fig.pl.nv.z = stof(str);
	next(str);
	lst->specular = stoi(str);
	next(str);
	lst->reflective = stof(str);
	next(str);
	lst->color |= stoi(str) << 16;
	comma(str);
	lst->color |= stoi(str) << 8;
	comma(str);
	lst->color |= stoi(str) << 0;
}
/*
void		parse_triangle(t_scn *data, t_lst *lst, char *str)
{
	a
}
*/

void	parse_elems(t_scn *data, t_lst **lst, t_lst **begin, char *str)
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
			parse_alight(data, &str);
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
		//else if (*str == 't')
		//	parse_triangle(data, lst, str);
		str++;
	}
}

void	parse_scene(t_scn *data, t_lst **lst, int ac, char **av)
{
	t_lst	*begin;
	char	*str;
	int		fd;

	if (ac != 2)
	{
		printf("Error\n");
		exit(0);
	}
	*lst = NULL;
	data->l = NULL;
	str = (char *)malloc(sizeof(char) * (BUFSIZE + 1));
	fd = open(av[1], 0);
	str = readfile(str, fd);
	parse_elems(data, lst, &begin, str);
	*lst = begin;
}
