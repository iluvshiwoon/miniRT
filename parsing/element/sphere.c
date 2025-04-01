/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gschwand <gschwand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 16:32:15 by gschwand          #+#    #+#             */
/*   Updated: 2025/03/29 10:21:13 by kgriset          ###   ########.fr       */
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

// int	parse_sphere(t_rt *rt, char *line)
void	parse_sphere(t_rt *rt, char *line)
{
	char	**tab;
	int		i;

	i = find_sphere_id(rt->scene.spheres);
	rt->scene.spheres[i].id = i + 1;
	// tab = ft_split(line, ' ');
    tab = rt_ft_split(rt, line, ' ');
	// if (!tab)
	// 	return (1);
	if (tab[1] && tab[2] && tab[3])
	{
		rt->scene.spheres[i].origin = parse_vec(rt, tab[1]);
		// if (!rt->scene.spheres[i].origin)
		// 	return (free_tab_char(tab), 1);
		rt->scene.spheres[i].radius = ft_atoi_double(tab[2]);
		if (rt->scene.spheres[i].radius < 0)
			exit_error(rt,"Error: Invalid radius for sphere");
			// return (free_tab_char(tab), ft_putstr_fd("Error: Invalid radius for sphere\n", 2), 1);
		rt->scene.spheres[i].albedo = parse_color(rt, tab[3]);
		// if (!rt->scene.spheres[i].albedo)
		// 	return (free_tab_char(tab), 1);
		// free_tab_char(tab);
		// return (0);
        return;
	}
    exit_error(rt, "Error: Invalid number of arguments for sphere");
	// return (ft_putstr_fd("Error: Invalid number of arguments for sphere\n", 2),
	// 	1);
}
