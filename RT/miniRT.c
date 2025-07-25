/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 00:19:53 by kgriset           #+#    #+#             */
/*   Updated: 2025/07/25 16:03:08 by kgriset          ###   ########.fr       */
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

int	key_events(int keycode, t_rt *rt)
{
	int			id;
	enum e_type	type;
	t_vec		local_movement;

	id = rt->state.display_id;
	type = rt->scene.objects[id].type;
	if (keycode == KEY_ESC)
	{
		close_win(rt);
		return (0);
	}
	if (type != A)
	{
		local_movement = handle_movement_keys(keycode);
		apply_movement(rt, id, type, local_movement);
	}
	handle_rotation_keys(rt, id, type, keycode);
	handle_toggle_keys(rt, keycode);
	handle_navigation_keys(rt, keycode);
	if (keycode == KEY_ENTER)
		write_to_file(rt);
	else
		printf("%d\n", keycode);
	return (0);
}

void	init_main(t_rt *rt, char **av)
{
	uint64_t	seeds[2];

	rt->state.re_render_scene = true;
	rt->graphic_heap = init_alloc(&rt->graphic_heap);
	rt->parsing_heap = init_alloc(&rt->parsing_heap);
	entropy_getbytes((void *)seeds, sizeof(seeds));
	pcg_setseq_64_srandom_r(&rt->rng, seeds[0], seeds[1]);
	rt->current_heap = rt->parsing_heap;
	parsing_minirt(rt, av[1]);
	rt->w = 800;
	rt->h = 800;
	rt->total_pixels = rt->w * rt->h;
	rt->current_heap = rt->graphic_heap;
}

int	main(int ac, char **av)
{
	t_rt		rt;

	if (check_args(ac, av))
		return (1);
	rt = (t_rt){};
	init_main(&rt, av);
	rt.config = (t_pass_config[]){{4, 2, rt.w}, \
		{10, 1, rt.w / 2}, {80, 1, rt.w / 8}};
	rt.mlx = mlx_init();
	if (!rt.mlx)
		printf("malloc error mlx_init\n");
	rt.win = mlx_new_window(rt.mlx, rt.w, rt.h, "miniRT");
	if (!rt.win)
		printf("malloc error mlx_init\n");
	rt.state.shuffled_pixels = wrap_malloc(&rt, rt.w * rt.h
			* sizeof(*(rt.state.shuffled_pixels)));
	gen_shuffled_pixels(&rt, rt.state.shuffled_pixels);
	mlx_hook(rt.win, KeyPress, KeyPressMask, &key_events, &rt);
	mlx_hook(rt.win, DestroyNotify, 0, &close_win, &rt);
	mlx_loop_hook(rt.mlx, render, &rt);
	mlx_loop(rt.mlx);
	return (0);
}
