/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 16:30:24 by kgriset           #+#    #+#             */
/*   Updated: 2025/08/14 20:25:19 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"

void	wait_for_work_available(t_shared *shared)
{
	pthread_mutex_lock(&shared->work_mutex);
	while (!atomic_load(&shared->work_ready)
		&& !atomic_load(&shared->should_exit))
		pthread_cond_wait(&shared->work_available, &shared->work_mutex);
	pthread_mutex_unlock(&shared->work_mutex);
}

void	handle_pause_state(t_shared *shared, int *pass)
{
	if (atomic_load(&shared->work_paused))
	{
		*pass = 0;
		atomic_fetch_add(&shared->paused_threads, 1);
		while (atomic_load(&shared->work_paused)
			&& !atomic_load(&shared->should_exit))
			usleep(1000);
		atomic_fetch_sub(&shared->paused_threads, 1);
	}
}
