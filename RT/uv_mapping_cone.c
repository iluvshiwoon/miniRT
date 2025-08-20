/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uv_mapping_cone.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 20:42:31 by kgriset           #+#    #+#             */
/*   Updated: 2025/08/20 14:55:05 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"

static void	get_cone_body_uv1(t_cone_uv c, double *u, double *v, t_object *obj)
{
	if (norm2(c.radial) < 1e-12)
		c.theta = 0.0;
	else
		c.theta = atan2(vec_scal(c.radial, c.v_axis), vec_scal(c.radial,
					c.u_axis));
	*u = (c.theta + M_PI) / (2.0 * M_PI);
	*v = c.h / c.co->height;
	if (obj->texture_scale.x != 0.0)
		*u = fmod(*u / obj->texture_scale.x, 1.0);
	else
		*u = fmod(*u, 1.0);
	if (obj->texture_scale.y != 0.0)
		*v = fmod(*v / obj->texture_scale.y, 1.0);
	else
		*v = fmod(*v, 1.0);
	if (*u < 0.0)
		*u += 1.0;
	if (*v < 0.0)
		*v += 1.0;
}

static void	get_cone_body_uv(t_object *obj, t_vec p, double *u, double *v)
{
	t_cone_uv	c;

	c.co = (t_cone *)obj->obj;
	c.d = vec_minus(p, c.co->origin);
	if (fabs(c.co->dir.y) > 0.99)
		c.u_axis = (t_vec){1, 0, 0};
	else
		c.u_axis = normalize(cross((t_vec){0, 1, 0}, c.co->dir));
	c.v_axis = normalize(cross(c.co->dir, c.u_axis));
	c.h = vec_scal(c.d, c.co->dir);
	if (c.h < 0.0)
		c.h = 0.0;
	if (c.h > c.co->height)
		c.h = c.co->height;
	c.axis_component = vec_mult(c.h, c.co->dir);
	c.radial = vec_minus(c.d, c.axis_component);
	get_cone_body_uv1(c, u, v, obj);
}

static void	get_cone_cap_uv(t_object *obj, t_vec p, double *u, double *v)
{
	t_cap_uv	c;

	c.co = (t_cone *)obj->obj;
	c.base_center = vec_plus(c.co->origin, vec_mult(c.co->height, c.co->dir));
	c.d = vec_minus(p, c.base_center);
	if (fabs(c.co->dir.y) > 0.99)
		c.u_axis = (t_vec){1, 0, 0};
	else
		c.u_axis = normalize(cross((t_vec){0, 1, 0}, c.co->dir));
	c.v_axis = normalize(cross(c.co->dir, c.u_axis));
	*u = (vec_scal(c.d, c.u_axis) / c.co->radius + 1.0) / 2.0;
	*v = (vec_scal(c.d, c.v_axis) / c.co->radius + 1.0) / 2.0;
	if (*u < 0.0)
		*u = 0.0;
	if (*u > 1.0)
		*u = 1.0;
	if (*v < 0.0)
		*v = 0.0;
	if (*v > 1.0)
		*v = 1.0;
}

void	get_cone_uv(t_object *obj, t_intersection *intersection, double *u,
		double *v)
{
	if (intersection->hit_cap)
		get_cone_cap_uv(obj, intersection->point, u, v);
	else
		get_cone_body_uv(obj, intersection->point, u, v);
}
