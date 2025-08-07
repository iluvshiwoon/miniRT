/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder_intersection.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 17:12:12 by kgriset           #+#    #+#             */
/*   Updated: 2025/07/25 16:10:45 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"

void	determine_closest_intersection(t_cylinder_inter *cy)
{
	if (cy->body_has_sol && cy->cap_has_sol)
	{
		cy->has_sol = true;
		if (cy->body_t < cy->cap_t)
			cy->final_t = cy->body_t;
		else
		{
			cy->final_t = cy->cap_t;
			cy->hit_cap = true;
		}
	}
	else if (cy->body_has_sol)
	{
		cy->has_sol = true;
		cy->final_t = cy->body_t;
	}
	else if (cy->cap_has_sol)
	{
		cy->has_sol = true;
		cy->final_t = cy->cap_t;
		cy->hit_cap = true;
	}
}

t_vec	calculate_body_normal(const t_cylinder_inter *cy,
		const t_vec intersection_point)
{
	t_vec	to_point;
	t_vec	axis_projection;

	to_point = vec_minus(intersection_point, cy->cylinder.origin);
	axis_projection = vec_mult(vec_scal(to_point, cy->cylinder.dir),
			cy->cylinder.dir);
	return (normalize(vec_minus(to_point, axis_projection)));
}

t_vec	calculate_intersection_normal(const t_cylinder_inter *cy,
		const t_vec intersection_point, const t_ray ray)
{
	t_vec	normal;
	t_vec	to_ray_origin;

	if (cy->hit_cap)
	{
		normal = cy->cap_normal;
	}
	else
	{
		normal = calculate_body_normal(cy, intersection_point);
	}
	to_ray_origin = normalize(vec_minus(ray.origin, intersection_point));
	if (vec_scal(normal, to_ray_origin) < 0)
	{
		normal = vec_mult(-1, normal);
	}
	return (normal);
}

void	fill_intersection_data(t_intersection *intersection,
		const t_cylinder_inter *cy, const t_ray ray)
{
	intersection->t = cy->final_t;
	intersection->point = vec_plus(ray.origin, vec_mult(intersection->t,
				ray.direction));
	intersection->normal = calculate_intersection_normal(cy,
			intersection->point, ray);
	intersection->hit_cap = cy->hit_cap;
}

int	is_intersection_cylinder(const t_ray ray, const t_object obj,
		t_intersection *intersection)
{
	t_cylinder_inter	cy;

	init_cylinder_intersection(&cy, obj);
	cy.body_has_sol = cylinder_intersection_solve(ray, cy.cylinder,
			&(cy.body_t));
	cy.cap_has_sol = cylinder_cap_intersection(ray, cy.cylinder, &(cy.cap_t),
			&(cy.cap_normal));
	determine_closest_intersection(&cy);
	if (cy.has_sol)
	{
		fill_intersection_data(intersection, &cy, ray);
	}
	return (cy.has_sol);
}
