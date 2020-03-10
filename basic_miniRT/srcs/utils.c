/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgarcia- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/10 18:06:59 by mgarcia-          #+#    #+#             */
/*   Updated: 2020/03/10 18:11:24 by mgarcia-         ###   ########.fr       */
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

int			next_cam(int keycode, t_scene *data)
{
	if (keycode == 53)
		exit(0);
	if (keycode != 49)
		return (0);
	if (data->cam->next)
	{
		data->cam = data->cam->next;
		mlx_put_image_to_window(data->cam->mlx_ptr, data->cam->win_ptr, data->cam->img_ptr, 0, 0);
	}
	else
	{
		data->cam = data->cam->begin;
		mlx_put_image_to_window(data->cam->mlx_ptr, data->cam->win_ptr, data->cam->img_ptr, 0, 0);
	}
	return (1);
}

int			ft_close(void *param)
{
	exit(0);
	return (1);
}
