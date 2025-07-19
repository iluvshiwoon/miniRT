/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_color.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 19:54:14 by kgriset           #+#    #+#             */
/*   Updated: 2025/07/19 16:58:05 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"

bool	visible_intersection(const t_ray ray, t_scene scene,
		t_intersection *intersection, int *obj_id)
{
	t_intersection	local_intersection;
	t_object		*obj;
	bool			has_inter;

	intersection->t = 1e99;
	has_inter = false;
	while (scene.total_objects)
	{
		obj = &scene.objects[scene.total_objects - 1];
		if (obj->is_intersection != NULL && obj->is_intersection(ray, *obj,
				&local_intersection))
		{
			has_inter = true;
			if (local_intersection.t < intersection->t)
			{
				intersection->t = local_intersection.t;
				intersection->point = local_intersection.point;
				intersection->normal = local_intersection.normal;
				*obj_id = scene.total_objects - 1;
			}
		}
		scene.total_objects--;
	}
	return (has_inter);
}

t_vec	get_color(t_ray ray, t_rt *rt, int nb_rebound)
{
	t_get_color	gc;
	t_vec		direct_light;
	t_vec		ambient_light;
	t_vec		reflection;

	gc.pixel = (t_vec){};
	gc.obj_id = 0;
	if (nb_rebound == 0)
		return ((t_vec){0, 0, 0});
	if (!visible_intersection(ray, rt->scene, &(gc.intersection), &(gc.obj_id)))
		return ((t_vec){0, 0, 0});
	direct_light = calculate_direct_lighting(rt, &gc);
	ambient_light = calculate_ambient_lighting(rt, &gc);
	gc.pixel = vec_plus(direct_light, ambient_light);
	reflection = calculate_recursive_reflection(rt, &gc, nb_rebound);
	gc.pixel = vec_plus(gc.pixel, reflection);
	return (gc.pixel);
}
