/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_rt.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gschwand <gschwand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 10:48:41 by gschwand          #+#    #+#             */
/*   Updated: 2025/01/23 15:39:44 by gschwand         ###   ########.fr       */
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

// attention fuite de memoire
int alloc_scene(t_scene * scene, t_file **file)
{
    t_file *node;

    node = *file;
    while (node)
    {
        if (!ft_strncmp(node->line, "sp ", 3))
            scene->sphere_nb++;
        else if (!ft_strncmp(node->line, "pl ", 3))
            scene->plane_nb++;
        else if (!ft_strncmp(node->line, "cy ", 3))
            scene->cylinder_nb++;
        node = node->next;
    }
    scene->spheres = malloc(sizeof(t_sphere) * scene->sphere_nb);
    if (!scene->spheres)
        return (ft_putstr_fd("Error: Memory allocation failed\n", 2), 1);
    scene->plane = malloc(sizeof(t_plane) * scene->plane_nb);
    if (!scene->plane)
        return (ft_putstr_fd("Error: Memory allocation failed\n", 2), 1);
    scene->cylinder = malloc(sizeof(t_cylinder) * scene->cylinder_nb);
    if (!scene->cylinder)
        return (ft_putstr_fd("Error: Memory allocation failed\n", 2), 1);
    return (0);
}

t_scene creat_scene(t_file **file)
{
    t_scene scene;
    scene = (t_scene) {0};
    if (check_files(file))
        return (scene);
    if (alloc_scene(&scene, file))
        return (scene);
    
    return (scene);
}

t_rt alloc_rt(t_file **file)
{
    t_rt rt;

    rt.scene = creat_scene(file);
    return (rt);
}
