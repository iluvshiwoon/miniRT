/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_color_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 19:55:30 by kgriset           #+#    #+#             */
/*   Updated: 2025/07/18 20:09:02 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"

t_vec	calculate_direct_lighting(t_rt *rt, t_get_color *gc)
{
	t_vec	total_light_contribution;
	int		i;

	total_light_contribution = (t_vec){0, 0, 0};
	i = 0;
	while (i < rt->scene.num_lights)
	{
		gc->ray_light = (t_ray){vec_plus(gc->intersection.point,
				vec_mult(EPSILON, gc->intersection.normal)),
			normalize(vec_minus(rt->scene.lights[i]->origin,
					gc->intersection.point))};
		gc->obj_id_light = 0;
		gc->inter = visible_intersection(gc->ray_light, rt->scene,
				&(gc->intersection_light), &(gc->obj_id_light));
		gc->d_light2 = norm2(vec_minus(rt->scene.lights[i]->origin,
					gc->intersection.point));
		if (!(gc->inter && gc->intersection_light.t
			* gc->intersection_light.t < gc->d_light2))
		{
			gc->cos_theta = fmax(0, vec_scal(normalize(
						vec_minus(rt->scene.lights[i]->origin,
							gc->intersection.point)), gc->intersection.normal));
			gc->light_contribution = vec_mult(rt->scene.lights[i]->intensity
					* EXPOSURE * gc->cos_theta / gc->d_light2,
					rt->scene.lights[i]->color);
			total_light_contribution = vec_plus(total_light_contribution,
					gc->light_contribution);
		}
		i++;
	}
	return (vec_mult(1 / M_PI, vec_m_vec(total_light_contribution,
				get_material_color(&rt->scene.objects[gc->obj_id],
					gc->intersection.point, &gc->intersection))));
}

t_vec	calculate_ambient_lighting(t_rt *rt, t_get_color *gc)
{
	gc->ambient_contribution = vec_mult(GLOBAL_EXPOSURE
			* rt->scene.ambient_light.intensity, rt->scene.ambient_light.color);
	return (vec_mult(1 / M_PI, vec_m_vec(gc->ambient_contribution,
				get_material_color(&rt->scene.objects[gc->obj_id], gc->intersection.point, &gc->intersection))));
}

t_vec	calculate_specular_reflection(t_rt *rt, t_get_color *gc,
		t_vec view_direction)
{
	t_vec	total_specular_contribution;
	int		i;

	total_specular_contribution = (t_vec){0, 0, 0};
	i = 0;
	while (i < rt->scene.num_lights)
	{
		gc->ray_light = (t_ray){vec_plus(gc->intersection.point,
				vec_mult(EPSILON, gc->intersection.normal)),
			normalize(vec_minus(rt->scene.lights[i]->origin,
					gc->intersection.point))};
		gc->obj_id_light = 0;
		gc->inter = visible_intersection(gc->ray_light, rt->scene,
				&(gc->intersection_light), &(gc->obj_id_light));
		gc->d_light2 = norm2(vec_minus(rt->scene.lights[i]->origin,
					gc->intersection.point));
		if (!(gc->inter && gc->intersection_light.t
			* gc->intersection_light.t < gc->d_light2))
		{
			gc->light_direction = normalize(
					vec_minus(rt->scene.lights[i]->origin,
						gc->intersection.point));
			gc->half_vector = normalize(vec_plus(gc->light_direction,
						view_direction));
			gc->cos_alpha = fmax(0, vec_scal(gc->half_vector,
						gc->intersection.normal));
			gc->cos_alpha = pow(gc->cos_alpha,
					rt->scene.objects[gc->obj_id].shininess);
			gc->specular_contribution = vec_mult(rt->scene.lights[i]->intensity
					* EXPOSURE * gc->cos_alpha / gc->d_light2,
					rt->scene.lights[i]->color);
			total_specular_contribution = vec_plus(total_specular_contribution,
					gc->specular_contribution);
		}
		i++;
	}
	return (vec_m_vec(total_specular_contribution,
			rt->scene.objects[gc->obj_id].specular));
}

t_vec	get_plane_checkerboard(t_vec point, t_vec albedo, t_plane *plane)
{
	t_vec	right, up;
	double	u, v;
	t_vec	checker_color;
	t_vec	temp;

	// Create local coordinate system for the plane
	temp = (t_vec){0, 1, 0};
	if (fabs(plane->normal.y) > 0.99)
		temp = (t_vec){1, 0, 0};
	right = normalize(cross(temp, plane->normal));
	up = normalize(cross(plane->normal, right));
	
	// Project the intersection point onto the plane's coordinate system
	u = vec_scal(point, right) * 0.3;
	v = vec_scal(point, up) * 0.3;
	
	// Use floor to avoid boundary issues and create consistent squares
	int checker_u = (int)floor(u);
	int checker_v = (int)floor(v);
	
	// Create alternating pattern
	if ((checker_u + checker_v) % 2 == 0)
		checker_color = (t_vec){1.0, 1.0, 1.0};
	else
		checker_color = albedo;
	
	return (checker_color);
}

t_vec	get_checkerboard_color(t_vec point, t_vec albedo)
{
	int		checker_x, checker_y, checker_z;
	t_vec	checker_color;
	double	scale;

	scale = 0.5; // Bigger squares for planes
	checker_x = (int)(point.x * scale);
	checker_y = (int)(point.y * scale);
	checker_z = (int)(point.z * scale);
	
	// Create alternating pattern based on sum of coordinates
	if ((checker_x + checker_y + checker_z) % 2 == 0)
	{
		// White squares
		checker_color = (t_vec){1.0, 1.0, 1.0};
	}
	else
	{
		// Colored squares (use the original albedo)
		checker_color = albedo;
	}
	
	return (checker_color);
}

t_vec	get_sphere_checkerboard(t_vec point, t_vec albedo, t_sphere *sphere)
{
	double	u, v;
	int		checker_u, checker_v;
	t_vec	checker_color;
	double	scale;

	// Convert 3D point to spherical coordinates (u, v)
	u = atan2(point.z - sphere->origin.z, point.x - sphere->origin.x) / (2 * M_PI);
	v = acos((point.y - sphere->origin.y) / sphere->radius) / M_PI;
	
	scale = 8.0; // Number of divisions around the sphere
	checker_u = (int)(u * scale);
	checker_v = (int)(v * scale);
	
	if ((checker_u + checker_v) % 2 == 0)
		checker_color = (t_vec){1.0, 1.0, 1.0};
	else
		checker_color = albedo;
	
	return (checker_color);
}

t_vec	get_cylinder_checkerboard(t_vec point, t_vec albedo, t_cylinder *cylinder)
{
	t_vec	axis = normalize(cylinder->dir);
	t_vec	to_point = vec_minus(point, cylinder->origin);

	// Find a vector perpendicular to the axis for the local basis
	t_vec	temp = (fabs(axis.y) < 0.99) ? (t_vec){0, 1, 0} : (t_vec){1, 0, 0};
	t_vec	right = normalize(cross(temp, axis));
	t_vec	up = normalize(cross(axis, right));

	// Project to_point into the local basis
	double	x = vec_scal(to_point, right);
	double	y = vec_scal(to_point, up);
	double	z = vec_scal(to_point, axis);

	// Cylindrical coordinates: angle around axis and height along axis
	double	theta = atan2(y, x); // [-pi, pi]
	double	height_ratio = z / cylinder->height; // [0, 1]

	// Scale for proper square aspect ratio
	// Circumference = 2πr, so we need to scale theta accordingly
	double	circumference = 2 * M_PI * cylinder->radius;
	double	theta_scale = 8.0; // number of divisions around
	double	height_scale = 4.0; // number of divisions along height

	int	checker_theta = (int)floor((theta + M_PI) / (2 * M_PI) * theta_scale);
	int	checker_height = (int)floor(height_ratio * height_scale);

	if ((checker_theta + checker_height) % 2 == 0)
		return ((t_vec){1.0, 1.0, 1.0});
	else
		return (albedo);
}

t_vec	get_cylinder_cap_checkerboard(t_vec point, t_vec albedo, t_cylinder *cylinder)
{
	// Caps use solid color, no checkerboard pattern
	return (albedo);
}

t_vec	get_cone_checkerboard(t_vec point, t_vec albedo, t_cone *cone)
{
	t_vec	axis = normalize(cone->dir);
	t_vec	to_point = vec_minus(point, cone->origin);

	// Find a vector perpendicular to the axis for the local basis
	t_vec	temp = (fabs(axis.y) < 0.99) ? (t_vec){0, 1, 0} : (t_vec){1, 0, 0};
	t_vec	right = normalize(cross(temp, axis));
	t_vec	up = normalize(cross(axis, right));

	// Project to_point into the local basis
	double	x = vec_scal(to_point, right);
	double	y = vec_scal(to_point, up);
	double	z = vec_scal(to_point, axis);

	// Cone coordinates: angle around axis and height along axis
	double	theta = atan2(y, x); // [-pi, pi]
	double	height_ratio = z / cone->height; // [0, 1]

	// Scale for proper square aspect ratio
	// Circumference varies with height, so we need to scale accordingly
	double	radius_at_height = cone->radius * (1.0 - height_ratio);
	double	circumference = 2 * M_PI * radius_at_height;
	double	theta_scale = 8.0; // number of divisions around
	double	height_scale = 4.0; // number of divisions along height

	int	checker_theta = (int)floor((theta + M_PI) / (2 * M_PI) * theta_scale);
	int	checker_height = (int)floor(height_ratio * height_scale);

	if ((checker_theta + checker_height) % 2 == 0)
		return ((t_vec){1.0, 1.0, 1.0});
	else
		return (albedo);
}

t_vec	get_cone_cap_checkerboard(t_vec point, t_vec albedo, t_cone *cone)
{
	// Base cap uses solid color, no checkerboard pattern
	return (albedo);
}

t_vec	get_material_color(t_object *obj, t_vec point, t_intersection *intersection)
{
	double	u;
	double	v;

	if (obj->texture_map_path)
	{
		get_uv(obj, point, &u, &v);
		return (get_texture_color(obj, u, v));
	}
	if (!obj->checkerboard)
		return (obj->albedo);
	
	// Use different patterns based on object type
	if (obj->type == sp)
		return (get_sphere_checkerboard(point, obj->albedo, (t_sphere *)obj->obj));
	else if (obj->type == cy)
	{
		if (intersection && intersection->hit_cap)
			return (get_cylinder_cap_checkerboard(point, obj->albedo, (t_cylinder *)obj->obj));
		else
			return (get_cylinder_checkerboard(point, obj->albedo, (t_cylinder *)obj->obj));
	}
	else if (obj->type == pl)
		return (get_plane_checkerboard(point, obj->albedo, (t_plane *)obj->obj));
	else if (obj->type == co)
	{
		if (intersection && intersection->hit_cap)
			return (get_cone_cap_checkerboard(point, obj->albedo, (t_cone *)obj->obj));
		else
			return (get_cone_checkerboard(point, obj->albedo, (t_cone *)obj->obj));
	}
	else
		return (get_checkerboard_color(point, obj->albedo)); // For other objects
}

t_vec	generate_random_hemisphere_direction(t_vec normal, t_pcg32_random *rng)
{
	t_get_color	gc;

	gc.r1 = double_rng(rng);
	gc.r2 = double_rng(rng);
	gc.direction_random_local_basis = (t_vec){cos(2 * M_PI * gc.r1) * sqrt(1
			- gc.r2), sin(2 * M_PI * gc.r1) * sqrt(1 - gc.r2), sqrt(gc.r2)};
	gc.random_vec = (t_vec){double_rng(rng) - 0.5, double_rng(rng)
		- 0.5, double_rng(rng) - 0.5};
	gc.tangent1 = normalize(cross(normal, gc.random_vec));
	gc.tangent2 = cross(gc.tangent1, normal);
	return (vec_plus(vec_plus(vec_mult(gc.direction_random_local_basis.x,
					gc.tangent1), vec_mult(gc.direction_random_local_basis.y,
					gc.tangent2)), vec_mult(gc.direction_random_local_basis.z,
				normal)));
}

t_vec	calculate_recursive_reflection(t_rt *rt, t_get_color *gc,
		int nb_rebound, t_pcg32_random *rng)
{
	t_vec	direction_random;
	t_ray	random_ray;
	t_vec	recursive_color;

	direction_random = generate_random_hemisphere_direction(gc->intersection.normal, rng);
	random_ray = (t_ray){vec_plus(gc->intersection.point, vec_mult(0.001,
				gc->intersection.normal)), direction_random};
	recursive_color = get_color(random_ray, rt, nb_rebound - 1, rng);
	return (vec_m_vec(recursive_color, get_material_color(&rt->scene.objects[gc->obj_id], gc->intersection.point, &gc->intersection)));
}
