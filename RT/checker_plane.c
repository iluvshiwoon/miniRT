/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker_plane.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 19:06:16 by kgriset           #+#    #+#             */
/*   Updated: 2025/08/20 14:55:04 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"

static void	plane_local_basis(t_plane *plane, t_vec *right, t_vec *up)
{
	t_vec	temp;

	temp = (t_vec){0, 1, 0};
	if (fabs(plane->normal.y) > 0.99)
		temp = (t_vec){1, 0, 0};
	*right = normalize(cross(temp, plane->normal));
	*up = normalize(cross(plane->normal, *right));
}

static t_vec	plane_checker_color(double u, double v, t_vec albedo)
{
	int	checker_u;
	int	checker_v;

	checker_u = (int)floor(u);
	checker_v = (int)floor(v);
	if ((checker_u + checker_v) % 2 == 0)
		return ((t_vec){1.0, 1.0, 1.0});
	return (albedo);
}

t_vec	get_plane_checkerboard(t_vec point, t_vec albedo, t_plane *plane)
{
	t_vec	right;
	t_vec	up;
	double	u;
	double	v;

	plane_local_basis(plane, &right, &up);
	u = vec_scal(point, right) * 0.3;
	v = vec_scal(point, up) * 0.3;
	return (plane_checker_color(u, v, albedo));
}
