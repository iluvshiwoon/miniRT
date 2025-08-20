/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT_keys.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 19:56:15 by kgriset           #+#    #+#             */
/*   Updated: 2025/08/20 14:55:04 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"

static void	handle_system_keys(t_rt *rt, int keycode)
{
	if (keycode == KEY_ESC)
	{
		close_win(rt);
		return ;
	}
	if (keycode == KEY_ENTER)
		write_to_file(rt);
}

static void	apply_actions(t_rt *rt, int id, enum e_type type, int keycode)
{
	t_vec	local_movement;

	if (type != A)
	{
		local_movement = handle_movement_keys(keycode);
		apply_movement(rt, id, type, local_movement);
	}
	if (!(type != C && type != cy && type != pl && type != co))
		handle_rotation_keys(rt, id, keycode);
	if (keycode == KEY_B)
		rt->scene.objects[rt->state.display_id].checkerboard = \
			!rt->scene.objects[rt->state.display_id].checkerboard;
}

int	key_events(int keycode, t_rt *rt)
{
	int			id;
	enum e_type	type;
	static int	last_keycode;

	id = rt->state.display_id;
	type = rt->scene.objects[id].type;
	handle_system_keys(rt, keycode);
	if (handle_navigation_keys(rt, keycode) == true || handle_toggle_keys(rt,
			keycode) == true)
		return (0);
	if (atomic_load(&rt->state.re_render_scene) == false)
	{
		atomic_store(&rt->state.re_render_scene, true);
		last_keycode = keycode;
		return (0);
	}
	if (atomic_load(&rt->shared->work_paused) != true)
		return (0);
	apply_actions(rt, id, type, last_keycode);
	return (0);
}
