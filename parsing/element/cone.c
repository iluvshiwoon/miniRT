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
	char	dest2[24 + 1];

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
	rt->scene.objects[id].checkerboard = false;
	rt->scene.objects[id].specular = (t_vec){0.5, 0.5, 0.5};
	rt->scene.objects[id].shininess = 32.0;
	rt->scene.objects[id].texture_scale = (t_vec){1.0, 1.0, 1.0};
	rt->scene.objects[id].checkerboard = false;
	rt->scene.objects[id].texture_map_path = NULL;
	rt->scene.objects[id].normal_map_path = NULL;
}

void	parse_cone(t_rt *rt, char *line, int *id)
{
	char	**tab;
	t_cone	*cone;

	cone = wrap_malloc(rt, sizeof(*cone));
	tab = rt_ft_split(rt, line, ' ');
	if (tab[1] && tab[2] && tab[3] && tab[4] && tab[5])
	{
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
		fill_co(rt, cone, *id);
		parse_cone_optional(rt, tab, id);
		(*id)++;
		return ;
	}
	exit_error(rt, "Error: Invalid number of arguments for cones\n");
}
