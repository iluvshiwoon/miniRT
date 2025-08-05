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

int	close_win(t_rt *rt)
{
    int i;

    // Signal all worker threads to exit
    if (rt->shared)
    {
        pthread_mutex_lock(&rt->shared->work_mutex);
        atomic_store(&rt->shared->should_exit, true);
        pthread_cond_broadcast(&rt->shared->work_available);
        pthread_mutex_unlock(&rt->shared->work_mutex);

        // Wait for all worker threads to finish
        if (rt->workers)
        {
            i = 0;
            while (i < rt->shared->num_threads)
            {
                pthread_join(rt->workers[i].thread, NULL);
                i++;
            }
        }

        // Clean up threading resources
        pthread_mutex_destroy(&rt->shared->display_mutex);
        pthread_mutex_destroy(&rt->shared->work_mutex);
        pthread_cond_destroy(&rt->shared->work_available);
        pthread_cond_destroy(&rt->shared->to_display);
    }

    // Now safely clean up MLX resources
    if (rt->mlx && rt->image.img)
        mlx_destroy_image(rt->mlx, rt->image.img);
    if (rt->mlx && rt->win)
        mlx_destroy_window(rt->mlx, rt->win);
    if (rt->mlx)
    {
        mlx_destroy_display(rt->mlx);
        free(rt->mlx);
    }
    if (rt->fd_file)
        close(rt->fd_file);
    free_heap(rt);
    exit(0);
}
