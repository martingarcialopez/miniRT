#include "minirt.h"

static void		*render_thread(void *ptr)
{
	t_wrapper	*w;

	w = (t_wrapper *)ptr;
	while (w->mlx.cam)
	{
		render_scene(w);
		w->mlx.cam = w->mlx.cam->next;
	}
	return (NULL);
}

void			multithreaded_render(t_wrapper wrapper[NUM_THREADS])
{
	pthread_t	threads[NUM_THREADS];
	int			i;

	i = 0;
	while (i < NUM_THREADS)
	{
		pthread_create(&threads[i], NULL, render_thread, &wrapper[i]);
		i++;
	}
	i = 0;
	while (i < NUM_THREADS)
	{
		if (i > 0)
			ft_printf("waiting thread %d to finish...\n", i + 1);
		pthread_join(threads[i++], NULL);
	}
}

void			wrapp_data(t_minilibx mlx, t_scene data, t_figures *lst,
														t_wrapper *wrapper)
{
	int	i;

	i = 0;
	while (i < NUM_THREADS)
	{
		wrapper[i].mlx = mlx;
		wrapper[i].data = data;
		wrapper[i].lst = lst;
		wrapper[i].tid = i;
		i++;
	}
}
