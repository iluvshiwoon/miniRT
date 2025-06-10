/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gschwand <gschwand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 10:15:25 by gschwand          #+#    #+#             */
/*   Updated: 2025/04/04 14:25:52 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../miniRT.h"

void	parse_camera(t_rt *rt, char *line, int * id)
{
	char	**tab;

    (void)id;
	tab = rt_ft_split(rt, line, ' ');
	if (tab[1] && tab[2] && tab[3])
	{
		rt->scene.camera.origin = parse_vec(rt, tab[1]);
		rt->scene.camera.direction = parse_vec(rt, tab[2]);
		rt->scene.camera.fov = ft_atoi_double(tab[3]);
		if (rt->scene.camera.fov < 0 || rt->scene.camera.fov > 180)
            exit_error(rt, "Error: Invalid fov for camera");
        rt->scene.camera.fov *= M_PI / 180;
        return;
	}
    exit_error(rt, "Error: Invalid number of arguments for camera");
}
