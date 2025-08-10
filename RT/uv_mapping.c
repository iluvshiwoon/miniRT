/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uv_mapping.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kershuen <kershuen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 11:00:00 by kershuen          #+#    #+#             */
/*   Updated: 2025/07/26 11:00:00 by kershuen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"
#include <math.h>

void	get_sphere_uv(t_object *obj, t_vec p, double *u, double *v)
{
	t_sphere	*sphere;
	t_vec		p_local;
	double		phi;
	double		theta;

	sphere = (t_sphere *)obj->obj;
	p_local = normalize(vec_minus(p, sphere->origin));
	phi = atan2(p_local.z, p_local.x);
	theta = asin(p_local.y);
	*u = 1 - (phi + M_PI) / (2 * M_PI);
	*v = (theta + M_PI / 2) / M_PI;
}

void	get_plane_uv(t_object *obj, t_vec p, double *u, double *v)
{
	t_plane	*pl;
	t_vec	u_axis;
	t_vec	v_axis;

	pl = (t_plane *)obj->obj;
	if (fabs(pl->normal.y) > 0.99)
		u_axis = (t_vec){1, 0, 0};
	else
		u_axis = normalize(cross((t_vec){0, 1, 0}, pl->normal));
	v_axis = normalize(cross(pl->normal, u_axis));
	*u = fmod(vec_scal(p, u_axis) / obj->texture_scale.x, 1.0);
	*v = fmod(vec_scal(p, v_axis) / obj->texture_scale.y, 1.0);
	if (*u < 0)
		*u += 1.0;
	if (*v < 0)
		*v += 1.0;
}

void	get_cylinder_body_uv(t_object *obj, t_vec p, double *u, double *v)
{
	t_cylinder	*cy;
	t_vec		d;
	double		theta;
	double		y;
	t_vec		u_axis;

	cy = (t_cylinder *)obj->obj;
	d = vec_minus(p, cy->origin);
	if (fabs(cy->dir.y) > 0.99)
		u_axis = (t_vec){1, 0, 0};
	else
		u_axis = normalize(cross((t_vec){0, 1, 0}, cy->dir));
	theta = atan2(vec_scal(d, normalize(cross(u_axis, cy->dir))), vec_scal(d, u_axis));
	y = vec_scal(d, cy->dir);
	*u = (theta + M_PI) / (2 * M_PI);
	*v = fmod(y / cy->height, 1.0);
	if (*v < 0)
		*v += 1.0;
}

void	get_cylinder_cap_uv(t_object *obj, t_vec p, double *u, double *v)
{
	t_cylinder	*cy;
	t_vec		d;
	t_vec		u_axis;
	t_vec		v_axis;

	cy = (t_cylinder *)obj->obj;
	d = vec_minus(p, cy->origin);
	if (fabs(cy->dir.y) > 0.99)
		u_axis = (t_vec){1, 0, 0};
	else
		u_axis = normalize(cross((t_vec){0, 1, 0}, cy->dir));
	v_axis = normalize(cross(cy->dir, u_axis));
	*u = (vec_scal(d, u_axis) / cy->radius + 1.0) / 2.0;
	*v = (vec_scal(d, v_axis) / cy->radius + 1.0) / 2.0;
}

void	get_cylinder_uv(t_object *obj, t_intersection *intersection, double *u, double *v)
{
	if (intersection->hit_cap)
		get_cylinder_cap_uv(obj, intersection->point, u, v);
	else
		get_cylinder_body_uv(obj, intersection->point, u, v);
}

static void get_cone_body_uv(t_object *obj, t_vec p, double *u, double *v)
{
    t_cone  *co;
    t_vec   d;
    t_vec   u_axis;
    t_vec   v_axis;
    t_vec   axis_component;
    t_vec   radial;
    double  h;
    double  theta;

    co = (t_cone *)obj->obj;
    d = vec_minus(p, co->origin);

    if (fabs(co->dir.y) > 0.99)
        u_axis = (t_vec){1, 0, 0};
    else
        u_axis = normalize(cross((t_vec){0, 1, 0}, co->dir));
    v_axis = normalize(cross(co->dir, u_axis));

    h = vec_scal(d, co->dir);
    if (h < 0.0)
        h = 0.0;
    if (h > co->height)
        h = co->height;

    axis_component = vec_mult(h, co->dir);
    radial = vec_minus(d, axis_component);

    // Handle degeneracy near the apex
    if (norm2(radial) < 1e-12)
        theta = 0.0;
    else
        theta = atan2(vec_scal(radial, v_axis), vec_scal(radial, u_axis));

    *u = (theta + M_PI) / (2.0 * M_PI);
    *v = h / co->height;

    // Apply tiling via texture_scale (avoid div by zero)
    if (obj->texture_scale.x != 0.0)
        *u = fmod(*u / obj->texture_scale.x, 1.0);
    else
        *u = fmod(*u, 1.0);

    if (obj->texture_scale.y != 0.0)
        *v = fmod(*v / obj->texture_scale.y, 1.0);
    else
        *v = fmod(*v, 1.0);

    if (*u < 0.0) *u += 1.0;
    if (*v < 0.0) *v += 1.0;
}

static void get_cone_cap_uv(t_object *obj, t_vec p, double *u, double *v)
{
    t_cone  *co;
    t_vec   base_center;
    t_vec   u_axis;
    t_vec   v_axis;
    t_vec   d;

    co = (t_cone *)obj->obj;
    base_center = vec_plus(co->origin, vec_mult(co->height, co->dir));
    d = vec_minus(p, base_center);

    if (fabs(co->dir.y) > 0.99)
        u_axis = (t_vec){1, 0, 0};
    else
        u_axis = normalize(cross((t_vec){0, 1, 0}, co->dir));
    v_axis = normalize(cross(co->dir, u_axis));

    *u = (vec_scal(d, u_axis) / co->radius + 1.0) / 2.0;
    *v = (vec_scal(d, v_axis) / co->radius + 1.0) / 2.0;

    // Optional tiling on cap (keep behavior similar to cylinder caps = no tiling)
    if (*u < 0.0) *u = 0.0;
    if (*u > 1.0) *u = 1.0;
    if (*v < 0.0) *v = 0.0;
    if (*v > 1.0) *v = 1.0;
}

void    get_cone_uv(t_object *obj, t_intersection *intersection, double *u, double *v)
{
    if (intersection->hit_cap)
        get_cone_cap_uv(obj, intersection->point, u, v);
    else
        get_cone_body_uv(obj, intersection->point, u, v);
}

void	get_uv(t_object *obj, t_intersection *intersection, double *u, double *v)
{
	if (obj->type == sp)
		get_sphere_uv(obj, intersection->point, u, v);
	else if (obj->type == pl)
		get_plane_uv(obj, intersection->point, u, v);
	else if (obj->type == cy)
		get_cylinder_uv(obj, intersection, u, v);
    else if (obj->type == co)
        get_cone_uv(obj, intersection, u, v);
	else
	{
		*u = 0;
		*v = 0;
	}
}

t_vec	get_normal_from_map(t_object *obj, double u, double v, t_vec normal)
{
	int		x;
	int		y;
	char	*dst;
	t_vec	map_normal;
	t_vec	tangent;
	t_vec	bitangent;
	t_mat3	tbn;

	if (!obj->normal_map.img)
		return (normal);
	x = u * (obj->normal_map.width - 1);
	y = (1.0 - v) * (obj->normal_map.height - 1);
	dst = obj->normal_map.addr + (y * obj->normal_map.line_length + x * (obj->normal_map.bits_per_pixel / 8));
	map_normal.x = ((*(unsigned int *)dst >> 16) & 0xFF) / 255.0 * 2.0 - 1.0;
	map_normal.y = ((*(unsigned int *)dst >> 8) & 0xFF) / 255.0 * 2.0 - 1.0;
	map_normal.z = (*(unsigned int *)dst & 0xFF) / 255.0 * 2.0 - 1.0;
	map_normal = normalize(map_normal);
	if (fabs(normal.y) > 0.99)
		tangent = normalize(cross(normal, (t_vec){0, 0, 1}));
	else
		tangent = normalize(cross(normal, (t_vec){0, 1, 0}));
	bitangent = normalize(cross(normal, tangent));
	tbn.m[0][0] = tangent.x;
	tbn.m[0][1] = bitangent.x;
	tbn.m[0][2] = normal.x;
	tbn.m[1][0] = tangent.y;
	tbn.m[1][1] = bitangent.y;
	tbn.m[1][2] = normal.y;
	tbn.m[2][0] = tangent.z;
	tbn.m[2][1] = bitangent.z;
	tbn.m[2][2] = normal.z;
	return (normalize(mat3_multiply_vec(tbn, map_normal)));
}

t_vec	get_texture_color(t_object *obj, double u, double v)
{
	int		x;
	int		y;
	char	*dst;
	t_vec	color;
	int		color_int;

	x = u * (obj->texture_map.width - 1);
	y = (1.0 - v) * (obj->texture_map.height - 1);
	dst = obj->texture_map.addr + (y * obj->texture_map.line_length + x * (obj->texture_map.bits_per_pixel / 8));
	color_int = *(unsigned int *)dst;
	color.x = ((color_int >> 16) & 0xFF) / 255.0;
	color.y = ((color_int >> 8) & 0xFF) / 255.0;
	color.z = (color_int & 0xFF) / 255.0;
	return (color);
} 