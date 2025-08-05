/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gschwand <gschwand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 16:35:47 by gschwand          #+#    #+#             */
/*   Updated: 2025/07/25 13:20:07 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../miniRT.h"

void	rotate_cylinder_local(t_rt *rt, int id, t_rvec rvec)
{
	t_cl	cl;

	cl.cyl = rt->scene.objects[id].obj;
	cl.axis = normalize(cl.cyl->dir);
	cl.temp = (t_vec){0, 1, 0};
	if (fabs(cl.axis.y) > 0.99)
		cl.temp = (t_vec){1, 0, 0};
	cl.right = normalize(cross(cl.temp, cl.axis));
	cl.forward = cross(cl.axis, cl.right);
	if (fabs(rvec.pitch) > 1e-6)
	{
		cl.pitch_rot = create_rotation_axis(cl.right, rvec.pitch);
		cl.cyl->dir = normalize(mat3_multiply_vec(cl.pitch_rot,
					cl.cyl->dir));
	}
	if (fabs(rvec.yaw) > 1e-6)
	{
		cl.yaw_rot = create_rotation_axis(cl.forward, rvec.yaw);
		cl.cyl->dir = normalize(mat3_multiply_vec(cl.yaw_rot, cl.cyl->dir));
	}
	rt->scene.objects[id].string = rt->scene.objects[id].display_string(rt,
			rt->scene.objects[id]);
	if (rvec.roll == 0)
        atomic_store(&rt->state.re_render_scene, true);
}

void	translate_cylinder(t_rt *rt, int id, t_vec vec)
{
	t_cylinder	*cylinder;

	cylinder = rt->scene.objects[id].obj;
	cylinder->origin = vec_plus(cylinder->origin, vec);
	rt->scene.objects[id].string = rt->scene.objects[id].display_string(rt,
			rt->scene.objects[id]);
    atomic_store(&rt->state.re_render_scene, true);
}

char	*string_cylinder(t_rt *rt, const struct s_object object)
{
	char		*r_value;
	char		*dest;
	t_cylinder	*cylinder;

	dest = (char [24 + 1]){};
	cylinder = object.obj;
	r_value = rt_ft_strjoin(rt, "cy  ", vec_toa(rt, cylinder->origin));
	r_value = rt_ft_strjoin(rt, r_value, "  ");
	r_value = rt_ft_strjoin(rt, r_value, vec_toa(rt, cylinder->dir));
	r_value = rt_ft_strjoin(rt, r_value, " ");
	fpconv_dtoa(cylinder->radius, dest);
	r_value = rt_ft_strjoin(rt, r_value, dest);
	dest = (char [24 + 1]){};
	r_value = rt_ft_strjoin(rt, r_value, " ");
	fpconv_dtoa(cylinder->height, dest);
	r_value = rt_ft_strjoin(rt, r_value, dest);
	r_value = rt_ft_strjoin(rt, r_value, "  ");
	r_value = rt_ft_strjoin(rt, r_value, vec_toa(rt, vec_mult(255,
					object.albedo)));
	return (r_value);
}

void	fill_cy(t_rt *rt, t_cylinder *cylinder, int id)
{
	rt->scene.objects[id].obj = cylinder;
	rt->scene.objects[id].id = id;
	rt->scene.objects[id].type = cy;
	rt->scene.objects[id].display_string = &string_cylinder;
	rt->scene.objects[id].rotate = &rotate_cylinder_local;
	rt->scene.objects[id].translate = &translate_cylinder;
	rt->scene.objects[id].string = string_cylinder(rt, \
			rt->scene.objects[id]);
}

void	parse_cylinder(t_rt *rt, char *line, int *id)
{
	char		**tab;
	t_cylinder	*cylinder;

	cylinder = wrap_malloc(rt, sizeof(*cylinder));
	tab = rt_ft_split(rt, line, ' ');
	if (tab[1] && tab[2] && tab[3] && tab[4] && tab[5])
	{
		cylinder->origin = parse_vec(rt, tab[1]);
		cylinder->dir = normalize(parse_vec(rt, tab[2]));
		cylinder->radius = ft_atoi_double(tab[3]);
		if (cylinder->radius < 0)
			exit_error(rt, "Error: Invalid radius for cylinders");
		cylinder->height = ft_atoi_double(tab[4]);
		if (cylinder->height < 0)
			exit_error(rt, "Error: Invalid height for cylinders");
		rt->scene.objects[*id].is_intersection = &is_intersection_cylinder;
		rt->scene.objects[*id].albedo = vec_mult(1.0 / 255, parse_color(rt,
					tab[5]));
		fill_cy(rt, cylinder, *id);
		(*id)++;
		return ;
	}
	exit_error(rt, "Error: Invalid number of arguments for cylinders\n");
}
