/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 16:30:00 by kgriset           #+#    #+#             */
/*   Updated: 2025/07/25 16:30:00 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../miniRT.h"

static char	*append_cone_dims(t_rt *rt, char *r_value, t_cone *cone)
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

static char	*append_optional_maps_co(t_rt *rt, char *r_value,
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

static void	parse_cone_optional1(t_rt *rt, char **tab, int *id)
{
    if (tab[9])
    {
        if (ft_strncmp(tab[9], ".", 2) != 0)
            rt->scene.objects[*id].texture_map_path = rt_ft_strdup(rt,
                                                                   tab[9]);
        if (tab[10])
            rt->scene.objects[*id].texture_scale = parse_vec(rt,
                                                             tab[10]);
    }
}

static void	parse_cone_optional(t_rt *rt, char **tab, int *id)
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
			if (tab[8])
			{
				if (ft_strncmp(tab[8], ".", 2) != 0)
					rt->scene.objects[*id].normal_map_path = rt_ft_strdup(rt,
							tab[8]);
                parse_cone_optional(rt, tab, id);
			}
		}
	}
}

void	rotate_cone_local(t_rt *rt, int id, t_rvec rvec)
{
	t_cone	*cone;
	t_vec	axis;
	t_vec	temp;
	t_vec	right;
	t_vec	forward;
	t_mat3	pitch_rot;
	t_mat3	yaw_rot;

	cone = rt->scene.objects[id].obj;
	axis = normalize(cone->dir);
	temp = (t_vec){0, 1, 0};
	if (fabs(axis.y) > 0.99)
		temp = (t_vec){1, 0, 0};
	right = normalize(cross(temp, axis));
	forward = cross(axis, right);
	if (fabs(rvec.pitch) > 1e-6)
	{
		pitch_rot = create_rotation_axis(right, rvec.pitch);
		cone->dir = normalize(mat3_multiply_vec(pitch_rot, cone->dir));
	}
	if (fabs(rvec.yaw) > 1e-6)
	{
		yaw_rot = create_rotation_axis(forward, rvec.yaw);
		cone->dir = normalize(mat3_multiply_vec(yaw_rot, cone->dir));
	}
	rt->scene.objects[id].string = rt->scene.objects[id].display_string(rt,
			rt->scene.objects[id]);
	atomic_store(&rt->state.re_render_scene, true);
}

void	translate_cone(t_rt *rt, int id, t_vec vec)
{
	t_cone	*cone;

	cone = rt->scene.objects[id].obj;
	cone->origin = vec_plus(cone->origin, vec);
	rt->scene.objects[id].string = rt->scene.objects[id].display_string(rt,
			rt->scene.objects[id]);
	atomic_store(&rt->state.re_render_scene, true);
}

char	*string_cone(t_rt *rt, const struct s_object object)
{
	char	*r_value;
	t_cone	*cone;
		char dest2[24 + 1];

	cone = object.obj;
	r_value = rt_ft_strjoin(rt, "co  ", vec_toa(rt, cone->origin));
	r_value = rt_ft_strjoin(rt, r_value, "  ");
	r_value = rt_ft_strjoin(rt, r_value, vec_toa(rt, cone->dir));
	r_value = rt_ft_strjoin(rt, r_value, " ");
	r_value = append_cone_dims(rt, r_value, cone);
	r_value = rt_ft_strjoin(rt, r_value, "  ");
	r_value = rt_ft_strjoin(rt, r_value, color_toa(rt, object.albedo));
	r_value = rt_ft_strjoin(rt, r_value, " ");
	r_value = rt_ft_strjoin(rt, r_value, color_toa(rt, object.specular));
	r_value = rt_ft_strjoin(rt, r_value, " ");
	{
		ft_memset(dest2, 0, sizeof(dest2));
		fpconv_dtoa(object.shininess, dest2);
		r_value = rt_ft_strjoin(rt, r_value, dest2);
	}
	r_value = append_optional_maps_co(rt, r_value, object);
	return (r_value);
}

void	fill_co(t_rt *rt, t_cone *cone, int id)
{
	rt->scene.objects[id].obj = cone;
	rt->scene.objects[id].id = id;
	rt->scene.objects[id].type = co;
	rt->scene.objects[id].display_string = &string_cone;
	rt->scene.objects[id].rotate = &rotate_cone_local;
	rt->scene.objects[id].translate = &translate_cone;
	rt->scene.objects[id].string = string_cone(rt, rt->scene.objects[id]);
	// Initialize material properties with default values
	rt->scene.objects[id].checkerboard = false; // Default no checkerboard
}

void	parse_cone(t_rt *rt, char *line, int *id)
{
	char **tab;
	t_cone *cone;

	cone = wrap_malloc(rt, sizeof(*cone));
	tab = rt_ft_split(rt, line, ' ');
	if (tab[1] && tab[2] && tab[3] && tab[4] && tab[5])
	{
		rt->scene.objects[*id].texture_map_path = NULL;
		rt->scene.objects[*id].normal_map_path = NULL;
		cone->origin = parse_vec(rt, tab[1]);
		cone->dir = normalize(parse_vec(rt, tab[2]));
		cone->radius = ft_atoi_double(tab[3]);
		if (cone->radius < 0)
			exit_error(rt, "Error: Invalid radius for cones");
		cone->height = ft_atoi_double(tab[4]);
		if (cone->height < 0)
			exit_error(rt, "Error: Invalid height for cones");
		rt->scene.objects[*id].is_intersection = &is_intersection_cone;
		rt->scene.objects[*id].albedo = vec_mult(1.0 / 255, parse_color(rt,
					tab[5]));
		rt->scene.objects[*id].specular = (t_vec){0.5, 0.5, 0.5};
		rt->scene.objects[*id].shininess = 32.0;
		rt->scene.objects[*id].texture_scale = (t_vec){1.0, 1.0, 1.0};
		rt->scene.objects[*id].checkerboard = false;
		parse_cone_optional(rt, tab, id);
		fill_co(rt, cone, *id);
		(*id)++;
		return ;
	}
	exit_error(rt, "Error: Invalid number of arguments for cones\n");
}
