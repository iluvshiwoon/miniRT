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

void	parse_light(t_rt *rt, char *line, int * id)
{
	char	**tab;

    (void)id;
	tab = rt_ft_split(rt, line, ' ');
	if (tab[1] && tab[2] && tab[3])
	{
		rt->scene.light.origin = parse_vec(rt, tab[1]);
		rt->scene.light.intensity = ft_atoi_double(tab[2]);
		if (rt->scene.light.intensity < 0 || rt->scene.light.intensity > 1)
            exit_error(rt, "Error: Invalid ratio for light");
		rt->scene.light.color =vec_mult(1.0/255,parse_color(rt, tab[3])); 
        return;
	}
    exit_error(rt, "Error: Invalid number of arguments for light");
}
