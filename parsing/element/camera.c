/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gschwand <gschwand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 10:15:25 by gschwand          #+#    #+#             */
/*   Updated: 2025/03/29 11:24:30 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../miniRT.h"

// int	parse_camera(t_rt *rt, char *line)
void	parse_camera(t_rt *rt, char *line)
{
	char	**tab;
	int		i;

	i = 0;
	// tab = ft_split(line, ' ');
	tab = rt_ft_split(rt, line, ' ');
	// if (!tab)
	// 	return (1);
	if (tab[1] && tab[2] && tab[3])
	{
		// rt->scene.camera->origin = parse_vec(rt, tab[1]);
		rt->scene.camera.origin = parse_vec(rt, tab[1]);
		// if (!rt->scene.camera->origin)
		// 	return (free_tab_char(tab), ft_putstr_fd("Error: Invalid origin for camera\n", 2), 1);
		// rt->scene.camera->direction = parse_vec(rt, tab[2]);
		rt->scene.camera.direction = parse_vec(rt, tab[2]);
		// if (!rt->scene.camera->direction)
		// 	return (free_tab_char(tab), ft_putstr_fd("Error: Invalid direction for camera\n", 2),
		// 		1);
		// rt->scene.camera->fov = ft_atoi_double(tab[3]);
		rt->scene.camera.fov = ft_atoi_double(tab[3]);
		// if (rt->scene.camera->fov < 0 || rt->scene.camera->fov > 180)
		// 	return (free_tab_char(tab), ft_putstr_fd("Error: Invalid fov for camera\n", 2), 1);
		if (rt->scene.camera.fov < 0 || rt->scene.camera.fov > 180)
            exit_error(rt, "Error: Invalid fov for camera");
		// free_tab_char(tab);
		// return (0);
        return;
	}
	// return (free_tab_char(tab), ft_putstr_fd("Error: Invalid number of arguments for camera\n", 2),
	// 	1);
    exit_error(rt, "Error: Invalid number of arguments for camera");
}
