/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uv_mapping_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kershuen <kershuen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 11:00:00 by kershuen          #+#    #+#             */
/*   Updated: 2025/08/14 20:35:15 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"

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

void	get_cylinder_uv(t_object *obj, t_intersection *intersection, double *u,
		double *v)
{
	if (intersection->hit_cap)
		get_cylinder_cap_uv(obj, intersection->point, u, v);
	else
		get_cylinder_body_uv(obj, intersection->point, u, v);
}

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
	theta = atan2(vec_scal(d, normalize(cross(u_axis, cy->dir))), vec_scal(d,
				u_axis));
	y = vec_scal(d, cy->dir);
	*u = (theta + M_PI) / (2 * M_PI);
	*v = fmod(y / cy->height, 1.0);
	if (*v < 0)
		*v += 1.0;
}
