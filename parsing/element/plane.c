/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gschwand <gschwand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 16:33:07 by gschwand          #+#    #+#             */
/*   Updated: 2025/03/29 10:28:20 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../miniRT.h"

void translate_plane(t_rt * rt, int id, t_vec vec)
{
	t_plane * plane;

	plane = rt->scene.objects[id].obj;
	plane->origin = vec_plus(plane->origin, vec);
	rt->scene.objects[id].string = rt->scene.objects[id].display_string(rt, rt->scene.objects[id]);
	rt->state.re_render_scene = true;
}

char * string_plane(t_rt * rt, const struct s_object object)
{
	char * r_value;
    t_plane *plane;

    plane = object.obj;
    r_value = rt_ft_strjoin(rt, "pl id:", rt_ft_itoa(rt, object.id));
    r_value = rt_ft_strjoin(rt, r_value, "  ");
    r_value = rt_ft_strjoin(rt, r_value, vec_toa(rt, plane->origin));
    r_value = rt_ft_strjoin(rt, r_value, " ");
    r_value = rt_ft_strjoin(rt, r_value, vec_toa(rt, plane->normal));
    r_value = rt_ft_strjoin(rt, r_value, " ");
    r_value = rt_ft_strjoin(rt, r_value, vec_toa(rt, vec_mult(255, object.albedo)));

    return r_value;
}

void	parse_plane(t_rt *rt, char *line, int * id) {
	char	**tab;
    t_plane * plane;

    plane = wrap_malloc(rt , sizeof(*plane));
    tab = rt_ft_split(rt, line, ' ');
	if (tab[1] && tab[2] && tab[3])
	{
		plane->origin = parse_vec(rt, tab[1]);
		plane->normal = normalize(parse_vec(rt, tab[2]));
        rt->scene.objects[*id].is_intersection = &is_intersection_plane;
        rt->scene.objects[*id].albedo =vec_mult(1.0/255,parse_color(rt, tab[3])); 
        rt->scene.objects[*id].obj = plane;
        rt->scene.objects[*id].id = *id;
        rt->scene.objects[*id].type = pl;
        rt->scene.objects[*id].display_string = &string_plane;
        rt->scene.objects[*id].translate = &translate_plane;
        rt->scene.objects[*id].string = string_plane(rt, rt->scene.objects[*id]);
        (*id)++;
        return;
	}
    exit_error(rt, "Error: Invalid number of arguments for planes");
}
