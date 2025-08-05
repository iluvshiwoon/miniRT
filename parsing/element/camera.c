/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gschwand <gschwand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 10:15:25 by gschwand          #+#    #+#             */
/*   Updated: 2025/07/25 12:44:39 by kgriset          ###   ########.fr       */
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

void	rotate_camera_local(t_rt *rt, int id, t_rvec rvec)
{
	t_camera	*cam;
	t_vec		forward;
	t_vec		up;
	t_vec		right;

	cam = rt->scene.objects[id].obj;
	forward = normalize(cam->direction);
	up = normalize(cam->up);
	right = normalize(cross(forward, up));
	apply_pitch_rotation(&forward, &up, right, rvec.pitch);
	apply_yaw_rotation(&forward, &right, up, rvec.yaw);
	apply_roll_rotation(&up, &right, forward, rvec.roll);
	finalize_camera_rotation(rt, id, forward, up);
}

void	translate_camera(t_rt *rt, int id, t_vec vec)
{
	t_camera	*camera;

	camera = rt->scene.objects[id].obj;
	camera->origin = vec_plus(camera->origin, vec);
	rt->scene.camera = *camera;
	rt->scene.objects[id].string = rt->scene.objects[id].display_string(rt,
			rt->scene.objects[id]);
    atomic_store(&rt->state.re_render_scene, true);
}

char	*string_camera(t_rt *rt, const struct s_object object)
{
	char		*r_value;
	char		*dest;
	t_camera	*camera;

	dest = (char [24 + 1]){};
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
		setup_camera_orientation(camera);
		setup_camera_object(rt, camera, id);
		return ;
	}
	exit_error(rt, "Error: Invalid number of arguments for camera");
}
