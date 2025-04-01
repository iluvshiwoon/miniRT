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

static int	find_cylinder_id(t_cylinder *cylinder)
{
	int	i;

	i = 0;
	if (cylinder[0].id == 0)
		return (i);
	while (1)
	{
		if (cylinder[i].id == 0)
			return (i);
		i++;
	}
	return (i);
}

// int	parse_cylinder(t_rt *rt, char *line)
void	parse_cylinder(t_rt *rt, char *line)
{
	char	**tab;
	int		i;

	i = find_cylinder_id(rt->scene.cylinders);
	rt->scene.cylinders[i].id = i + 1;
	// tab = ft_split(line, ' ');
	// if (!tab)
	// 	return (1);
    tab = rt_ft_split(rt, line, ' ');
	if (tab[1] && tab[2] && tab[3] && tab[4] && tab[5])
	{
		rt->scene.cylinders[i].origin = parse_vec(rt, tab[1]);
		// if (!rt->scene.cylinders[i].origin)
		// 	return (free_tab_char(tab), 1);
		rt->scene.cylinders[i].direction = parse_vec(rt, tab[2]);
		// if (!rt->scene.cylinders[i].direction)
		// 	return (free_tab_char(tab), 1);
		rt->scene.cylinders[i].radius = ft_atoi_double(tab[3]);
		if (rt->scene.cylinders[i].radius < 0)
            exit_error(rt, "Error: Invalid radius for cylinders");
			// return (free_tab_char(tab), ft_putstr_fd("Error: Invalid radius for cylinders\n", 2), 1);
		rt->scene.cylinders[i].height = ft_atoi_double(tab[4]);
		if (rt->scene.cylinders[i].height < 0)
			// return (free_tab_char(tab), ft_putstr_fd("Error: Invalid height for cylinders\n", 2), 1);
            exit_error(rt, "Error: Invalid height for cylinders");
		rt->scene.cylinders[i].albedo = parse_color(rt, tab[5]);
		// if (!rt->scene.cylinders[i].albedo)
		// 	return (free_tab_char(tab), 1);
		// free_tab_char(tab);
		// return (0);
        return;
	}
	// return (free_tab_char(tab), ft_putstr_fd("Error: Invalid number of arguments for cylinders\n", 2), 1);
    exit_error(rt, "Error: Invalid number of arguments for cylinders\n");
}
