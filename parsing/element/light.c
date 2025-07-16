/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light->c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gschwand <gschwand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 16:30:27 by gschwand          #+#    #+#             */
/*   Updated: 2025/03/29 11:33:00 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../miniRT.h"

void translate_light(t_rt * rt, int id, t_vec vec)
{
	t_light * light;

	light = rt->scene.objects[id].obj;
	light->origin = vec_plus(light->origin, vec);
	rt->scene.light = *light;
	rt->scene.objects[id].string = rt->scene.objects[id].display_string(rt, rt->scene.objects[id]);
	rt->state.re_render_scene = true;
}

char * string_light(t_rt * rt, const struct s_object object)
{
	char * r_value;
	char * dest;
    t_light *light;

    dest = (char[24 + 1]){};
    light = object.obj;
    r_value = rt_ft_strjoin(rt, "L id:", rt_ft_itoa(rt, object.id));
    r_value = rt_ft_strjoin(rt, r_value, "  ");
    r_value = rt_ft_strjoin(rt, r_value, vec_toa(rt, light->origin));
    r_value = rt_ft_strjoin(rt, r_value, " ");
    fpconv_dtoa(light->intensity, dest);
    r_value = rt_ft_strjoin(rt, r_value, dest);
    r_value = rt_ft_strjoin(rt, r_value, " ");
    r_value = rt_ft_strjoin(rt, r_value, vec_toa(rt, vec_mult(255, object.albedo)));

    return r_value;
}
void	parse_light(t_rt *rt, char *line, int * id)
{
	t_light *light;
	char	**tab;

	light = wrap_malloc(rt, sizeof(*light));
	tab = rt_ft_split(rt, line, ' ');
	if (tab[1] && tab[2] && tab[3])
	{
		light->origin = parse_vec(rt, tab[1]);
		light->intensity = ft_atoi_double(tab[2]);
		if (light->intensity < 0 || light->intensity > 1)
            		exit_error(rt, "Error: Invalid ratio for light");
		rt->scene.objects[*id].is_intersection = NULL;
		light->color = vec_mult(1.0/255,parse_color(rt, tab[3]));
		rt->scene.objects[*id].albedo = light->color;
		rt->scene.objects[*id].obj = light;
		rt->scene.objects[*id].id = *id;
		rt->scene.objects[*id].type = L;
		rt->scene.objects[*id].display_string = &string_light;
		rt->scene.objects[*id].translate = &translate_light;
		rt->scene.objects[*id].string = string_light(rt, rt->scene.objects[*id]);
		(*id)++;
		rt->scene.light = *light;
        return;
	}
    exit_error(rt, "Error: Invalid number of arguments for light");
}
