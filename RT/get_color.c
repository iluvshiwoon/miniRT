/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_color.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 19:54:14 by kgriset           #+#    #+#             */
/*   Updated: 2025/08/20 14:55:04 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"

static void	apply_normal_map_if_any(t_rt *rt, int obj_id,
		t_intersection *intersection)
{
	double	u;
	double	v;

	if (!rt->scene.objects[obj_id].normal_map_path)
		return ;
	get_uv(&rt->scene.objects[obj_id], intersection, &u, &v);
	intersection->normal = get_normal_from_map(&rt->scene.objects[obj_id], u, v,
			intersection->normal);
}

static void	compute_lighting_components(t_rt *rt, t_get_color *gc,
		t_light_comp *s_comp)
{
	s_comp->direct_light = calculate_direct_lighting(rt, gc);
	s_comp->ambient_light = calculate_ambient_lighting(rt, gc);
	s_comp->specular_light = calculate_specular_reflection(rt, gc,
			s_comp->view_direction);
}

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
				update_best_intersection(intersection, &local_intersection,
					obj_id, scene.total_objects - 1);
		}
		scene.total_objects--;
	}
	return (has_inter);
}

t_vec	get_color(t_ray ray, t_rt *rt, int nb_rebound, t_pcg32_random *rng)
{
	t_get_color		gc;
	t_light_comp	s_comp;

	gc.pixel = (t_vec){};
	gc.obj_id = 0;
	if (nb_rebound == 0)
		return ((t_vec){0, 0, 0});
	if (!visible_intersection(ray, rt->scene, &(gc.intersection), &(gc.obj_id)))
		return ((t_vec){0, 0, 0});
	apply_normal_map_if_any(rt, gc.obj_id, &gc.intersection);
	s_comp.view_direction = normalize(vec_minus(ray.origin,
				gc.intersection.point));
	compute_lighting_components(rt, &gc, &s_comp);
	gc.pixel = vec_plus(s_comp.direct_light, s_comp.ambient_light);
	gc.pixel = vec_plus(gc.pixel, s_comp.specular_light);
	calculate_recursive_reflection(rt, &gc, nb_rebound, rng);
	return (gc.pixel);
}
