/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone_intersection_utils1.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 19:25:57 by kgriset           #+#    #+#             */
/*   Updated: 2025/08/14 19:37:51 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"

void	cone_compute_params(const t_ray ray, const t_cone cone,
		t_cone_solve *c)
{
	c->cos_angle = vec_scal(ray.direction, cone.dir);
	c->cos_angle_sq = (c->cos_angle) * (c->cos_angle);
	c->tan_angle = cone.radius / cone.height;
}

void	cone_prepare_projections(t_cone_inter_solve *co, const t_ray ray,
		const t_cone cone)
{
	co->oc = vec_minus(ray.origin, cone.origin);
	co->oc_proj = vec_minus(co->oc, vec_mult(vec_scal(co->oc, cone.dir),
				cone.dir));
	co->ray_dir_proj = vec_minus(ray.direction, vec_mult(vec_scal(ray.direction,
					cone.dir), cone.dir));
}

void	cone_compute_quadratic(t_cone_inter_solve *co, t_cone_solve c, \
		const t_cone cone)
{
	co->a = vec_scal(co->ray_dir_proj, co->ray_dir_proj) - \
		c.tan_angle * c.tan_angle * c.cos_angle_sq;
	co->half_b = vec_scal(co->ray_dir_proj, co->oc_proj) - \
		c.tan_angle * c.tan_angle * c.cos_angle * \
		vec_scal(co->oc, cone.dir);
	co->c = vec_scal(co->oc_proj, co->oc_proj) - c.tan_angle * c.tan_angle
		* vec_scal(co->oc, cone.dir) * vec_scal(co->oc, cone.dir);
}

void	cone_check_first_root(t_cone_inter_solve *co, const t_ray ray,
		const t_cone cone)
{
	if (co->t1 > EPSILON)
	{
		co->hit_point = vec_plus(ray.origin, vec_mult(co->t1, ray.direction));
		co->to_hit = vec_minus(co->hit_point, cone.origin);
		co->height_proj = vec_scal(co->to_hit, cone.dir);
		if (co->height_proj >= 0 && co->height_proj <= cone.height)
			co->candidate_t = co->t1;
	}
}
