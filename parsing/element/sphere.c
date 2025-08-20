/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 14:59:01 by kgriset           #+#    #+#             */
/*   Updated: 2025/08/20 14:59:02 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../miniRT.h"

void	parse_sphere_optional(t_rt *rt, char **tab, int *id)
{
	if (tab[4])
	{
		if (ft_strncmp(tab[4], ".", 2) != 0)
			rt->scene.objects[*id].specular = vec_mult(1.0 / 255,
					parse_color(rt, tab[4]));
		if (tab[5])
		{
			if (ft_strncmp(tab[5], ".", 2) != 0)
				rt->scene.objects[*id].shininess = ft_atoi_double(tab[5]);
			if (tab[6])
			{
				if (ft_strncmp(tab[6], ".", 2) != 0)
					rt->scene.objects[*id].normal_map_path = rt_ft_strdup(rt,
							tab[6]);
				parse_sphere_optional1(rt, tab, id);
			}
		}
	}
}

void	parse_sphere(t_rt *rt, char *line, int *id)
{
	char		**tab;
	t_sphere	*sphere;

	sphere = wrap_malloc(rt, sizeof(*sphere));
	tab = rt_ft_split(rt, line, ' ');
	if (tab[1] && tab[2] && tab[3])
	{
		sphere->origin = parse_vec(rt, tab[1]);
		sphere->radius = ft_atoi_double(tab[2]);
		if (sphere->radius < 0)
			exit_error(rt, "Error: Invalid radius for sphere");
		rt->scene.objects[*id].is_intersection = &is_intersection_sphere;
		rt->scene.objects[*id].albedo = vec_mult(1.0 / 255, parse_color(rt,
					tab[3]));
		fill_sphere(rt, sphere, *id);
		parse_sphere_optional(rt, tab, id);
		(*id)++;
		return ;
	}
	exit_error(rt, "Error: Invalid number of arguments for sphere");
}
