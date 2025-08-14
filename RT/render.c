/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 17:17:58 by kgriset           #+#    #+#             */
/*   Updated: 2025/08/14 20:27:04 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../miniRT.h"
#include <sys/time.h>

static void	process_work(t_shared *shared, t_worker *worker, int *pass)
{
	while (*pass < 3 && !atomic_load(&shared->work_paused)
		&& !atomic_load(&shared->should_exit))
	{
		render_chunk(shared->rt, worker, *pass);
		(*pass)++;
	}
}

void	*worker_thread_loop(void *arg)
{
	t_worker	*worker;
	t_shared	*shared;
	int			pass;

	worker = arg;
	shared = worker->shared;
	pass = 0;
	while (!atomic_load(&shared->should_exit))
	{
		wait_for_work_available(shared);
		if (atomic_load(&shared->should_exit))
			break ;
		handle_pause_state(shared, &pass);
		if (atomic_load(&shared->should_exit))
			break ;
		process_work(shared, worker, &pass);
	}
	return (NULL);
}

static void	pause_threads_for_rerender(t_rt *rt, bool first_render)
{
	if (!first_render)
	{
		pthread_mutex_lock(&rt->shared->work_mutex);
		atomic_store(&rt->shared->work_paused, true);
		pthread_mutex_unlock(&rt->shared->work_mutex);
		while (atomic_load(&rt->shared->paused_threads) < \
				rt->shared->num_threads)
			usleep(1000);
		key_events(0, rt);
	}
}

static void	start_or_resume_workers(t_rt *rt)
{
	pthread_mutex_lock(&rt->shared->work_mutex);
	atomic_store(&rt->shared->work_paused, false);
	atomic_store(&rt->shared->work_ready, true);
	pthread_cond_broadcast(&rt->shared->work_available);
	pthread_mutex_unlock(&rt->shared->work_mutex);
}

int	render(t_rt *rt)
{
	static bool	first_render = true;

	if (atomic_load(&rt->state.re_render_scene))
	{
		pause_threads_for_rerender(rt, first_render);
		init_render(rt);
		start_or_resume_workers(rt);
		rt->state.pass = 3;
		first_render = false;
	}
	if (rt->state.pass >= 3)
	{
		mlx_put_image_to_window(rt->mlx, rt->win, rt->image.img, 0, 0);
		if (rt->state.display_string == true)
			display_string(rt, rt->state.display_id);
		return (1);
	}
	return (0);
}
