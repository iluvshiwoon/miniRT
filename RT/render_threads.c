/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_threads.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 20:28:34 by kgriset           #+#    #+#             */
/*   Updated: 2025/08/20 14:55:05 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"

void	init_thread(t_rt *rt)
{
	int			i;
	t_shared	*shared;
	t_worker	*workers;
	int			seeds[2];

	shared = rt->shared;
	i = 0;
	workers = wrap_malloc(rt, sizeof(*workers) * shared->num_threads);
	rt->workers = workers;
	while (i < shared->num_threads)
	{
		workers[i].shared = shared;
		workers[i].data.thread_id = i;
		workers[i].data.start_pixel = shared->chunks[i].start_pixel;
		workers[i].data.end_pixel = shared->chunks[i].end_pixel;
		entropy_getbytes((void *)seeds, sizeof(seeds));
		pcg_setseq_64_srandom_r(&workers[i].data.rng, seeds[0], seeds[1]);
		pthread_create(&workers[i].thread, NULL, worker_thread_loop,
			&workers[i]);
		i++;
	}
}

void	init_multi_threading(t_rt *rt)
{
	rt->shared = wrap_malloc(rt, sizeof(*rt->shared));
	*rt->shared = (t_shared){};
	rt->shared->rt = rt;
	atomic_store(&rt->shared->work_paused, false);
	atomic_store(&rt->shared->paused_threads, 0);
	atomic_store(&rt->shared->should_exit, false);
	atomic_store(&rt->shared->work_ready, false);
	atomic_store(&rt->shared->current_pass, 0);
	if (pthread_mutex_init(&rt->shared->display_mutex, NULL) != 0)
		exit_error(rt, "Failed to initialize mutex");
	if (pthread_mutex_init(&rt->shared->work_mutex, NULL) != 0)
		exit_error(rt, "Failed to initialize mutex");
	if (pthread_cond_init(&rt->shared->work_available, NULL) != 0)
		exit_error(rt, "Failed to initialize condition");
	if (pthread_cond_init(&rt->shared->to_display, NULL) != 0)
		exit_error(rt, "Failed to initialize condition");
	rt->shared->num_threads = sysconf(_SC_NPROCESSORS_ONLN) * 16;
	if (rt->shared->num_threads <= 0)
		exit_error(rt, "Failed to retrieve number of available threads");
	rt->shared->chunks = wrap_malloc(rt, sizeof(*rt->shared->chunks)
			* rt->shared->num_threads);
	fill_chunk_index(rt, rt->shared->chunks, rt->shared->num_threads);
	init_thread(rt);
}
