/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gschwand <gschwand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 16:35:47 by gschwand          #+#    #+#             */
/*   Updated: 2025/03/29 10:32:50 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../miniRT.h"

void	parse_cylinder(t_rt *rt, char *line, int * id)
{
	char	**tab;
    t_cylinder * cylinder;

    cylinder = wrap_malloc(rt, sizeof(*cylinder));
    tab = rt_ft_split(rt, line, ' ');
	if (tab[1] && tab[2] && tab[3] && tab[4] && tab[5])
	{
		cylinder->origin = parse_vec(rt, tab[1]);
		cylinder->direction = normalize(parse_vec(rt, tab[2]));
		cylinder->radius = ft_atoi_double(tab[3]);
		if (cylinder->radius < 0)
            exit_error(rt, "Error: Invalid radius for cylinders");
		cylinder->height = ft_atoi_double(tab[4]);
		if (cylinder->height < 0)
            exit_error(rt, "Error: Invalid height for cylinders");
        rt->scene.objects[*id].is_intersection = &is_intersection_cylinder;
        rt->scene.objects[*id].debug_print = &print_cylinder;
        rt->scene.objects[*id].albedo =vec_mult(1.0/255,parse_color(rt, tab[5])); 
        rt->scene.objects[*id].obj = cylinder;
        rt->scene.objects[*id].id = *id;
        (*id)++;
        return;
	}
    exit_error(rt, "Error: Invalid number of arguments for cylinders\n");
}
