/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lighting_specular.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 19:52:10 by kgriset           #+#    #+#             */
/*   Updated: 2025/08/14 19:52:11 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"

static void	prepare_specular_light(t_rt *rt, t_get_color *gc, int i)
{
	gc->ray_light = (t_ray){vec_plus(gc->intersection.point, vec_mult(EPSILON,
				gc->intersection.normal)),
		normalize(vec_minus(rt->scene.lights[i]->origin,
				gc->intersection.point))};
	gc->obj_id_light = 0;
	gc->inter = visible_intersection(gc->ray_light, rt->scene,
			&(gc->intersection_light), &(gc->obj_id_light));
	gc->d_light2 = norm2(vec_minus(rt->scene.lights[i]->origin,
				gc->intersection.point));
}

static t_vec	compute_specular_contribution(t_rt *rt, t_get_color *gc,
		t_vec view_direction, int i)
{
	t_vec	contrib;

	contrib = (t_vec){0, 0, 0};
	if (!(gc->inter && gc->intersection_light.t
			* gc->intersection_light.t < gc->d_light2))
	{
		gc->light_direction = normalize(vec_minus(rt->scene.lights[i]->origin,
					gc->intersection.point));
		gc->half_vector = normalize(vec_plus(gc->light_direction,
					view_direction));
		gc->cos_alpha = fmax(0, vec_scal(gc->half_vector,
					gc->intersection.normal));
		gc->cos_alpha = pow(gc->cos_alpha,
				rt->scene.objects[gc->obj_id].shininess);
		gc->specular_contribution = vec_mult(rt->scene.lights[i]->intensity
				* EXPOSURE * gc->cos_alpha / gc->d_light2,
				rt->scene.lights[i]->color);
		contrib = gc->specular_contribution;
	}
	return (contrib);
}

t_vec	calculate_specular_reflection(t_rt *rt, t_get_color *gc,
		t_vec view_direction)
{
	t_vec	total_specular_contribution;
	int		i;

	total_specular_contribution = (t_vec){0, 0, 0};
	i = 0;
	while (i < rt->scene.num_lights)
	{
		prepare_specular_light(rt, gc, i);
		total_specular_contribution = vec_plus(total_specular_contribution,
				compute_specular_contribution(rt, gc, view_direction, i));
		i++;
	}
	return (vec_m_vec(total_specular_contribution,
			rt->scene.objects[gc->obj_id].specular));
}
