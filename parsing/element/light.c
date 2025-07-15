/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gschwand <gschwand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 16:30:27 by gschwand          #+#    #+#             */
/*   Updated: 2025/03/29 11:33:00 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../miniRT.h"
	// char	**tab;
	//    t_sphere * sphere;
	//
	//    sphere = wrap_malloc(rt, sizeof(*sphere));
	//    tab = rt_ft_split(rt, line, ' ');
	// if (tab[1] && tab[2] && tab[3])
	// {
	// 	sphere->origin = parse_vec(rt, tab[1]);
	// 	sphere->radius = ft_atoi_double(tab[2]);
	// 	if (sphere->radius < 0)
	// 		exit_error(rt,"Error: Invalid radius for sphere");
	//        rt->scene.objects[*id].is_intersection = &is_intersection_sphere;
	//        rt->scene.objects[*id].debug_print = &print_sphere;
	//        rt->scene.objects[*id].albedo =vec_mult(1.0/255,parse_color(rt, tab[3])); 
	//        rt->scene.objects[*id].obj = sphere;
	//        rt->scene.objects[*id].id = *id;
	// rt->scene.objects[*id].display_string = &string_sphere;
	//        rt->scene.objects[*id].string = string_sphere(rt, rt->scene.objects[*id]);
	//        (*id)++;
	//        return;
	// }
	//    exit_error(rt, "Error: Invalid number of arguments for sphere");

void	parse_light(t_rt *rt, char *line, int * id)
{
	t_light light;
	char	**tab;

    (void)id;
	tab = rt_ft_split(rt, line, ' ');
	if (tab[1] && tab[2] && tab[3])
	{
		light.origin = parse_vec(rt, tab[1]);
		light.intensity = ft_atoi_double(tab[2]);
		if (light.intensity < 0 || light.intensity > 1)
            		exit_error(rt, "Error: Invalid ratio for light");
		light.color =vec_mult(1.0/255,parse_color(rt, tab[3])); 
		rt->scene.objects[*id].obj = :bn

        return;
	}
    exit_error(rt, "Error: Invalid number of arguments for light");
}
