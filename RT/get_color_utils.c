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

static void	plane_local_basis(t_plane *plane, t_vec *right, t_vec *up)
{
    t_vec   temp;

    temp = (t_vec){0, 1, 0};
    if (fabs(plane->normal.y) > 0.99)
        temp = (t_vec){1, 0, 0};
    *right = normalize(cross(temp, plane->normal));
    *up = normalize(cross(plane->normal, *right));
}

static t_vec	plane_checker_color(double u, double v, t_vec albedo)
{
    int checker_u;
    int checker_v;

    checker_u = (int)floor(u);
    checker_v = (int)floor(v);
    if ((checker_u + checker_v) % 2 == 0)
        return ((t_vec){1.0, 1.0, 1.0});
    return (albedo);
}

static void	cylinder_local_basis(t_cylinder *cylinder, t_vec *axis, \
    t_vec *right, t_vec *up)
{
    t_vec   temp;

    *axis = normalize(cylinder->dir);
    temp = (fabs(axis->y) < 0.99) ? (t_vec){0, 1, 0} : (t_vec){1, 0, 0};
    *right = normalize(cross(temp, *axis));
    *up = normalize(cross(*axis, *right));
}

static t_checker_idx	cylinder_checker_indices(t_cyl_idx_args a)
{
    double  x;
    double  y;
    double  z;
    double  theta;
    double  height_ratio;
    t_checker_idx idx;

    x = vec_scal(a.to_point, a.right);
    y = vec_scal(a.to_point, a.up);
    z = vec_scal(a.to_point, a.axis);
    theta = atan2(y, x);
    height_ratio = z / a.cylinder->height;
    idx.a = (int)floor((theta + M_PI) / (2 * M_PI) * 8.0);
    idx.b = (int)floor(height_ratio * 4.0);
    return (idx);
}

static void	cone_local_basis(t_cone *cone, t_vec *axis, t_vec *right, \
    t_vec *up)
{
    t_vec   temp;

    *axis = normalize(cone->dir);
    temp = (fabs(axis->y) < 0.99) ? (t_vec){0, 1, 0} : (t_vec){1, 0, 0};
    *right = normalize(cross(temp, *axis));
    *up = normalize(cross(*axis, *right));
}

static t_checker_idx	cone_checker_indices(t_cone_idx_args a)
{
    double  x;
    double  y;
    double  z;
    double  theta;
    double  height_ratio;
    t_checker_idx idx;

    x = vec_scal(a.to_point, a.right);
    y = vec_scal(a.to_point, a.up);
    z = vec_scal(a.to_point, a.axis);
    theta = atan2(y, x);
    height_ratio = z / a.cone->height;
    idx.a = (int)floor((theta + M_PI) / (2 * M_PI) * 8.0);
    idx.b = (int)floor(height_ratio * 4.0);
    return (idx);
}

static t_vec	material_checker_dispatch(t_object *obj, t_vec point, \
    t_intersection *intersection)
{
    if (obj->type == sp)
        return (get_sphere_checkerboard(point, obj->albedo, \
            (t_sphere *)obj->obj));
    else if (obj->type == cy)
    {
        if (intersection && intersection->hit_cap)
            return (get_cylinder_cap_checkerboard(point, obj->albedo, \
                (t_cylinder *)obj->obj));
        return (get_cylinder_checkerboard(point, obj->albedo, \
            (t_cylinder *)obj->obj));
    }
    else if (obj->type == pl)
        return (get_plane_checkerboard(point, obj->albedo, \
            (t_plane *)obj->obj));
    else if (obj->type == co)
    {
        if (intersection && intersection->hit_cap)
            return (get_cone_cap_checkerboard(point, obj->albedo, \
                (t_cone *)obj->obj));
        return (get_cone_checkerboard(point, obj->albedo, \
            (t_cone *)obj->obj));
    }
    return (get_checkerboard_color(point, obj->albedo));
}

t_vec	get_material_color(t_object *obj, t_vec point, \
    t_intersection *intersection)
{
    double  u;
    double  v;

    if (obj->texture_map_path)
    {
        get_uv(obj, intersection, &u, &v);
        return (get_texture_color(obj, u, v));
    }
    if (obj->checkerboard)
        return (material_checker_dispatch(obj, point, intersection));
    return (obj->albedo);
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
