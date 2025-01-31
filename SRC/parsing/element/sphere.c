/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gschwand <gschwand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 16:32:15 by gschwand          #+#    #+#             */
/*   Updated: 2025/01/31 11:18:58 by gschwand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/parsing.h"

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

int	parse_sphere(t_rt *rt, char *line)
{
	char	**tab;
	int		i;

	i = find_sphere_id(rt->scene.sphere);
	rt->scene.sphere[i].id = i + 1;
	tab = ft_split(line, ' ');
	if (!tab)
		return (1);
	if (tab[1] && tab[2] && tab[3])
	{
		rt->scene.sphere[i].origin = parse_vec(rt, tab[1]);
		if (!rt->scene.sphere[i].origin)
			return (free_tab_char(tab), 1);
		rt->scene.sphere[i].radius = ft_atoi_double(tab[2]);
		if (rt->scene.sphere[i].radius < 0)
			return (free_tab_char(tab), ft_putstr_fd("Error: Invalid radius for sphere\n", 2), 1);
		rt->scene.sphere[i].albedo = parse_color(rt, tab[3]);
		if (!rt->scene.sphere[i].albedo)
			return (free_tab_char(tab), 1);
		free_tab_char(tab);
		return (0);
	}
	return (ft_putstr_fd("Error: Invalid number of arguments for sphere\n", 2),
		1);
}
