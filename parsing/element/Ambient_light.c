/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ambient_light.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gschwand <gschwand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 11:04:52 by gschwand          #+#    #+#             */
/*   Updated: 2025/08/18 10:36:41 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../miniRT.h"

char	*string_ambient(t_rt *rt, const struct s_object object)
{
	char			*r_value;
	char			*dest;
	t_ambient_light	*ambient_light;

	dest = (char [24 + 1]){};
	ambient_light = object.obj;
	fpconv_dtoa(ambient_light->intensity, dest);
	r_value = rt_ft_strjoin(rt, "A  ", dest);
	r_value = rt_ft_strjoin(rt, r_value, " ");
	r_value = rt_ft_strjoin(rt, r_value, vec_toa(rt, vec_mult(255,
					object.albedo)));
	return (r_value);
}

void	set_amb(t_rt *rt, t_ambient_light *ambient_light, int *id)
{
	rt->scene.objects[*id].albedo = ambient_light->color;
	rt->scene.objects[*id].obj = ambient_light;
	rt->scene.objects[*id].id = *id;
	rt->scene.objects[*id].type = A;
	rt->scene.objects[*id].display_string = &string_ambient;
	rt->scene.objects[*id].string = string_ambient(rt,
			rt->scene.objects[*id]);
	rt->scene.objects[*id].specular = (t_vec){0.0, 0.0, 0.0};
	rt->scene.objects[*id].shininess = 0.0;
	rt->scene.objects[*id].checkerboard = false;
}

void	parse_amb_light(t_rt *rt, char *line, int *id)
{
	t_ambient_light	*ambient_light;
	char			**tab;

	ambient_light = wrap_malloc(rt, sizeof(*ambient_light));
	tab = rt_ft_split(rt, line, ' ');
	if (tab[1] && tab[2])
	{
		ambient_light->intensity = ft_atoi_double(tab[1]);
		if (ambient_light->intensity < 0 || ambient_light->intensity > 1)
			exit_error(rt, "Error: Invalid ratio for ambient light");
		rt->scene.objects[*id].is_intersection = NULL;
		ambient_light->color = vec_mult(1.0 / 255, parse_color(rt, tab[2]));
		set_amb(rt, ambient_light, id);
		(*id)++;
		rt->scene.ambient_light = *ambient_light;
		return ;
	}
	exit_error(rt, "Error: Invalid number of arguments for ambient light");
}
