#include "miniRT.h"

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
