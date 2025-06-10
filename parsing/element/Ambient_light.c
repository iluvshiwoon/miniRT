/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ambient_light.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gschwand <gschwand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 11:04:52 by gschwand          #+#    #+#             */
/*   Updated: 2025/03/29 11:06:39 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../miniRT.h"

void	parse_amb_light(t_rt *rt, char *line, int * id)
{
	char	**tab;

    (void)id;
    tab = rt_ft_split(rt, line, ' ');
	if (tab[1] && tab[2])
	{
		rt->scene.ambient_light.intensity = ft_atoi_double(tab[1]);
        if (rt->scene.ambient_light.intensity < 0
			|| rt->scene.ambient_light.intensity > 1)
            exit_error(rt, "Error: Invalid ratio for ambient light");
		rt->scene.ambient_light.color = parse_color(rt, tab[2]);
        return;
	}
    exit_error(rt, "Error: Invalid number of arguments for ambient light");
}
