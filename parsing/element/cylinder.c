/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 14:58:27 by kgriset           #+#    #+#             */
/*   Updated: 2025/08/20 14:58:29 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../miniRT.h"

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
	t_cylinder	*cylinder;
	char		dest2[24 + 1];

	cylinder = object.obj;
	r_value = rt_ft_strjoin(rt, "cy  ", vec_toa(rt, cylinder->origin));
	r_value = rt_ft_strjoin(rt, r_value, "  ");
	r_value = rt_ft_strjoin(rt, r_value, vec_toa(rt, cylinder->dir));
	r_value = rt_ft_strjoin(rt, r_value, " ");
	r_value = append_cylinder_dims(rt, r_value, cylinder);
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
	r_value = append_optional_maps_cy(rt, r_value, object);
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
	rt->scene.objects[id].string = string_cylinder(rt, rt->scene.objects[id]);
	rt->scene.objects[id].checkerboard = false;
	rt->scene.objects[id].specular = (t_vec){0.5, 0.5, 0.5};
	rt->scene.objects[id].shininess = 32.0;
	rt->scene.objects[id].texture_scale = (t_vec){1.0, 1.0, 1.0};
	rt->scene.objects[id].checkerboard = false;
	rt->scene.objects[id].texture_map_path = NULL;
	rt->scene.objects[id].normal_map_path = NULL;
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
		parse_cylinder_optional(rt, tab, id);
		(*id)++;
		return ;
	}
	exit_error(rt, "Error: Invalid number of arguments for cylinders\n");
}
