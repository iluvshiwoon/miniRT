/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_rt.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gschwand <gschwand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 10:48:41 by gschwand          #+#    #+#             */
/*   Updated: 2025/01/28 13:53:36 by gschwand         ###   ########.fr       */
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

void init_null_element(t_rt *rt)
{
    int i = 0;

    while (i < rt->scene.sphere_nb)
    {
        rt->scene.sphere[i].id = 0;
        i++;
    }
    i = 0;
    while (i < rt->scene.plane_nb)
    {
        rt->scene.plane[i].id = 0;
        i++;
    }
    i = 0;
    while (i < rt->scene.cylinder_nb)
    {
        rt->scene.cylinder[i].id = 0;
        i++;
    }
}

int alloc_nbr_element(t_rt *rt)
{
    rt->scene.sphere = malloc(sizeof(t_sphere) * rt->scene.sphere_nb);
    if (!rt->scene.sphere)
        return (ft_putstr_fd("Error: Memory allocation failed\n", 2), 1);
    rt->scene.plane = malloc(sizeof(t_plane) * rt->scene.plane_nb);
    if (!rt->scene.plane)
    {
        free(rt->scene.sphere);
        return (ft_putstr_fd("Error: Memory allocation failed\n", 2), 1);
    }
    rt->scene.cylinder = malloc(sizeof(t_cylinder) * rt->scene.cylinder_nb);
    if (!rt->scene.cylinder)
    {
        free(rt->scene.sphere);
        free(rt->scene.plane);     
        return (ft_putstr_fd("Error: Memory allocation failed\n", 2), 1);
    }
    init_null_element(rt);
    return (0);
}

int alloc_single_element(t_rt *rt)
{
    rt->scene.ambient_light = malloc(sizeof(t_ambient_light));
    if (!rt->scene.ambient_light)
        return (ft_putstr_fd("Error: Memory allocation failed\n", 2), 1);
    rt->scene.light = malloc(sizeof(t_light));
    if (!rt->scene.light)
    {
        free(rt->scene.ambient_light);
        return (ft_putstr_fd("Error: Memory allocation failed\n", 2), 1);
    }
    rt->scene.camera = malloc(sizeof(t_camera));
    if (!rt->scene.camera)
    {
        free(rt->scene.ambient_light);
        free(rt->scene.light);
        return (ft_putstr_fd("Error: Memory allocation failed\n", 2), 1);
    }
    return (0);
}

void free_alloc_nbr_element(t_rt *rt)
{
    free(rt->scene.sphere);
    free(rt->scene.plane);
    free(rt->scene.cylinder);
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
    if (alloc_single_element(rt))
        return (free_alloc_nbr_element(rt), 1);
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
    printf("Creation de la scene semble ok\n");
    if (alloc_element(rt, file))
        return (NULL);
    printf("Creation des elements semble ok\n");
    return (rt);
}
