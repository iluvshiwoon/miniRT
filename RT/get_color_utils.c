/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_color_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 19:55:30 by kgriset           #+#    #+#             */
/*   Updated: 2025/08/20 14:55:04 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"

void	update_best_intersection(t_intersection *intersection,
		t_intersection *local_intersection, int *obj_id, int current_index)
{
	intersection->t = local_intersection->t;
	intersection->point = local_intersection->point;
	intersection->normal = local_intersection->normal;
	intersection->hit_cap = local_intersection->hit_cap;
	*obj_id = current_index;
}

static t_vec	material_checker_dispatch(t_object *obj, t_vec point,
		t_intersection *intersection)
{
	if (obj->type == sp)
		return (get_sphere_checkerboard(point, obj->albedo,
				(t_sphere *)obj->obj));
	else if (obj->type == cy)
	{
		if (intersection && intersection->hit_cap)
			return (obj->albedo);
		return (get_cylinder_checkerboard(point, obj->albedo,
				(t_cylinder *)obj->obj));
	}
	else if (obj->type == pl)
		return (get_plane_checkerboard(point, obj->albedo,
				(t_plane *)obj->obj));
	else if (obj->type == co)
	{
		if (intersection && intersection->hit_cap)
			return (obj->albedo);
		return (get_cone_checkerboard(point, obj->albedo, (t_cone *)obj->obj));
	}
	return (get_checkerboard_color(point, obj->albedo));
}

t_vec	get_material_color(t_object *obj, t_vec point,
		t_intersection *intersection)
{
	double	u;
	double	v;

	if (obj->texture_map_path)
	{
		get_uv(obj, intersection, &u, &v);
		return (get_texture_color(obj, u, v));
	}
	if (obj->checkerboard)
		return (material_checker_dispatch(obj, point, intersection));
	return (obj->albedo);
}

t_vec	generate_random_hemisphere_direction(t_vec normal, t_pcg32_random *rng)
{
	t_get_color	gc;

	gc.r1 = double_rng(rng);
	gc.r2 = double_rng(rng);
	gc.direction_random_local_basis = (t_vec){cos(2 * M_PI * gc.r1) * sqrt(1
			- gc.r2), sin(2 * M_PI * gc.r1) * sqrt(1 - gc.r2), sqrt(gc.r2)};
	gc.random_vec = (t_vec){double_rng(rng) - 0.5, double_rng(rng) - 0.5,
		double_rng(rng) - 0.5};
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

	direction_random = generate_random_hemisphere_direction(\
			gc->intersection.normal, rng);
	random_ray = (t_ray){vec_plus(gc->intersection.point, vec_mult(0.001,
				gc->intersection.normal)), direction_random};
	recursive_color = get_color(random_ray, rt, nb_rebound - 1, rng);
	return (vec_m_vec(recursive_color,
			get_material_color(&rt->scene.objects[gc->obj_id],
				gc->intersection.point, &gc->intersection)));
}
