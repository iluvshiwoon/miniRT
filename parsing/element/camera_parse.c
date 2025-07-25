/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera_parse.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 12:29:30 by kgriset           #+#    #+#             */
/*   Updated: 2025/07/25 12:31:00 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../miniRT.h"

void	setup_camera_orientation(t_camera *camera)
{
	t_vec	world_up;
	t_vec	right;

	world_up = (t_vec){0, 1, 0};
	if (fabs(camera->direction.y) > 0.99)
	{
		world_up = (t_vec){0, 0, 1};
	}
	right = normalize(cross(camera->direction, world_up));
	camera->up = normalize(cross(right, camera->direction));
}

void	setup_camera_object(t_rt *rt, t_camera *camera, int *id)
{
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
}
