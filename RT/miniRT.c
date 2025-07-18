/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 00:19:53 by kgriset           #+#    #+#             */
/*   Updated: 2025/07/18 17:38:39 by kgriset          ###   ########.fr       */
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

	key = pcg32_random_r(&rt->rng);
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

// Modified key_events function with camera-relative movement
int	key_events(int keycode, t_rt *rt)
{
	int			id;
	enum e_type	type;
	t_vec		world_movement;

	id = rt->state.display_id;
	type = rt->scene.objects[id].type;
	if (keycode == KEY_ESC)
		close_win(rt);
	// Camera-relative movement for non-ambient objects
	else if (type != A)
	{
		t_vec local_movement = {0, 0, 0}; // x=right, y=up, z=forward
		if (keycode == KEY_D)
			local_movement.x = -2.0; // Move right
		else if (keycode == KEY_A)
			local_movement.x = 2.0; // Move left
		else if (keycode == KEY_W)
			local_movement.z = -2.0; // Move forward
		else if (keycode == KEY_S)
			local_movement.z = 2.0; // Move backward
		else if (keycode == KEY_SHIFT)
			local_movement.y = 2.0; // Move up
		else if (keycode == KEY_CTRL)
			local_movement.y = -2.0; // Move down
		// Apply movement
		if (local_movement.x != 0 || local_movement.y != 0
			|| local_movement.z != 0)
		{
			if (type == C)
			{
				// Camera: use camera-relative movement
				world_movement = camera_to_world_movement(rt->scene.camera,
						local_movement);
				rt->scene.objects[id].translate(rt, id, world_movement);
			}
			else
			{
				// Other objects: use world-relative movement
				world_movement = (t_vec){-local_movement.x, local_movement.y,
					local_movement.z};
				rt->scene.objects[id].translate(rt, id, world_movement);
			}
		}
	}
	// Rotation controls (unchanged)
	if (keycode == KEY_PITCH_UP && (type == C || type == cy || type == pl))
		rt->scene.objects[id].rotate(rt, id, -0.07, 0, 0);
	else if (keycode == KEY_PITCH_DOWN && (type == C || type == cy
			|| type == pl))
		rt->scene.objects[id].rotate(rt, id, 0.07, 0, 0);
	else if (keycode == KEY_YAW_LEFT && (type == C || type == cy || type == pl))
		rt->scene.objects[id].rotate(rt, id, 0, 0.07, 0);
	else if (keycode == KEY_YAW_RIGHT && (type == C || type == cy
			|| type == pl))
		rt->scene.objects[id].rotate(rt, id, 0, -0.07, 0);
	else if (keycode == KEY_ROLL_LEFT && (type == C || type == cy
			|| type == pl))
		rt->scene.objects[id].rotate(rt, id, 0, 0, 0.07);
	else if (keycode == KEY_ROLL_RIGHT && (type == C || type == cy
			|| type == pl))
		rt->scene.objects[id].rotate(rt, id, 0, 0, -0.07);
	// Other controls (unchanged)
	else if (keycode == KEY_C && rt->state.color_black == false)
		rt->state.color_black = true;
	else if (keycode == KEY_C && rt->state.color_black == true)
		rt->state.color_black = false;
	else if (keycode == KEY_Z && rt->state.display_string == false)
		rt->state.display_string = true;
	else if (keycode == KEY_Z && rt->state.display_string == true)
		rt->state.display_string = false;
	else if (keycode == KEY_N)
		rt->state.display_id++;
	else if (keycode == KEY_P)
		rt->state.display_id--;
	else if (keycode == KEY_ENTER)
		write_to_file(rt);
	else
		printf("%d\n", keycode);
	if (rt->state.display_id >= rt->scene.total_objects)
		rt->state.display_id = 0;
	else if (rt->state.display_id < 0)
		rt->state.display_id = rt->scene.total_objects - 1;
	return (0);
}

int	main(int ac, char **av)
{
	t_rt		rt;
	uint64_t	seeds[2];

	if (check_args(ac, av))
		return (1);
	rt = (t_rt){};
	rt.state.re_render_scene = true;
	rt.graphic_heap = init_alloc(&rt.graphic_heap);
	rt.parsing_heap = init_alloc(&rt.parsing_heap);
	entropy_getbytes((void *)seeds, sizeof(seeds));
	pcg32_srandom_r(&rt.rng, seeds[0], seeds[1]);
	rt.current_heap = rt.parsing_heap;
	parsing_minirt(&rt, av[1]);
	rt.W = 800;
	rt.H = 800;
	rt.total_pixels = rt.W * rt.H;
	rt.current_heap = rt.graphic_heap;
	rt.config = (t_pass_config[]){{4, 2, rt.W},{10, 1, rt.W / 2},{80, 1, rt.W / 8}};
	rt.mlx = mlx_init();
	if (!rt.mlx)
		printf("malloc error mlx_init\n");
	rt.win = mlx_new_window(rt.mlx, rt.W, rt.H, "miniRT");
	if (!rt.win)
		printf("malloc error mlx_init\n");
	rt.state.shuffled_pixels = wrap_malloc(&rt, rt.W * rt.H
			* sizeof(*(rt.state.shuffled_pixels)));
	gen_shuffled_pixels(&rt, rt.state.shuffled_pixels);
	mlx_hook(rt.win, KeyPress, KeyPressMask, &key_events, &rt);
	mlx_hook(rt.win, DestroyNotify, 0, &close_win, &rt);
	mlx_loop_hook(rt.mlx, render, &rt);
	mlx_loop(rt.mlx);
	return (0);
}
