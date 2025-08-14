/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uv_mapping.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kershuen <kershuen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 11:00:00 by kershuen          #+#    #+#             */
/*   Updated: 2025/08/14 19:08:55 by kgriset          ###   ########.fr       */
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
