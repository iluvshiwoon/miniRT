/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 00:19:53 by kgriset           #+#    #+#             */
/*   Updated: 2025/08/14 19:07:21 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"
#include "unistd.h"

void	write_to_file(t_rt *rt)
{
	int		key;
	char	*name;
	int		fd;
	int		i;

	key = pcg_setseq_64_xsh_rr_32_random_r(&rt->rng);
	name = rt_ft_strjoin(rt, "scene", rt_ft_itoa(rt, key));
	name = rt_ft_strjoin(rt, name, ".rt");
	fd = open(name, O_CREAT, S_IRWXU);
	close(fd);
	fd = open(name, O_WRONLY);
	if (fd == -1)
		close_win(rt);
	i = 0;
	while (i < rt->scene.total_objects)
	{
		ft_printf_fd(fd, "%s\n", rt->scene.objects[i].string);
		i++;
	}
	close(fd);
}

// key_events moved to miniRT_keys.c

void	init_main(t_rt *rt, char **av)
{
	uint64_t	seeds[2];

    atomic_store(&rt->state.re_render_scene, true);
	rt->graphic_heap = init_alloc(&rt->graphic_heap);
	rt->parsing_heap = init_alloc(&rt->parsing_heap);
	entropy_getbytes((void *)seeds, sizeof(seeds));
	pcg_setseq_64_srandom_r(&rt->rng, seeds[0], seeds[1]);
	rt->current_heap = rt->parsing_heap;
	parsing_minirt(rt, av[1]);
	rt->w = 500;
	rt->h = 500;
	rt->total_pixels = rt->w * rt->h;
	rt->current_heap = rt->graphic_heap;
}

int	main(int ac, char **av)
{
	t_rt		*rt;

	if (check_args(ac, av))
		return (1);
	rt = malloc(sizeof(t_rt));
	if (!rt)
		return (1);
	ft_memset(rt, 0, sizeof(t_rt));
	rt->mlx = mlx_init();
	if (!rt->mlx)
		printf("malloc error mlx_init\n");
	init_main(rt, av);
	rt->config = (t_pass_config[]){{4, 2, rt->w * 80}, \
		{10, 1, rt->w * 40}, {80, 1, rt->w*20}};
	rt->win = mlx_new_window(rt->mlx, rt->w, rt->h, "miniRT");
	if (!rt->win)
		printf("malloc error mlx_init\n");
	rt->state.shuffled_pixels = wrap_malloc(rt, rt->w * rt->h
			* sizeof(*(rt->state.shuffled_pixels)));
	gen_shuffled_pixels(rt, rt->state.shuffled_pixels);
    init_multi_threading(rt);
	mlx_hook(rt->win, KeyPress, KeyPressMask, &key_events, rt);
	mlx_hook(rt->win, DestroyNotify, 0, &close_win, rt);
	mlx_loop_hook(rt->mlx, render, rt);
	mlx_loop(rt->mlx);
	return (0);
}
