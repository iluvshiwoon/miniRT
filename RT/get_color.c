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
	{ obj = &scene.objects[scene.total_objects - 1];
		if (obj->is_intersection != NULL && obj->is_intersection(ray, *obj,
				&local_intersection))
		{
			has_inter = true;
			if (local_intersection.t < intersection->t)
			{
				intersection->t = local_intersection.t;
				intersection->point = local_intersection.point;
				intersection->normal = local_intersection.normal;
				intersection->hit_cap = local_intersection.hit_cap;
				*obj_id = scene.total_objects - 1;
			}
		}
		scene.total_objects--;
	}
	return (has_inter);
}

t_vec	get_color(t_ray ray, t_rt *rt, int nb_rebound, t_pcg32_random *rng)
{
	t_get_color	gc;
	t_vec		direct_light;
	t_vec		ambient_light;
	t_vec		specular_light;
	t_vec		reflection;
	t_vec		view_direction;

	gc.pixel = (t_vec){};
	gc.obj_id = 0;
	if (nb_rebound == 0)
		return ((t_vec){0, 0, 0});
	if (!visible_intersection(ray, rt->scene, &(gc.intersection), &(gc.obj_id)))
		return ((t_vec){0, 0, 0});
	
	if (rt->scene.objects[gc.obj_id].normal_map_path)
	{
		double	u;
		double	v;

		get_uv(&rt->scene.objects[gc.obj_id], gc.intersection.point, &u, &v);
		// printf("Object %d has a normal map, u: %f, v: %f\n", gc.obj_id, u, v);
		gc.intersection.normal = get_normal_from_map(&rt->scene.objects[gc.obj_id], u, v, gc.intersection.normal);
	}
	
	// Calculate view direction (from intersection point to camera)
	view_direction = normalize(vec_minus(ray.origin, gc.intersection.point));
	
	// Calculate Phong lighting components
	direct_light = calculate_direct_lighting(rt, &gc);
	ambient_light = calculate_ambient_lighting(rt, &gc);
	specular_light = calculate_specular_reflection(rt, &gc, view_direction);
	
	// Combine all lighting components
	gc.pixel = vec_plus(direct_light, ambient_light);
	gc.pixel = vec_plus(gc.pixel, specular_light);
	
	// Add recursive reflection for global illumination
	reflection = calculate_recursive_reflection(rt, &gc, nb_rebound, rng);
	gc.pixel = vec_plus(gc.pixel, reflection);
	return (gc.pixel);
}
