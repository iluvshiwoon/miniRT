/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 16:30:00 by kgriset           #+#    #+#             */
/*   Updated: 2025/08/18 10:57:26 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../miniRT.h"

char	*append_cone_dims(t_rt *rt, char *r_value, t_cone *cone)
{
	char	dest[24 + 1];

	ft_memset(dest, 0, sizeof(dest));
	fpconv_dtoa(cone->radius, dest);
	r_value = rt_ft_strjoin(rt, r_value, dest);
	ft_memset(dest, 0, sizeof(dest));
	r_value = rt_ft_strjoin(rt, r_value, " ");
	fpconv_dtoa(cone->height, dest);
	return (rt_ft_strjoin(rt, r_value, dest));
}

char	*append_optional_maps_co(t_rt *rt, char *r_value,
		const struct s_object object)
{
	if (object.normal_map_path || object.texture_map_path
		|| object.texture_scale.x != 1.0 || object.texture_scale.y != 1.0)
	{
		r_value = rt_ft_strjoin(rt, r_value, " ");
		if (object.normal_map_path)
			r_value = rt_ft_strjoin(rt, r_value, object.normal_map_path);
		else
			r_value = rt_ft_strjoin(rt, r_value, ".");
		r_value = rt_ft_strjoin(rt, r_value, " ");
		if (object.texture_map_path)
			r_value = rt_ft_strjoin(rt, r_value, object.texture_map_path);
		else
			r_value = rt_ft_strjoin(rt, r_value, ".");
		r_value = rt_ft_strjoin(rt, r_value, " ");
		r_value = rt_ft_strjoin(rt, r_value, vec_toa(rt, object.texture_scale));
	}
	return (r_value);
}

void	parse_cone_optional1(t_rt *rt, char **tab, int *id)
{
	if (tab[8])
	{
		if (ft_strncmp(tab[8], ".", 2) != 0)
			rt->scene.objects[*id].normal_map_path = \
			rt_ft_strdup(rt, tab[8]);
		if (tab[9])
		{
			if (ft_strncmp(tab[9], ".", 2) != 0)
				rt->scene.objects[*id].texture_map_path = \
				rt_ft_strdup(rt, tab[9]);
			if (tab[10])
				rt->scene.objects[*id].texture_scale = parse_vec(rt,
						tab[10]);
		}
	}
}

void	parse_cone_optional(t_rt *rt, char **tab, int *id)
{
	if (tab[6])
	{
		if (ft_strncmp(tab[6], ".", 2) != 0)
			rt->scene.objects[*id].specular = vec_mult(1.0 / 255,
					parse_color(rt, tab[6]));
		if (tab[7])
		{
			if (ft_strncmp(tab[7], ".", 2) != 0)
				rt->scene.objects[*id].shininess = ft_atoi_double(tab[7]);
			parse_cone_optional1(rt, tab, id);
		}
	}
}

void	rotate_cone_local(t_rt *rt, int id, t_rvec rvec)
{
	t_rot_cone	r;

	r.cone = rt->scene.objects[id].obj;
	r.axis = normalize(r.cone->dir);
	r.temp = (t_vec){0, 1, 0};
	if (fabs(r.axis.y) > 0.99)
		r.temp = (t_vec){1, 0, 0};
	r.right = normalize(cross(r.temp, r.axis));
	r.forward = cross(r.axis, r.right);
	if (fabs(rvec.pitch) > 1e-6)
	{
		r.pitch_rot = create_rotation_axis(r.right, rvec.pitch);
		r.cone->dir = normalize(mat3_multiply_vec(r.pitch_rot, r.cone->dir));
	}
	if (fabs(rvec.yaw) > 1e-6)
	{
		r.yaw_rot = create_rotation_axis(r.forward, rvec.yaw);
		r.cone->dir = normalize(mat3_multiply_vec(r.yaw_rot, r.cone->dir));
	}
	rt->scene.objects[id].string = rt->scene.objects[id].display_string(rt,
			rt->scene.objects[id]);
	atomic_store(&rt->state.re_render_scene, true);
}
