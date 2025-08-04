/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_color_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 19:55:30 by kgriset           #+#    #+#             */
/*   Updated: 2025/07/18 20:09:02 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"

t_vec	calculate_direct_lighting(t_rt *rt, t_get_color *gc)
{
	gc->ray_light = (t_ray){vec_plus(gc->intersection.point, vec_mult(EPSILON,
				gc->intersection.normal)),
		normalize(vec_minus(rt->scene.light.origin, gc->intersection.point))};
	gc->obj_id_light = 0;
	gc->inter = visible_intersection(gc->ray_light, rt->scene,
			&(gc->intersection_light), &(gc->obj_id_light));
	gc->d_light2 = norm2(vec_minus(rt->scene.light.origin,
				gc->intersection.point));
	if (gc->inter && gc->intersection_light.t
		* gc->intersection_light.t < gc->d_light2)
		return ((t_vec){0, 0, 0});
	gc->cos_theta = fmax(0, vec_scal(normalize(vec_minus(rt->scene.light.origin,
						gc->intersection.point)), gc->intersection.normal));
	gc->light_contribution = vec_mult(rt->scene.light.intensity * EXPOSURE
			* gc->cos_theta / gc->d_light2, rt->scene.light.color);
	return (vec_mult(1 / M_PI, vec_m_vec(gc->light_contribution,
				rt->scene.objects[gc->obj_id].albedo)));
}

t_vec	calculate_ambient_lighting(t_rt *rt, t_get_color *gc)
{
	gc->ambient_contribution = vec_mult(GLOBAL_EXPOSURE
			* rt->scene.ambient_light.intensity, rt->scene.ambient_light.color);
	return (vec_mult(1 / M_PI, vec_m_vec(gc->ambient_contribution,
				rt->scene.objects[gc->obj_id].albedo)));
}

t_vec	generate_random_hemisphere_direction(t_rt *rt, t_vec normal, t_pcg32_random *rng)
{
	t_get_color	gc;

	gc.r1 = double_rng(rng);
	gc.r2 = double_rng(rng);
	gc.direction_random_local_basis = (t_vec){cos(2 * M_PI * gc.r1) * sqrt(1
			- gc.r2), sin(2 * M_PI * gc.r1) * sqrt(1 - gc.r2), sqrt(gc.r2)};
	gc.random_vec = (t_vec){double_rng(rng) - 0.5, double_rng(rng)
		- 0.5, double_rng(rng) - 0.5};
	gc.tangent1 = normalize(cross(normal, gc.random_vec));
	gc.tangent2 = cross(gc.tangent1, normal);
	return (vec_plus(vec_plus(vec_mult(gc.direction_random_local_basis.x,
					gc.tangent1), vec_mult(gc.direction_random_local_basis.y,
					gc.tangent2)), vec_mult(gc.direction_random_local_basis.z,
				normal)));
}

t_vec	calculate_recursive_reflection(t_rt *rt, t_get_color *gc,
		int nb_rebound, t_pcg32_random *rng)
{
	t_vec	direction_random;
	t_ray	random_ray;
	t_vec	recursive_color;

	direction_random = generate_random_hemisphere_direction(rt,
			gc->intersection.normal, rng);
	random_ray = (t_ray){vec_plus(gc->intersection.point, vec_mult(0.001,
				gc->intersection.normal)), direction_random};
	recursive_color = get_color(random_ray, rt, nb_rebound - 1, rng);
	return (vec_m_vec(recursive_color, rt->scene.objects[gc->obj_id].albedo));
}
