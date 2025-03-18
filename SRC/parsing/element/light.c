/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gschwand <gschwand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 16:30:27 by gschwand          #+#    #+#             */
/*   Updated: 2025/03/18 09:32:22 by gschwand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/parsing.h"

int	parse_light(t_rt *rt, char *line)
{
	char	**tab;
	int		i;

	i = 0;
	tab = ft_split(line, ' ');
	if (!tab)
		return (1);
	if (tab[1] && tab[2] && tab[3])
	{
		rt->scene.light->origin = parse_vec(rt, tab[1]);
		if (!rt->scene.light->origin)
			return (free_tab_char(tab), 1);
		rt->scene.light->intensity = ft_atoi_double(tab[2]);
		if (rt->scene.light->intensity < 0 || rt->scene.light->intensity > 1)
			return (free_tab_char(tab), ft_putstr_fd("Error: Invalid ratio for light\n", 2), 1);
		rt->scene.light->color = parse_color_albego(rt, tab[3]);
		if (!rt->scene.light->color)
			return (free_tab_char(tab), 1);
		free_tab_char(tab);
		return (0);
	}
	return (free_tab_char(tab), ft_putstr_fd("Error: Invalid number of arguments for light\n", 2),
		1);
}
