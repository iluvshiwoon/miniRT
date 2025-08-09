/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gschwand <gschwand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 16:32:15 by gschwand          #+#    #+#             */
/*   Updated: 2025/07/25 13:11:39 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../miniRT.h"

void	translate_sphere(t_rt *rt, int id, t_vec vec)
{
	t_sphere	*sphere;

	sphere = rt->scene.objects[id].obj;
	sphere->origin = vec_plus(sphere->origin, vec);
	rt->scene.objects[id].string = rt->scene.objects[id].display_string(rt,
			rt->scene.objects[id]);
    atomic_store(&rt->state.re_render_scene, true);
}

char	*string_sphere(t_rt *rt, const struct s_object object)
{
	char		*r_value;
	char		*dest;
	t_sphere	*sphere;

	dest = (char [24 + 1]){};
	sphere = object.obj;
	r_value = rt_ft_strjoin(rt, "sp  ", vec_toa(rt, sphere->origin));
	r_value = rt_ft_strjoin(rt, r_value, " ");
	fpconv_dtoa(sphere->radius, dest);
	r_value = rt_ft_strjoin(rt, r_value, dest);
	r_value = rt_ft_strjoin(rt, r_value, " ");
	r_value = rt_ft_strjoin(rt, r_value, vec_toa(rt, vec_mult(255,
					object.albedo)));
	if (object.normal_map_path)
	{
		r_value = rt_ft_strjoin(rt, r_value, " ");
		r_value = rt_ft_strjoin(rt, r_value, object.normal_map_path);
	}
	if (object.texture_map_path)
	{
		r_value = rt_ft_strjoin(rt, r_value, " ");
		r_value = rt_ft_strjoin(rt, r_value, object.texture_map_path);
	}
	if (object.texture_scale.x != 0)
	{
		r_value = rt_ft_strjoin(rt, r_value, " ");
		r_value = rt_ft_strjoin(rt, r_value, vec_toa(rt, object.texture_scale));
	}
	return (r_value);
}

void	fill_sphere(t_rt *rt, t_sphere *sphere, int id)
{
	rt->scene.objects[id].obj = sphere;
	rt->scene.objects[id].id = id;
	rt->scene.objects[id].type = sp;
	rt->scene.objects[id].display_string = &string_sphere;
	rt->scene.objects[id].translate = &translate_sphere;
	rt->scene.objects[id].string = string_sphere(rt, \
			rt->scene.objects[id]);
	// Initialize material properties with default values
	rt->scene.objects[id].specular = (t_vec){0.5, 0.5, 0.5}; // Default specular color
	rt->scene.objects[id].shininess = 32.0; // Default shininess
	rt->scene.objects[id].checkerboard = false; // Default no checkerboard
}

void	parse_sphere(t_rt *rt, char *line, int *id)
{
	char		**tab;
	t_sphere	*sphere;

	sphere = wrap_malloc(rt, sizeof(*sphere));
	tab = rt_ft_split(rt, line, ' ');
	if (tab[1] && tab[2] && tab[3])
	{
		rt->scene.objects[*id].texture_map_path = NULL;
		rt->scene.objects[*id].normal_map_path = NULL;
		sphere->origin = parse_vec(rt, tab[1]);
		sphere->radius = ft_atoi_double(tab[2]);
		if (sphere->radius < 0)
			exit_error(rt, "Error: Invalid radius for sphere");
		rt->scene.objects[*id].is_intersection = &is_intersection_sphere;
		rt->scene.objects[*id].albedo = vec_mult(1.0 / 255, parse_color(rt,
					tab[3]));
		rt->scene.objects[*id].texture_scale = (t_vec){1.0, 1.0, 1.0};
		if (tab[4])
		{
			if (ft_strncmp(tab[4], ".", 2) != 0)
			{
				rt->scene.objects[*id].normal_map_path = rt_ft_strdup(rt, tab[4]);
				printf("Found normal map for sphere: %s\n", rt->scene.objects[*id].normal_map_path);
			}
			if (tab[5])
			{
				if (ft_strncmp(tab[5], ".", 2) != 0)
					rt->scene.objects[*id].texture_map_path = rt_ft_strdup(rt, tab[5]);
				if (tab[6])
					rt->scene.objects[*id].texture_scale = parse_vec(rt, tab[6]);
			}
		}
		fill_sphere(rt, sphere, *id);
				(*id)++;
		return ;
	}
	exit_error(rt, "Error: Invalid number of arguments for sphere");
}
