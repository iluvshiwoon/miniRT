/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 16:21:09 by kgriset           #+#    #+#             */
/*   Updated: 2025/07/25 12:43:36 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"

t_vec	handle_movement_keys(int keycode)
{
	t_vec	local_movement;

	local_movement = (t_vec){0, 0, 0};
	if (keycode == KEY_D)
		local_movement.x = -2.0;
	else if (keycode == KEY_A)
		local_movement.x = 2.0;
	else if (keycode == KEY_W)
		local_movement.z = -2.0;
	else if (keycode == KEY_S)
		local_movement.z = 2.0;
	else if (keycode == KEY_SHIFT)
		local_movement.y = 2.0;
	else if (keycode == KEY_CTRL)
		local_movement.y = -2.0;
	return (local_movement);
}

void	apply_movement(t_rt *rt, int id, enum e_type type, t_vec local_movement)
{
	t_vec	world_movement;

	if (local_movement.x == 0 && local_movement.y == 0 && local_movement.z == 0)
		return ;
	if (type == C)
	{
		world_movement = camera_to_world_movement(rt->scene.camera,
				local_movement);
		rt->scene.objects[id].translate(rt, id, world_movement);
	}
	else
	{
		world_movement = (t_vec){-local_movement.x, local_movement.y,
			local_movement.z};
		rt->scene.objects[id].translate(rt, id, world_movement);
	}
}

void	handle_rotation_keys(t_rt *rt, int id, enum e_type type, int keycode)
{
	if (type != C && type != cy && type != pl)
		return ;
	if (keycode == KEY_PITCH_UP)
		rt->scene.objects[id].rotate(rt, id,(t_rvec){ -0.07, 0, 0});
	else if (keycode == KEY_PITCH_DOWN)
		rt->scene.objects[id].rotate(rt, id,(t_rvec){ 0.07, 0, 0});
	else if (keycode == KEY_YAW_LEFT)
		rt->scene.objects[id].rotate(rt, id,(t_rvec){ 0, 0.07, 0});
	else if (keycode == KEY_YAW_RIGHT)
		rt->scene.objects[id].rotate(rt, id, (t_rvec){0, -0.07, 0});
	else if (keycode == KEY_ROLL_LEFT)
		rt->scene.objects[id].rotate(rt, id, (t_rvec){0, 0, 0.07});
	else if (keycode == KEY_ROLL_RIGHT)
		rt->scene.objects[id].rotate(rt, id, (t_rvec){0, 0, -0.07});
}

void	handle_toggle_keys(t_rt *rt, int keycode)
{
	if (keycode == KEY_C)
		rt->state.color_black = !rt->state.color_black;
	else if (keycode == KEY_Z)
		rt->state.display_string = !rt->state.display_string;
}

void	handle_navigation_keys(t_rt *rt, int keycode)
{
	if (keycode == KEY_N)
		rt->state.display_id++;
	else if (keycode == KEY_P)
		rt->state.display_id--;
	if (rt->state.display_id >= rt->scene.total_objects)
		rt->state.display_id = 0;
	else if (rt->state.display_id < 0)
		rt->state.display_id = rt->scene.total_objects - 1;
}
