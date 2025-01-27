/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gschwand <gschwand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 16:32:15 by gschwand          #+#    #+#             */
/*   Updated: 2025/01/27 16:32:50 by gschwand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/parsing.h"

int parse_sphere(t_rt *rt, char *line)
{
    char **tab;
    int i;

    i = 0;
    tab = ft_split(line, ' ');
    if (!tab)
        return (1);
    if (tab[1] && tab[2] && tab[3])
    {
        rt->scene.sphere->origin = parse_vec(tab[1]);
        rt->scene.sphere->radius = ft_atoi_double(tab[2]);
        rt->scene.sphere->albedo = parse_color(tab[3]);
        if (!rt->scene.sphere->albedo)
            return (1);
        while (tab[i])
            free(tab[i++]);
        free(tab);
        return (0);
    }
    return (ft_putstr_fd("Error: Invalid number of arguments for sphere\n", 2), 1);
}
