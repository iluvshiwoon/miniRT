/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone_intersection_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 16:30:00 by kgriset           #+#    #+#             */
/*   Updated: 2025/07/25 16:30:00 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"

bool	cone_intersection_solve1(t_cone_inter_solve *co,
		const t_ray ray, const t_cone cone)
{
	double	cos_angle;
	double	tan_angle;
	double	cos_angle_sq;

	// Calculate cone parameters
	cos_angle = vec_scal(ray.direction, cone.dir);
	cos_angle_sq = cos_angle * cos_angle;
	tan_angle = cone.radius / cone.height;

	co->oc = vec_minus(ray.origin, cone.origin);
	co->oc_proj = vec_minus(co->oc, vec_mult(vec_scal(co->oc, cone.dir),
				cone.dir));
	co->ray_dir_proj = vec_minus(ray.direction, vec_mult(vec_scal(ray.direction,
					cone.dir), cone.dir));

	// Cone quadratic equation coefficients
	// a = |D⊥|² - tan²(α) * cos²(θ)
	co->a = vec_scal(co->ray_dir_proj, co->ray_dir_proj) - 
		tan_angle * tan_angle * cos_angle_sq;
	
	// b = D⊥ · OC⊥ - tan²(α) * cos(θ) * (OC · axis)
	co->half_b = vec_scal(co->ray_dir_proj, co->oc_proj) - 
		tan_angle * tan_angle * cos_angle * vec_scal(co->oc, cone.dir);
	
	// c = |OC⊥|² - tan²(α) * (OC · axis)²
	co->c = vec_scal(co->oc_proj, co->oc_proj) - 
		tan_angle * tan_angle * vec_scal(co->oc, cone.dir) * vec_scal(co->oc, cone.dir);

	if (fabs(co->a) < EPSILON)
		return (false);
	co->discriminant = co->half_b * co->half_b - co->a * co->c;
	if (co->discriminant < 0)
		return (false);
	co->sqrt_discriminant = sqrt(co->discriminant);
	co->t1 = (-co->half_b - co->sqrt_discriminant) / co->a;
	co->t2 = (-co->half_b + co->sqrt_discriminant) / co->a;
	co->candidate_t = -1;
	
	// Check first intersection point
	if (co->t1 > EPSILON)
	{
		co->hit_point = vec_plus(ray.origin, vec_mult(co->t1, ray.direction));
		co->to_hit = vec_minus(co->hit_point, cone.origin);
		co->height_proj = vec_scal(co->to_hit, cone.dir);
		if (co->height_proj >= 0 && co->height_proj <= cone.height)
			co->candidate_t = co->t1;
	}
	return (true);
}

bool	cone_intersection_solve(const t_ray ray, const t_cone cone,
		double *t)
{
	t_cone_inter_solve	co;
	t_vec				hit_point;
	t_vec				to_hit;
	double				height_proj;

	if (cone_intersection_solve1(&co, ray, cone) == false)
		return (false);
	
	// If first point didn't work, check second intersection point
	if (co.candidate_t < 0 && co.t2 > EPSILON)
	{
		hit_point = vec_plus(ray.origin, vec_mult(co.t2, ray.direction));
		to_hit = vec_minus(hit_point, cone.origin);
		height_proj = vec_scal(to_hit, cone.dir);
		if (height_proj >= 0 && height_proj <= cone.height)
			co.candidate_t = co.t2;
	}
	
	if (co.candidate_t > 0)
	{
		*t = co.candidate_t;
		return (true);
	}
	return (false);
}

void	cone_cap_calc(t_cone_cap *co, const t_ray ray,
		const t_cone cone, t_vec *normal)
{
	// Check intersection with the base cap plane
	if (plane_intersection_solve(ray, co->cap_plane,
			&(co->current_t)) && co->current_t > EPSILON)
	{
		co->p = vec_plus(ray.origin, vec_mult(co->current_t,
					ray.direction));
		// Check if point is within the base circle
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

bool	cone_cap_intersection(const t_ray ray,
		const t_cone cone, double *t, t_vec *normal)
{
	t_cone_cap	co;

	// Set up the base cap plane at the base of the cone (not at origin!)
	// The base is at origin + height * direction
	co.cap_plane.origin = vec_plus(cone.origin, 
		vec_mult(cone.height, cone.dir));
	co.cap_plane.normal = vec_mult(-1, cone.dir); // Normal points inward
	co.min_t = -1.0;
	cone_cap_calc(&co, ray, cone, normal);
	if (co.min_t > 0)
	{
		*t = co.min_t;
		return (true);
	}
	return (false);
}

void	init_cone_intersection(t_cone_inter *co, const t_object obj)
{
	co->cone = *((t_cone *)obj.obj);
	co->has_sol = false;
	co->final_t = -1;
	co->hit_cap = false;
} 