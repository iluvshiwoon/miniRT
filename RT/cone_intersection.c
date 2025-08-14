/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone_intersection.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 16:30:00 by kgriset           #+#    #+#             */
/*   Updated: 2025/08/14 19:39:09 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"
#include <math.h>

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
	t_calc_cone	calc;

	calc.to_point = vec_minus(intersection_point, co->cone.origin);
	calc.axis_projection = vec_mult(vec_scal(calc.to_point, co->cone.dir),
			co->cone.dir);
	calc.height_proj = vec_scal(calc.to_point, co->cone.dir);
	calc.tan_angle = co->cone.radius / co->cone.height;
	calc.radius_at_height = co->cone.radius * (1.0 - calc.height_proj
			/ co->cone.height);
	calc.radial = vec_minus(calc.to_point, calc.axis_projection);
	if (norm2(calc.radial) > EPSILON)
		calc.radial = normalize(calc.radial);
	else
	{
		calc.temp = (t_vec){1, 0, 0};
		if (fabs(co->cone.dir.y) < 0.99)
			calc.temp = (t_vec){0, 1, 0};
		calc.radial = normalize(cross(calc.temp, co->cone.dir));
	}
	calc.normal = normalize(vec_minus(calc.radial, vec_mult(calc.tan_angle,
					co->cone.dir)));
	return (calc.normal);
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
	co.body_has_sol = cone_intersection_solve(ray, co.cone, &(co.body_t));
	co.cap_has_sol = cone_cap_intersection_s((t_co_cap_args){ray, co.cone,
			&(co.cap_t), &(co.cap_normal)});
	determine_closest_cone_intersection(&co);
	if (co.has_sol)
	{
		fill_cone_intersection_data(intersection, &co, ray);
	}
	return (co.has_sol);
}
