/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gschwand <gschwand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 10:15:25 by gschwand          #+#    #+#             */
/*   Updated: 2025/07/25 12:21:03 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../miniRT.h"

t_vec	camera_to_world_movement(t_camera cam, t_vec local_movement)
{
	t_vec	right;
	t_vec	up;
	t_vec	forward;
	t_vec	world_movement;

	right = get_camera_right(cam);
	up = get_camera_up(cam);
	forward = normalize(cam.direction);
	world_movement = vec_plus(vec_plus(vec_mult(local_movement.x, right),
				vec_mult(local_movement.y, up)), vec_mult(local_movement.z,
				forward));
	return (world_movement);
}

void	rotate_camera_local(t_rt *rt, int id, double pitch, double yaw,
		double roll)
{
	t_camera	*cam;
	t_vec		forward;
	t_vec		up;
	t_vec		right;
	t_mat3		pitch_rot;
	t_mat3		yaw_rot;
	t_mat3		roll_rot;

	cam = rt->scene.objects[id].obj;
	// Get current camera basis vectors
	forward = normalize(cam->direction);
	up = normalize(cam->up);
	right = normalize(cross(forward, up));
	// Apply pitch rotation (around current right axis)
	if (fabs(pitch) > 1e-6)
	{
		pitch_rot = create_rotation_axis(right, pitch);
		forward = normalize(mat3_multiply_vec(pitch_rot, forward));
		up = normalize(mat3_multiply_vec(pitch_rot, up));
	}
	// Apply yaw rotation (around current up axis - TRUE FPS behavior)
	if (fabs(yaw) > 1e-6)
	{
		yaw_rot = create_rotation_axis(up, yaw);
		forward = normalize(mat3_multiply_vec(yaw_rot, forward));
		right = normalize(mat3_multiply_vec(yaw_rot, right));
		// Note: up vector doesn't change during yaw rotation around itself
	}
	// Apply roll rotation (around current forward axis)
	if (fabs(roll) > 1e-6)
	{
		roll_rot = create_rotation_axis(forward, roll);
		up = normalize(mat3_multiply_vec(roll_rot, up));
		right = normalize(mat3_multiply_vec(roll_rot, right));
		// Note: forward vector doesn't change during roll rotation around itself
	}
	// Ensure orthogonality (Gram-Schmidt process to prevent drift)
	// Keep forward as-is, orthogonalize right and up
	right = normalize(cross(forward, up));
	up = normalize(cross(right, forward));
	// Update camera vectors
	cam->direction = forward;
	cam->up = up;
	rt->scene.camera = *cam;
	rt->scene.objects[id].string = rt->scene.objects[id].display_string(rt,
			rt->scene.objects[id]);
	rt->state.re_render_scene = true;
}
void	translate_camera(t_rt *rt, int id, t_vec vec)
{
	t_camera	*camera;

	camera = rt->scene.objects[id].obj;
	camera->origin = vec_plus(camera->origin, vec);
	rt->scene.camera = *camera;
	rt->scene.objects[id].string = rt->scene.objects[id].display_string(rt,
			rt->scene.objects[id]);
	rt->state.re_render_scene = true;
}

char	*string_camera(t_rt *rt, const struct s_object object)
{
	char		*r_value;
	char		*dest;
	t_camera	*camera;

	dest = (char[24 + 1]){};
	camera = object.obj;
	r_value = rt_ft_strjoin(rt, "C  ", vec_toa(rt, camera->origin));
	r_value = rt_ft_strjoin(rt, r_value, " ");
	r_value = rt_ft_strjoin(rt, r_value, vec_toa(rt, camera->direction));
	r_value = rt_ft_strjoin(rt, r_value, " ");
	fpconv_dtoa(camera->fov * 180 / M_PI, dest);
	r_value = rt_ft_strjoin(rt, r_value, dest);
	return (r_value);
}

void	parse_camera(t_rt *rt, char *line, int *id)
{
	t_camera	*camera;
	char		**tab;
	t_vec		world_up;
	t_vec		right;

	camera = wrap_malloc(rt, sizeof(*camera));
	tab = rt_ft_split(rt, line, ' ');
	if (tab[1] && tab[2] && tab[3])
	{
		camera->origin = parse_vec(rt, tab[1]);
		camera->direction = normalize(parse_vec(rt, tab[2]));
		camera->fov = ft_atoi_double(tab[3]);
		if (camera->fov < 0 || camera->fov > 180)
			exit_error(rt, "Error: Invalid fov for camera");
		camera->fov *= M_PI / 180.0;
		// Initialize camera up vector
		world_up = (t_vec){0, 1, 0};
		if (fabs(camera->direction.y) > 0.99)
		{
			world_up = (t_vec){0, 0, 1};
		}
		right = normalize(cross(camera->direction, world_up));
		camera->up = normalize(cross(right, camera->direction));
		rt->scene.objects[*id].is_intersection = NULL;
		rt->scene.objects[*id].albedo = (t_vec){};
		rt->scene.objects[*id].obj = camera;
		rt->scene.objects[*id].id = *id;
		rt->scene.objects[*id].type = C;
		rt->scene.objects[*id].display_string = &string_camera;
		rt->scene.objects[*id].rotate = &rotate_camera_local;
		rt->scene.objects[*id].translate = &translate_camera;
		rt->scene.objects[*id].string = string_camera(rt,
				rt->scene.objects[*id]);
		(*id)++;
		rt->scene.camera = *camera;
		return ;
	}
	exit_error(rt, "Error: Invalid number of arguments for camera");
}
