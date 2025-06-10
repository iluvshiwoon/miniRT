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

static int	find_sphere_id(t_sphere *sphere)
{
	int	i;

	i = 0;
	if (sphere[0].id == 0)
		return (i);
	while (1)
	{
		if (sphere[i].id == 0)
			return (i);
		i++;
	}
	return (i);
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
        (*id)++;
        return;
	}
    exit_error(rt, "Error: Invalid number of arguments for sphere");
}
