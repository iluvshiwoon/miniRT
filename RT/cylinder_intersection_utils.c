/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder_intersection_utils.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 17:17:30 by kgriset           #+#    #+#             */
/*   Updated: 2025/07/24 16:18:35 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"

bool	cylinder_intersection_solve1(t_cylinder_inter_solve *cy,
		const t_ray ray, const t_cylinder cylinder)
{
	cy->ray_dir_proj = vec_minus(ray.direction, vec_mult(vec_scal(ray.direction,
					cylinder.direction), cylinder.direction));
	cy->a = vec_scal(cy->ray_dir_proj, cy->ray_dir_proj);
	cy->half_b = vec_scal(cy->ray_dir_proj, cy->oc_proj);
	cy->c = vec_scal(cy->oc_proj, cy->oc_proj) - cylinder.radius
		* cylinder.radius;
	if (fabs(cy->a) < EPSILON)
		return (false);
	cy->discriminant = cy->half_b * cy->half_b - cy->a * cy->c;
	if (cy->discriminant < 0)
		return (false);
	cy->sqrt_discriminant = sqrt(cy->discriminant);
	cy->t1 = (-cy->half_b - cy->sqrt_discriminant) / cy->a;
	cy->t2 = (-cy->half_b + cy->sqrt_discriminant) / cy->a;
	cy->candidate_t = -1;
	if (cy->t1 > EPSILON)
	{
		cy->hit_point = vec_plus(ray.origin, vec_mult(cy->t1, ray.direction));
		cy->to_hit = vec_minus(cy->hit_point, cylinder.origin);
		cy->height_proj = vec_scal(cy->to_hit, cylinder.direction);
		if (cy->height_proj >= 0 && cy->height_proj <= cylinder.height)
			cy->candidate_t = cy->t1;
	}
	return (true);
}

bool	cylinder_intersection_solve(const t_ray ray, const t_cylinder cylinder,
		double *t)
{
	t_cylinder_inter_solve	cy;
	t_vec					hit_point;
	t_vec					to_hit;
	double					height_proj;

	cy.oc = vec_minus(ray.origin, cylinder.origin);
	cy.oc_proj = vec_minus(cy.oc, vec_mult(vec_scal(cy.oc, cylinder.direction),
				cylinder.direction));
	if (cylinder_intersection_solve1(&cy, ray, cylinder) == false)
		return (false);
	if (cy.candidate_t < 0 && cy.t2 > EPSILON)
	{
		hit_point = vec_plus(ray.origin, vec_mult(cy.t2, ray.direction));
		to_hit = vec_minus(hit_point, cylinder.origin);
		height_proj = vec_scal(to_hit, cylinder.direction);
		if (height_proj >= 0 && height_proj <= cylinder.height)
			cy.candidate_t = cy.t2;
	}
	if (cy.candidate_t > 0)
	{
		*t = cy.candidate_t;
		return (true);
	}
	return (false);
}

void	cylinder_cap_calc(t_cylinder_cap *cy, const t_ray ray,
		const t_cylinder cylinder, t_vec *normal)
{
	cy->i = -1;
	while (++(cy->i) < 2)
	{
		if (plane_intersection_solve(ray, cy->cap_planes[cy->i],
				&(cy->current_t)) && cy->current_t > EPSILON)
		{
			cy->p = vec_plus(ray.origin, vec_mult(cy->current_t,
						ray.direction));
			if (norm2(vec_minus(cy->p,
						cy->cap_planes[cy->i].origin)) <= cylinder.radius
				* cylinder.radius)
			{
				if (cy->min_t < 0 || cy->current_t < cy->min_t)
				{
					cy->min_t = cy->current_t;
					*normal = cy->cap_planes[cy->i].normal;
				}
			}
		}
	}
}

bool	cylinder_cap_intersection(const t_ray ray,
		const t_cylinder cylinder, double *t, t_vec *normal)
{
	t_cylinder_cap	cy;

	cy.cap_planes[0].origin = cylinder.origin;
	cy.cap_planes[0].normal = vec_mult(-1, cylinder.direction);
	cy.cap_planes[1].origin = vec_plus(cylinder.origin,
			vec_mult(cylinder.height, cylinder.direction));
	cy.cap_planes[1].normal = cylinder.direction;
	cy.min_t = -1.0;
	cylinder_cap_calc(&cy, ray, cylinder, normal);
	if (cy.min_t > 0)
	{
		*t = cy.min_t;
		return (true);
	}
	return (false);
}

void	init_cylinder_intersection(t_cylinder_inter *cy, const t_object obj)
{
	cy->cylinder = *((t_cylinder *)obj.obj);
	cy->has_sol = false;
	cy->final_t = -1;
	cy->hit_cap = false;
}
