/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone_intersection.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 16:30:00 by kgriset           #+#    #+#             */
/*   Updated: 2025/07/25 16:30:00 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"

void	determine_closest_cone_intersection(t_cone_inter *co)
{
	if (co->body_has_sol && co->cap_has_sol)
	{
		co->has_sol = true;
		if (co->body_t < co->cap_t)
			co->final_t = co->body_t;
		else
		{
			co->final_t = co->cap_t;
			co->hit_cap = true;
		}
	}
	else if (co->body_has_sol)
	{
		co->has_sol = true;
		co->final_t = co->body_t;
	}
	else if (co->cap_has_sol)
	{
		co->has_sol = true;
		co->final_t = co->cap_t;
		co->hit_cap = true;
	}
}

t_vec	calculate_cone_body_normal(const t_cone_inter *co,
		const t_vec intersection_point)
{
	t_vec	to_point;
	t_vec	axis_projection;
	t_vec	radial;
	double	height_proj;
	double	tan_angle;
	double	radius_at_height;

	to_point = vec_minus(intersection_point, co->cone.origin);
	axis_projection = vec_mult(vec_scal(to_point, co->cone.dir),
			co->cone.dir);
	height_proj = vec_scal(to_point, co->cone.dir);
	
	// Calculate the tangent of the cone angle
	tan_angle = co->cone.radius / co->cone.height;
	
	// Calculate the radius at the current height
	radius_at_height = co->cone.radius * (1.0 - height_proj / co->cone.height);
	
	// The radial direction from the axis to the intersection point
	radial = vec_minus(to_point, axis_projection);
	
	// Normalize the radial component
	if (norm2(radial) > EPSILON)
		radial = normalize(radial);
	else
	{
		// If we're exactly on the axis, choose an arbitrary perpendicular direction
		t_vec temp = (fabs(co->cone.dir.y) < 0.99) ? 
			(t_vec){0, 1, 0} : (t_vec){1, 0, 0};
		radial = normalize(cross(temp, co->cone.dir));
	}
	
	// The normal is the normalized combination of radial and tangent components
	// N = normalize(radial - tan(α) * axis)
	t_vec	normal = normalize(vec_minus(radial, 
		vec_mult(tan_angle, co->cone.dir)));
	
	return (normal);
}

t_vec	calculate_cone_intersection_normal(const t_cone_inter *co,
		const t_vec intersection_point, const t_ray ray)
{
	t_vec	normal;
	t_vec	to_ray_origin;

	if (co->hit_cap)
	{
		normal = co->cap_normal;
	}
	else
	{
		normal = calculate_cone_body_normal(co, intersection_point);
	}
	to_ray_origin = normalize(vec_minus(ray.origin, intersection_point));
	if (vec_scal(normal, to_ray_origin) < 0)
	{
		normal = vec_mult(-1, normal);
	}
	return (normal);
}

void	fill_cone_intersection_data(t_intersection *intersection,
		const t_cone_inter *co, const t_ray ray)
{
	intersection->t = co->final_t;
	intersection->point = vec_plus(ray.origin, vec_mult(intersection->t,
				ray.direction));
	intersection->normal = calculate_cone_intersection_normal(co,
			intersection->point, ray);
	intersection->hit_cap = co->hit_cap;
}

int	is_intersection_cone(const t_ray ray, const t_object obj,
		t_intersection *intersection)
{
	t_cone_inter	co;

	init_cone_intersection(&co, obj);
	co.body_has_sol = cone_intersection_solve(ray, co.cone,
			&(co.body_t));
    co.cap_has_sol = cone_cap_intersection_s((t_co_cap_args){ray, co.cone, \
            &(co.cap_t), &(co.cap_normal)});
	determine_closest_cone_intersection(&co);
	if (co.has_sol)
	{
		fill_cone_intersection_data(intersection, &co, ray);
	}
	return (co.has_sol);
} 