/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gschwand <gschwand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 16:35:47 by gschwand          #+#    #+#             */
/*   Updated: 2025/01/27 16:36:24 by gschwand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/parsing.h"

int parse_cylinder(t_rt *rt, char *line)
{
    char **tab;
    int i;

    i = 0;
    tab = ft_split(line, ' ');
    if (!tab)
        return (1);
    if (tab[1] && tab[2] && tab[3] && tab[4] && tab[5])
    {
        rt->scene.cylinder->origin = parse_vec(tab[1]);
        rt->scene.cylinder->direction = parse_vec(tab[2]);
        rt->scene.cylinder->radius = ft_atoi_double(tab[3]);
        rt->scene.cylinder->height = ft_atoi_double(tab[4]);
        rt->scene.cylinder->albedo = parse_color(tab[5]);
        if (!rt->scene.cylinder->albedo)
            return (1);
        while (tab[i])
            free(tab[i++]);
        free(tab);
        return (0);
    }
    return (ft_putstr_fd("Error: Invalid number of arguments for cylinder\n", 2), 1);
}
