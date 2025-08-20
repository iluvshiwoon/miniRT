/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersections.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 18:13:15 by kgriset           #+#    #+#             */
/*   Updated: 2025/08/20 14:55:05 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"

bool	plane_intersection_solve(const t_ray ray, const t_plane plane,
		double *t)
{
	double	denominator;
	double	numerator;

	denominator = vec_scal(ray.direction, plane.normal);
	if (fabs(denominator) < EPSILON)
		return (false);
	numerator = vec_scal(vec_minus(plane.origin, ray.origin), plane.normal);
	*t = numerator / denominator;
	return (*t > EPSILON);
}

bool	sphere_second_degree_solve(const t_ray ray, const t_sphere sphere, \
		double *s)
{
	double	t[2];
	double	delta;
	double	half_b;
	double	c;

	half_b = vec_scal(ray.direction, vec_minus(ray.origin, sphere.origin));
	c = norm2(vec_minus(ray.origin, sphere.origin)) - sphere.radius
		* sphere.radius;
	delta = half_b * half_b - c;
	if (delta < 0)
		return (false);
	delta = sqrt(delta);
	t[0] = -half_b - delta;
	t[1] = -half_b + delta;
	if (t[0] > EPSILON)
		*s = t[0];
	else if (t[1] > EPSILON)
		*s = t[1];
	else
		return (false);
	return (true);
}

int	is_intersection_plane(const t_ray ray, const t_object obj,
		t_intersection *intersection)
{
	bool	has_sol;
	t_plane	plane;

	plane = *((t_plane *){obj.obj});
	has_sol = plane_intersection_solve(ray, plane, &intersection->t);
	if (has_sol == true)
	{
		intersection->point = vec_plus(ray.origin, vec_mult(intersection->t,
					ray.direction));
		intersection->normal = plane.normal;
		if (vec_scal(intersection->normal, vec_mult(-1, ray.direction)) < 0)
			intersection->normal = vec_mult(-1, intersection->normal);
	}
	return (has_sol);
}

int	is_intersection_sphere(const t_ray ray, const t_object obj,
		t_intersection *intersection)
{
	bool		has_sol;
	t_sphere	sphere;
	t_vec		to_ray_origin;

	sphere = *((t_sphere *){obj.obj});
	has_sol = sphere_second_degree_solve(ray, sphere, &intersection->t);
	if (has_sol == true)
	{
		intersection->point = vec_plus(ray.origin, vec_mult(intersection->t,
					ray.direction));
		intersection->normal = normalize(vec_minus(intersection->point,
					sphere.origin));
		to_ray_origin = normalize(vec_minus(ray.origin, intersection->point));
		if (vec_scal(intersection->normal, to_ray_origin) < 0)
			intersection->normal = vec_mult(-1, intersection->normal);
	}
	return (has_sol);
}
