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

// int	parse_plane(t_rt *rt, char *line)
void	parse_plane(t_rt *rt, char *line)
{
	char	**tab;
	int		i;

	i = find_plane_id(rt->scene.planes);
	rt->scene.planes[i].id = i + 1;
	// tab = ft_split(line, ' ');
	// if (!tab)
	// 	return (1);
    tab = rt_ft_split(rt, line, ' ');
	if (tab[1] && tab[2] && tab[3])
	{
		rt->scene.planes[i].origin = parse_vec(rt, tab[1]);
		// if (!rt->scene.planes[i].origin)
		// 	return (free_tab_char(tab), 1);
		rt->scene.planes[i].normal = parse_vec(rt, tab[2]);
		// if (!rt->scene.planes[i].normal)
		// 	return (free_tab_char(tab), 1);
		rt->scene.planes[i].albedo = parse_color(rt, tab[3]);
		// if (!rt->scene.planes[i].albedo)
		// 	return (free_tab_char(tab), 1);
		// free_tab_char(tab);
		// return (0);
        return;
	}
    exit_error(rt, "Error: Invalid number of arguments for planes");
	// return (free_tab_char(tab), ft_putstr_fd("Error: Invalid number of arguments for planes\n", 2), 1);
}
