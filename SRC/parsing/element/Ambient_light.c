/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ambient_light.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gschwand <gschwand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 11:04:52 by gschwand          #+#    #+#             */
/*   Updated: 2025/01/28 14:11:59 by gschwand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/parsing.h"

int parse_amb_light(t_rt *rt, char *line)
{
    char **tab;
    int i;

    i = 0;
    tab = ft_split(line, ' ');
    if (tab[1] && tab[2])
    {
        rt->scene.ambient_light->intensity = ft_atoi_double(tab[1]);
        if (rt->scene.ambient_light->intensity < 0 || rt->scene.ambient_light->intensity > 1)
            return (ft_putstr_fd("Error: Invalid ratio for ambient light\n", 2), 1);
        rt->scene.ambient_light->color = parse_color(tab[2]);
        if (!rt->scene.ambient_light->color)
            return (1);
        while (tab[i])
            free(tab[i++]);
        free(tab);
        return (0);
    }
    return (ft_putstr_fd("Error: Invalid number of arguments for ambient light\n", 2), 1);
}
