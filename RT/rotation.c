/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotation.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 18:09:28 by kgriset           #+#    #+#             */
/*   Updated: 2025/08/20 14:55:04 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"

t_vec	get_camera_right(t_camera cam)
{
	return (normalize(cross(cam.direction, cam.up)));
}

t_vec	get_camera_up(t_camera cam)
{
	return (normalize(cam.up));
}

t_mat3	create_rotation_axis(t_vec axis, double angle)
{
	t_mat3	result;
	double	c;
	double	s;
	double	one_minus_c;

	axis = normalize(axis);
	c = cos(angle);
	s = sin(angle);
	one_minus_c = 1.0 - c;
	result.m[0][0] = c + axis.x * axis.x * one_minus_c;
	result.m[0][1] = axis.x * axis.y * one_minus_c - axis.z * s;
	result.m[0][2] = axis.x * axis.z * one_minus_c + axis.y * s;
	result.m[1][0] = axis.y * axis.x * one_minus_c + axis.z * s;
	result.m[1][1] = c + axis.y * axis.y * one_minus_c;
	result.m[1][2] = axis.y * axis.z * one_minus_c - axis.x * s;
	result.m[2][0] = axis.z * axis.x * one_minus_c - axis.y * s;
	result.m[2][1] = axis.z * axis.y * one_minus_c + axis.x * s;
	result.m[2][2] = c + axis.z * axis.z * one_minus_c;
	return (result);
}

t_vec	mat3_multiply_vec(t_mat3 m, t_vec v)
{
	return ((t_vec){m.m[0][0] * v.x + m.m[0][1] * v.y + m.m[0][2] * v.z,
		m.m[1][0] * v.x + m.m[1][1] * v.y + m.m[1][2] * v.z, m.m[2][0] * v.x
		+ m.m[2][1] * v.y + m.m[2][2] * v.z});
}
