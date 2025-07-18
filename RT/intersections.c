/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersections.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 18:13:15 by kgriset           #+#    #+#             */
/*   Updated: 2025/07/18 20:08:59 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"
#define EXPOSURE 300000000.0
#define GLOBAL_EXPOSURE 1000000.0
#define EPSILON 1e-6

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

static bool	cylinder_cap_intersection(const t_ray ray,
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

typedef struct cylinder_inter
{
	t_cylinder	cylinder;
	double		body_t;
	bool		body_has_sol;
	double		cap_t;
	t_vec		cap_normal;
	bool		cap_has_sol;
	bool		has_sol;
	double		final_t;
	bool		hit_cap;
	t_vec		to_point;
	t_vec		axis_projection;
	t_vec		to_ray_origin;

}				t_cylinder_inter;

void	init_cylinder_intersection(t_cylinder_inter *cy, const t_object obj)
{
	cy->cylinder = *((t_cylinder *)obj.obj);
	cy->has_sol = false;
	cy->final_t = -1;
	cy->hit_cap = false;
}

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
	axis_projection = vec_mult(vec_scal(to_point, cy->cylinder.direction),
			cy->cylinder.direction);
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
}

bool	is_intersection_cylinder(const t_ray ray, const t_object obj,
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

bool	sphere_second_degree_solve(const t_ray ray, const t_sphere sphere,
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

bool	is_intersection_plane(const t_ray ray, const t_object obj,
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

bool	is_intersection_sphere(const t_ray ray, const t_object obj,
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

bool	visible_intersection(const t_ray ray, t_scene scene,
		t_intersection *intersection, int *obj_id)
{
	t_intersection	local_intersection;
	t_object		*obj;
	bool			has_inter;

	intersection->t = 1e99;
	has_inter = false;
	while (scene.total_objects)
	{
		obj = &scene.objects[scene.total_objects - 1];
		if (obj->is_intersection != NULL && obj->is_intersection(ray, *obj,
				&local_intersection))
		{
			has_inter = true;
			if (local_intersection.t < intersection->t)
			{
				intersection->t = local_intersection.t;
				intersection->point = local_intersection.point;
				intersection->normal = local_intersection.normal;
				*obj_id = scene.total_objects - 1;
			}
		}
		scene.total_objects--;
	}
	return (has_inter);
}



t_vec	get_color(t_ray ray, t_rt *rt, int nb_rebound)
{
	t_get_color	gc;
	t_vec		direct_light;
	t_vec		ambient_light;
	t_vec		reflection;

	gc.pixel = (t_vec){};
	gc.obj_id = 0;
	if (nb_rebound == 0)
		return ((t_vec){0, 0, 0});
	if (!visible_intersection(ray, rt->scene, &(gc.intersection), &(gc.obj_id)))
		return ((t_vec){0, 0, 0});
	direct_light = calculate_direct_lighting(rt, &gc);
	ambient_light = calculate_ambient_lighting(rt, &gc);
	gc.pixel = vec_plus(direct_light, ambient_light);
	reflection = calculate_recursive_reflection(rt, &gc, nb_rebound);
	gc.pixel = vec_plus(gc.pixel, reflection);
	return (gc.pixel);
}
