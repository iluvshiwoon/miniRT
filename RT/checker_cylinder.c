/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker_cylinder.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 18:39:06 by kgriset           #+#    #+#             */
/*   Updated: 2025/08/20 14:55:17 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"
#include <math.h>

static void	cylinder_local_basis(t_cylinder *cylinder, t_vec *axis,
		t_vec *right, t_vec *up)
{
	t_vec	temp;

	*axis = normalize(cylinder->dir);
	temp = (t_vec){1, 0, 0};
	if (fabs(axis->y) < 0.99)
		temp = (t_vec){0, 1, 0};
	*right = normalize(cross(temp, *axis));
	*up = normalize(cross(*axis, *right));
}

static t_checker_idx	cylinder_checker_indices(t_cyl_idx_args a)
{
	t_cone_idx		c_idx;
	t_checker_idx	idx;

	c_idx.x = vec_scal(a.to_point, a.right);
	c_idx.y = vec_scal(a.to_point, a.up);
	c_idx.z = vec_scal(a.to_point, a.axis);
	c_idx.theta = atan2(c_idx.y, c_idx.x);
	c_idx.height_ratio = c_idx.z / a.cylinder->height;
	idx.a = (int)floor((c_idx.theta + M_PI) / (2 * M_PI) * 8.0);
	idx.b = (int)floor(c_idx.height_ratio * 4.0);
	return (idx);
}

t_vec	cylinder_checker_color_from_idx(t_checker_idx idx, t_vec albedo)
{
	if ((idx.a + idx.b) % 2 == 0)
		return ((t_vec){1.0, 1.0, 1.0});
	return (albedo);
}

t_vec	get_cylinder_checkerboard(t_vec point, t_vec albedo,
		t_cylinder *cylinder)
{
	t_checkboard	c;
	t_cyl_idx_args	a;

	c.to_point = vec_minus(point, cylinder->origin);
	cylinder_local_basis(cylinder, &(c.axis), &(c.right), &(c.up));
	a = (t_cyl_idx_args){cylinder, c.to_point, c.axis, c.right, c.up};
	return (cylinder_checker_color_from_idx(cylinder_checker_indices(a),
			albedo));
}
