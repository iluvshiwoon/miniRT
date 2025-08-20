/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uv_mapping.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kershuen <kershuen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 11:00:00 by kershuen          #+#    #+#             */
/*   Updated: 2025/08/20 14:54:57 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"

void	get_uv(t_object *obj, t_intersection *intersection, double *u,
		double *v)
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

static t_vec	tbn(t_vec tangent, t_vec bitangent, t_vec normal,
		t_vec map_normal)
{
	t_mat3	tbn;

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

t_vec	get_normal_from_map(t_object *obj, double u, double v, t_vec normal)
{
	t_normal	n;

	if (!obj->normal_map.img)
		return (normal);
	n.x = u * (obj->normal_map.width - 1);
	n.y = (1.0 - v) * (obj->normal_map.height - 1);
	n.dst = obj->normal_map.addr + (n.y * obj->normal_map.line_length + n.x
			* (obj->normal_map.bits_per_pixel / 8));
	n.map_normal.x = ((*(unsigned int *)n.dst >> 16) & 0xFF) / 255.0 * 2.0
		- 1.0;
	n.map_normal.y = ((*(unsigned int *)n.dst >> 8) & 0xFF) / 255.0 * 2.0 - 1.0;
	n.map_normal.z = (*(unsigned int *)n.dst & 0xFF) / 255.0 * 2.0 - 1.0;
	n.map_normal = normalize(n.map_normal);
	if (fabs(normal.y) > 0.99)
		n.tangent = normalize(cross(normal, (t_vec){0, 0, 1}));
	else
		n.tangent = normalize(cross(normal, (t_vec){0, 1, 0}));
	n.bitangent = normalize(cross(normal, n.tangent));
	return (tbn(n.tangent, n.bitangent, normal, n.map_normal));
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
	dst = obj->texture_map.addr + (y * obj->texture_map.line_length + x
			* (obj->texture_map.bits_per_pixel / 8));
	color_int = *(unsigned int *)dst;
	color.x = ((color_int >> 16) & 0xFF) / 255.0;
	color.y = ((color_int >> 8) & 0xFF) / 255.0;
	color.z = (color_int & 0xFF) / 255.0;
	return (color);
}
