/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 17:27:42 by kgriset           #+#    #+#             */
/*   Updated: 2025/07/19 17:27:43 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"
#include <X11/X.h>
#include <X11/keysym.h>

int	close_win(t_rt *rt)
{
	int i;

	if (rt->shared)
	{
		atomic_store(&rt->shared->should_exit, true);
		pthread_mutex_lock(&rt->shared->work_mutex);
		pthread_cond_broadcast(&rt->shared->work_available);
		pthread_mutex_unlock(&rt->shared->work_mutex);
		i = 0;
		while (i < rt->shared->num_threads)
		{
			pthread_join(rt->workers[i].thread, NULL);
			i++;
		}
		pthread_mutex_destroy(&rt->shared->display_mutex);
		pthread_mutex_destroy(&rt->shared->work_mutex);
		pthread_cond_destroy(&rt->shared->work_available);
		pthread_cond_destroy(&rt->shared->to_display);
	}
	free_normal_maps(rt);
	free_texture_maps(rt);
	if (rt->win)
		mlx_destroy_window(rt->mlx, rt->win);
	if (rt->mlx)
	{
		mlx_destroy_display(rt->mlx);
		free(rt->mlx);
	}
	if (rt->fd_file > 0)
		close(rt->fd_file);
	free_heap(rt);
	exit(0);
}

char	*color_toa(t_rt *rt, t_vec vec)
{
	char	*str;
	char	*tmp;
	t_vec	color;

	color = vec_mult(255, vec);
	str = rt_ft_itoa(rt, (int)color.x);
	tmp = rt_ft_strjoin(rt, str, ",");
	str = tmp;
	tmp = rt_ft_strjoin(rt, str, rt_ft_itoa(rt, (int)color.y));
	str = tmp;
	tmp = rt_ft_strjoin(rt, str, ",");
	str = tmp;
	tmp = rt_ft_strjoin(rt, str, rt_ft_itoa(rt, (int)color.z));
	str = tmp;
	return (str);
}
