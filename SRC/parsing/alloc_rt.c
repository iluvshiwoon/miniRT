/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_rt.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gschwand <gschwand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 10:48:41 by gschwand          #+#    #+#             */
/*   Updated: 2025/01/24 11:09:27 by gschwand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"

int check_if_element(char *str)
{
    if (!ft_strncmp(str, "sp ", 3))
        return (0);
    else if (!ft_strncmp(str, "pl ", 3))
        return (0);
    else if (!ft_strncmp(str, "cy ", 3))
        return (0);
    else
    {
        printf("%s\n", str);
        ft_putstr_fd("Error: Invalid line in the file\n", 2);
        return (1);
    }
}

int check_files(t_file **file)
{
    t_file *node;
    int amb_light;
    int light;
    int camera;

    amb_light = 0;
    light = 0;
    camera = 0;
    node = *file;
    while (node)
    {
        if (!ft_strncmp(node->line, "A ", 2))
            amb_light++;
        else if (!ft_strncmp(node->line, "C ", 2))
            camera++;
        else if (!ft_strncmp(node->line, "L ", 2))
            light++;
        else if (check_if_element(node->line))
            return (1);
        node = node->next;
    }
    if (amb_light != 1|| light != 1|| camera != 1)
        return (ft_putstr_fd("Error: Invalid number of elements in the file\n", 2), 1);
    return (0);
}

int alloc_nbr_element(t_rt *rt)
{
    rt->scene.spheres = malloc(sizeof(t_sphere) * rt->scene.sphere_nb);
    if (!rt->scene.spheres)
        return (ft_putstr_fd("Error: Memory allocation failed\n", 2), 1);
    rt->scene.plane = malloc(sizeof(t_plane) * rt->scene.plane_nb);
    if (!rt->scene.plane)
    {
        free(rt->scene.spheres);
        return (ft_putstr_fd("Error: Memory allocation failed\n", 2), 1);
    }
    rt->scene.cylinder = malloc(sizeof(t_cylinder) * rt->scene.cylinder_nb);
    if (!rt->scene.cylinder)
    {
        free(rt->scene.spheres);
        free(rt->scene.plane);     
        return (ft_putstr_fd("Error: Memory allocation failed\n", 2), 1);
    }
    return (0);
}

int alloc_scene(t_rt *rt, t_file **file)
{
    t_file *node;

    node = *file;
    while (node)
    {
        if (!ft_strncmp(node->line, "sp ", 3))
            rt->scene.sphere_nb++;
        else if (!ft_strncmp(node->line, "pl ", 3))
            rt->scene.plane_nb++;
        else if (!ft_strncmp(node->line, "cy ", 3))
            rt->scene.cylinder_nb++;
        node = node->next;
    }
    if (alloc_nbr_element(rt))
        return (1);
    return (0);
}

int creat_scene(t_rt *rt, t_file **file)
{
    rt->scene = (t_scene) {0};
    if (check_files(file))
        return (1);
    if (alloc_scene(rt, file))
        return (1);
    
    return (0);
}

int alloc_element(t_rt *rt, t_file **file)
{
    t_file *node;

    node = *file;
    while (node)
    {
        if (!ft_strncmp(node->line, "sp ", 3))
        {
            if (parse_sphere(rt, node->line))
                return (1);
        }
        else if (!ft_strncmp(node->line, "pl ", 3))
        {
            if (parse_plane(rt, node->line))
                return (1);
        }
        else if (!ft_strncmp(node->line, "cy ", 3))
        {
            if (parse_cylinder(rt, node->line))
                return (1);
        }
        else if (!ft_strncmp(node->line, "A ", 2))
        {
            if (parse_amb_light(rt, node->line))
                return (1);
        }
        else if (!ft_strncmp(node->line, "C ", 2))
        {
            if (parse_camera(rt, node->line))
                return (1);
        }
        else if (!ft_strncmp(node->line, "L ", 2))
        {
            if (parse_light(rt, node->line))
                return (1);
        }
        node = node->next;
    }
    return (0);
}

t_rt *alloc_rt(t_file **file)
{
    t_rt *rt;

    rt = malloc(sizeof(t_rt));
    if (!rt)
    {
        ft_putstr_fd("Error: Memory allocation failed\n", 2);
        return (NULL);
    }
    if (creat_scene(rt, file))
        return (NULL);
    if (alloc_element(rt, file))
        return (NULL);
    return (rt);
}
