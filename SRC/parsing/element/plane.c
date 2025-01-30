/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gschwand <gschwand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 16:33:07 by gschwand          #+#    #+#             */
/*   Updated: 2025/01/30 15:34:21 by gschwand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/parsing.h"

static int find_plane_id(t_plane *plane)
{
    int i;

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

int parse_plane(t_rt *rt, char *line)
{
    char **tab;
    int i;

    i = find_plane_id(rt->scene.plane);
    rt->scene.plane[i].id = i + 1;
    tab = ft_split(line, ' ');
    if (!tab)
        return (1);
    if (tab[1] && tab[2] && tab[3])
    {
        rt->scene.plane[i].origin = parse_vec(tab[1]);
        if (!rt->scene.plane[i].origin)
            return (1);
        rt->scene.plane[i].normal = parse_vec(tab[2]);
        if (!rt->scene.plane[i].normal)
            return (1);
        rt->scene.plane[i].albedo = parse_color(tab[3]);
        if (!rt->scene.plane[i].albedo)
            return (1);
        free_tab_char(tab);
        return (0);
    }
    return (ft_putstr_fd("Error: Invalid number of arguments for plane\n", 2), 1);
}
