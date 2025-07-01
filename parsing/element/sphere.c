/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gschwand <gschwand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 16:32:15 by gschwand          #+#    #+#             */
/*   Updated: 2025/04/12 15:22:15 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../miniRT.h"

char * string_sphere(t_rt * rt, const struct s_object object)
{
    t_sphere *sphere;

    sphere = object.obj;
    char * r_string;
    char * coordinates;
    r_string = rt_ft_strjoin(rt, "sp: ", rt_ft_itoa(rt, object.id));
    // coordinates = rt_ft_strjoin(rt, " ", rt_ft_itoa(rt, object.))
}

void	parse_sphere(t_rt *rt, char *line, int * id)
{
	char	**tab;
    t_sphere * sphere;

    sphere = wrap_malloc(rt, sizeof(*sphere));
    tab = rt_ft_split(rt, line, ' ');
	if (tab[1] && tab[2] && tab[3])
	{
		sphere->origin = parse_vec(rt, tab[1]);
		sphere->radius = ft_atoi_double(tab[2]);
		if (sphere->radius < 0)
			exit_error(rt,"Error: Invalid radius for sphere");
        rt->scene.objects[*id].is_intersection = &is_intersection_sphere;
        rt->scene.objects[*id].debug_print = &print_sphere;
        rt->scene.objects[*id].albedo =vec_mult(1.0/255,parse_color(rt, tab[3])); 
        rt->scene.objects[*id].obj = sphere;
        rt->scene.objects[*id].id = *id;
        (*id)++;
        return;
	}
    exit_error(rt, "Error: Invalid number of arguments for sphere");
}
