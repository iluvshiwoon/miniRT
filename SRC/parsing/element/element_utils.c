/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   element_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gschwand <gschwand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 09:38:28 by gschwand          #+#    #+#             */
/*   Updated: 2025/01/27 10:26:15 by gschwand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/parsing.h"

int check_size_tab(char **tab)
{
    int i;

    i = 0;
    while (tab[i])
        i++;
    return (i);
}

int check_color(char *line)
{
    int i;

    i = 0;
    while (line[i])
    {
        if (!ft_isdigit(line[i]))
            return (ft_putstr_fd("Error: Invalid color\n", 2), 1);
        i++;
    }
    return (0);
}

int check_vec(char * line)
{
    int i;
    int point;

    i = 0;
    point = 0;
    while (line[i])
    {
        if (!ft_isdigit(line[i]))
            return (ft_putstr_fd("Error: Invalid vector\n", 2), 1);
        else if (line[i] == '.')
            point++;
        i++;
    }
    if (point > 1)
        return (ft_putstr_fd("Error: Invalid vector\n", 2), 1);
    return (0);
}

t_vec *parse_color(char *line)
{
    t_vec *color;
    char **tab;
    
    tab = ft_split(line, ',');
    if (check_size_tab(tab) != 3)
        return (ft_putstr_fd("Error: Invalid number of arguments for color\n", 2), NULL);
    if (check_color(tab[0]))
        return (NULL);
    color->x = ft_atoi(tab[0]);
    if (check_color(tab[1]))
        return (NULL);
    color->y = ft_atoi(tab[1]);
    if (check_color(tab[2]))
        return (NULL);
    color->z = ft_atoi(tab[2]);
    free(tab[0]);
    free(tab[1]);
    free(tab[2]);
    free(tab);
    return (color);
}

t_vec *parse_vec(char *line)
{
    t_vec *vec;
    char **tab;

    tab = ft_split(line, ',');
    if (check_size_tab(tab) != 3)
        return (ft_putstr_fd("Error: Invalid number of arguments for vector\n", 2), NULL);
    if (check_vec(tab[0]))
        return (NULL);
    vec->x = ft_atoi_double(tab[0]);
    if (check_vec(tab[1]))
        return (NULL);
    vec->y = ft_atoi_double(tab[1]);
    if (check_vec(tab[2]))
        return (NULL);
    vec->z = ft_atoi_double(tab[2]);
    free(tab[0]);
    free(tab[1]);
    free(tab[2]);
    free(tab);
    return (vec);
}
