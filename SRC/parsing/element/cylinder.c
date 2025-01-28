/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gschwand <gschwand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 16:35:47 by gschwand          #+#    #+#             */
/*   Updated: 2025/01/28 14:38:16 by gschwand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/parsing.h"

static int find_cylinder_id(t_cylinder *cylinder)
{
    int i;

    i = 0;
    if (cylinder[0].id == 0)
        return (i);
    while (i)
    {
        if (cylinder[i].id == 0)
            return (i);
        i++;
    }
    return (i);
}

int parse_cylinder(t_rt *rt, char *line)
{
    char **tab;
    int i;

    i = find_cylinder_id(rt->scene.cylinder);
    rt->scene.cylinder[i].id = i + 1;
    tab = ft_split(line, ' ');
    if (!tab)
        return (1);
    if (tab[1] && tab[2] && tab[3] && tab[4] && tab[5])
    {
        rt->scene.cylinder[i].origin = parse_vec(tab[1]);
        if (!rt->scene.cylinder[i].origin)
            return (1);
        rt->scene.cylinder[i].direction = parse_vec(tab[2]);
        if (!rt->scene.cylinder[i].direction)
            return (1);
        rt->scene.cylinder[i].radius = ft_atoi_double(tab[3]);
        if (rt->scene.cylinder[i].radius < 0)
            return (ft_putstr_fd("Error: Invalid radius for cylinder\n", 2), 1);
        rt->scene.cylinder[i].height = ft_atoi_double(tab[4]);
        if (rt->scene.cylinder[i].height < 0)
            return (ft_putstr_fd("Error: Invalid height for cylinder\n", 2), 1);
        rt->scene.cylinder[i].albedo = parse_color(tab[5]);
        if (!rt->scene.cylinder[i].albedo)
            return (1);
        while (tab[i])
            free(tab[i++]);
        free(tab);
        return (0);
    }
    return (ft_putstr_fd("Error: Invalid number of arguments for cylinder\n", 2), 1);
}
