/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone_intersection_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 19:18:07 by kgriset           #+#    #+#             */
/*   Updated: 2025/08/20 14:55:05 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../miniRT.h"

bool	cone_intersection_solve1(t_cone_inter_solve *co, const t_ray ray,
		const t_cone cone)
{
	t_cone_solve	c;

	cone_compute_params(ray, cone, &c);
	cone_prepare_projections(co, ray, cone);
	cone_compute_quadratic(co, c, cone);
	if (fabs(co->a) < EPSILON)
		return (false);
	co->discriminant = co->half_b * co->half_b - co->a * co->c;
	if (co->discriminant < 0)
		return (false);
	co->sqrt_discriminant = sqrt(co->discriminant);
	co->t1 = (-co->half_b - co->sqrt_discriminant) / co->a;
	co->t2 = (-co->half_b + co->sqrt_discriminant) / co->a;
	co->candidate_t = -1;
	cone_check_first_root(co, ray, cone);
	return (true);
}

static void	cone_check_second_root(t_cone_inter_solve *co, const t_ray ray,
		const t_cone cone)
{
	t_vec	hit_point;
	t_vec	to_hit;
	double	height_proj;

	if (co->candidate_t < 0 && co->t2 > EPSILON)
	{
		hit_point = vec_plus(ray.origin, vec_mult(co->t2, ray.direction));
		to_hit = vec_minus(hit_point, cone.origin);
		height_proj = vec_scal(to_hit, cone.dir);
		if (height_proj >= 0 && height_proj <= cone.height)
			co->candidate_t = co->t2;
	}
}

bool	cone_intersection_solve(const t_ray ray, const t_cone cone, double *t)
{
	t_cone_inter_solve	co;

	if (cone_intersection_solve1(&co, ray, cone) == false)
		return (false);
	cone_check_second_root(&co, ray, cone);
	if (co.candidate_t > 0)
	{
		*t = co.candidate_t;
		return (true);
	}
	return (false);
}

static void	cone_cap_update_min(t_cone_cap *co, const t_ray ray,
		const t_cone cone, t_vec *normal)
{
	if (plane_intersection_solve(ray, co->cap_plane, &(co->current_t))
		&& co->current_t > EPSILON)
	{
		co->p = vec_plus(ray.origin, vec_mult(co->current_t, ray.direction));
		if (norm2(vec_minus(co->p, co->cap_plane.origin)) <= cone.radius
			* cone.radius)
		{
			if (co->min_t < 0 || co->current_t < co->min_t)
			{
				co->min_t = co->current_t;
				*normal = co->cap_plane.normal;
			}
		}
	}
}

void	cone_cap_calc_s(t_co_cap_calc_args args)
{
	cone_cap_update_min(args.co, args.ray, args.cone, args.normal);
}
