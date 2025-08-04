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
