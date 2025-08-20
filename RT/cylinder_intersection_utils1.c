/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder_intersection_utils1.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 17:17:30 by kgriset           #+#    #+#             */
/*   Updated: 2025/08/20 14:55:05 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"

void	cyl_prepare_oc_proj(t_cylinder_inter_solve *cy, const t_ray ray,
		const t_cylinder cylinder)
{
	cy->oc = vec_minus(ray.origin, cylinder.origin);
	cy->oc_proj = vec_minus(cy->oc, vec_mult(vec_scal(cy->oc, cylinder.dir),
				cylinder.dir));
}

void	cyl_prepare_ray_proj(t_cylinder_inter_solve *cy, const t_ray ray,
		const t_cylinder cylinder)
{
	cy->ray_dir_proj = vec_minus(ray.direction, vec_mult(vec_scal(ray.direction,
					cylinder.dir), cylinder.dir));
}

void	cyl_compute_quadratic(t_cylinder_inter_solve *cy,
		const t_cylinder cylinder)
{
	cy->a = vec_scal(cy->ray_dir_proj, cy->ray_dir_proj);
	cy->half_b = vec_scal(cy->ray_dir_proj, cy->oc_proj);
	cy->c = vec_scal(cy->oc_proj, cy->oc_proj) - cylinder.radius
		* cylinder.radius;
}

void	cyl_check_first_root(t_cylinder_inter_solve *cy, const t_ray ray,
		const t_cylinder cylinder)
{
	if (cy->t1 > EPSILON)
	{
		cy->hit_point = vec_plus(ray.origin, vec_mult(cy->t1, ray.direction));
		cy->to_hit = vec_minus(cy->hit_point, cylinder.origin);
		cy->height_proj = vec_scal(cy->to_hit, cylinder.dir);
		if (cy->height_proj >= 0 && cy->height_proj <= cylinder.height)
			cy->candidate_t = cy->t1;
	}
}
