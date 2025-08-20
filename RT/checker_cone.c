/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker_cone.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 18:39:06 by kgriset           #+#    #+#             */
/*   Updated: 2025/08/20 14:55:04 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"
#include <math.h>

void	cone_local_basis(t_cone *cone, t_vec *axis, t_vec *right, t_vec *up)
{
	t_vec	temp;

	*axis = normalize(cone->dir);
	temp = (t_vec){1, 0, 0};
	if (fabs(axis->y) < 0.99)
		temp = (t_vec){0, 1, 0};
	*right = normalize(cross(temp, *axis));
	*up = normalize(cross(*axis, *right));
}

t_checker_idx	cone_checker_indices(t_cone_idx_args a)
{
	t_cone_idx		c_idx;
	t_checker_idx	idx;

	c_idx.x = vec_scal(a.to_point, a.right);
	c_idx.y = vec_scal(a.to_point, a.up);
	c_idx.z = vec_scal(a.to_point, a.axis);
	c_idx.theta = atan2(c_idx.y, c_idx.x);
	c_idx.height_ratio = c_idx.z / a.cone->height;
	idx.a = (int)floor((c_idx.theta + M_PI) / (2 * M_PI) * 8.0);
	idx.b = (int)floor(c_idx.height_ratio * 4.0);
	return (idx);
}
