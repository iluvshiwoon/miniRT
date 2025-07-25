/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 12:25:20 by kgriset           #+#    #+#             */
/*   Updated: 2025/07/25 12:45:12 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../miniRT.h"

void	apply_pitch_rotation(t_vec *forward, t_vec *up, t_vec right,
		double pitch)
{
	t_mat3	pitch_rot;

	if (fabs(pitch) > 1e-6)
	{
		pitch_rot = create_rotation_axis(right, pitch);
		*forward = normalize(mat3_multiply_vec(pitch_rot, *forward));
		*up = normalize(mat3_multiply_vec(pitch_rot, *up));
	}
}

void	apply_yaw_rotation(t_vec *forward, t_vec *right, t_vec up, double yaw)
{
	t_mat3	yaw_rot;

	if (fabs(yaw) > 1e-6)
	{
		yaw_rot = create_rotation_axis(up, yaw);
		*forward = normalize(mat3_multiply_vec(yaw_rot, *forward));
		*right = normalize(mat3_multiply_vec(yaw_rot, *right));
	}
}

void	apply_roll_rotation(t_vec *up, t_vec *right, t_vec forward, double roll)
{
	t_mat3	roll_rot;

	if (fabs(roll) > 1e-6)
	{
		roll_rot = create_rotation_axis(forward, roll);
		*up = normalize(mat3_multiply_vec(roll_rot, *up));
		*right = normalize(mat3_multiply_vec(roll_rot, *right));
	}
}

void	finalize_camera_rotation(t_rt *rt, int id, t_vec forward, t_vec up)
{
	t_camera	*cam;
	t_vec		right;

	cam = rt->scene.objects[id].obj;
	right = normalize(cross(forward, up));
	up = normalize(cross(right, forward));
	cam->direction = forward;
	cam->up = up;
	rt->scene.camera = *cam;
	rt->scene.objects[id].string = rt->scene.objects[id].display_string(rt,
			rt->scene.objects[id]);
	rt->state.re_render_scene = true;
}
