/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gschwand <gschwand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 16:33:07 by gschwand          #+#    #+#             */
/*   Updated: 2025/01/27 16:35:17 by gschwand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/parsing.h"

int parse_plane(t_rt *rt, char *line)
{
    char **tab;
    int i;

    i = 0;
    tab = ft_split(line, ' ');
    if (!tab)
        return (1);
    if (tab[1] && tab[2] && tab[3])
    {
        rt->scene.plane->origin = parse_vec(tab[1]);
        rt->scene.plane->normal = parse_vec(tab[2]);
        rt->scene.plane->albedo = parse_color(tab[3]);
        if (!rt->scene.plane->albedo)
            return (1);
        while (tab[i])
            free(tab[i++]);
        free(tab);
        return (0);
    }
    return (ft_putstr_fd("Error: Invalid number of arguments for plane\n", 2), 1);
}
