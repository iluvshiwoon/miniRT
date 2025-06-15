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

static int	find_plane_id(t_plane *plane)
{
	int	i;

	i = 0;
	if (plane[0].id == 0)
		return (i);
	while (1)
	{
		if (plane[i].id == 0)
			return (i);
		i++;
	}
	return (i);
}

void	parse_plane(t_rt *rt, char *line, int * id)
{
	char	**tab;
    t_plane * plane;

    plane = wrap_malloc(rt , sizeof(*plane));
    tab = rt_ft_split(rt, line, ' ');
	if (tab[1] && tab[2] && tab[3])
	{
		plane->origin = parse_vec(rt, tab[1]);
		plane->normal = normalize(parse_vec(rt, tab[2]));
        rt->scene.objects[*id].is_intersection = &is_intersection_plane;
        rt->scene.objects[*id].debug_print = &print_plane;
        rt->scene.objects[*id].albedo =vec_mult(1.0/255,parse_color(rt, tab[3])); 
        rt->scene.objects[*id].obj = plane;
        (*id)++;
        return;
	}
    exit_error(rt, "Error: Invalid number of arguments for planes");
}
