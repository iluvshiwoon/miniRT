/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   element_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gschwand <gschwand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 09:38:28 by gschwand          #+#    #+#             */
/*   Updated: 2025/01/28 14:41:04 by gschwand         ###   ########.fr       */
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
        {
            printf("%sd\n", line);
            return (ft_putstr_fd("Error: Invalid color\n", 2), 1);
        }
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
        if (!ft_isdigit(line[i]) && i == 0 && line[i] != '-')
        {
            printf("%s\n", line);
            return (ft_putstr_fd("Error: Invalid vector\n", 2), 1);
        }
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
    color = malloc(sizeof(t_vec));
    if (!color)
        return (ft_putstr_fd("Error: Memory allocation failed\n", 2), NULL);
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
    vec = malloc(sizeof(t_vec));
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

void free_tab_char(char **tab)
{
    int i;

    i = 0;
    while (tab[i])
        free(tab[i++]);
    free(tab);
}

void print_scene(t_rt *rt)
{
    int i;

    i = 0;
    printf("Sphere\n");
    while (i < rt->scene.sphere_nb)
    {
        printf("Sphere %d\n", rt->scene.sphere[i].id);
        printf("Origin: %f %f %f\n", rt->scene.sphere[i].origin->x, rt->scene.sphere[i].origin->y, rt->scene.sphere[i].origin->z);
        printf("Radius: %f\n", rt->scene.sphere[i].radius);
        printf("Color: %f %f %f\n", rt->scene.sphere[i].albedo->x, rt->scene.sphere[i].albedo->y, rt->scene.sphere[i].albedo->z);
        i++;
    }
    i = 0;
    printf("Plane\n");
    while (i < rt->scene.plane_nb)
    {
        printf("Plane %d\n", i);
        printf("Origin: %f %f %f\n", rt->scene.plane[i].origin->x, rt->scene.plane[i].origin->y, rt->scene.plane[i].origin->z);
        printf("Normal: %f %f %f\n", rt->scene.plane[i].normal->x, rt->scene.plane[i].normal->y, rt->scene.plane[i].normal->z);
        printf("Color: %f %f %f\n", rt->scene.plane[i].albedo->x, rt->scene.plane[i].albedo->y, rt->scene.plane[i].albedo->z);
        i++;
    }
    i = 0;
    printf("Cylinder\n");
    while (i < rt->scene.cylinder_nb)
    {
        printf("Cylinder %d\n", i);
        printf("Origin: %f %f %f\n", rt->scene.cylinder[i].origin->x, rt->scene.cylinder[i].origin->y, rt->scene.cylinder[i].origin->z);
        printf("Direction: %f %f %f\n", rt->scene.cylinder[i].direction->x, rt->scene.cylinder[i].direction->y, rt->scene.cylinder[i].direction->z);
        printf("Radius: %f\n", rt->scene.cylinder[i].radius);
        printf("Height: %f\n", rt->scene.cylinder[i].height);
        printf("Color: %f %f %f\n", rt->scene.cylinder[i].albedo->x, rt->scene.cylinder[i].albedo->y, rt->scene.cylinder[i].albedo->z);
        i++;
    }
    printf("Light\n");
    printf("Origin: %f %f %f\n", rt->scene.light->origin->x, rt->scene.light->origin->y, rt->scene.light->origin->z);
    printf("Color: %f %f %f\n", rt->scene.light->color->x, rt->scene.light->color->y, rt->scene.light->color->z);

    printf("Ambient light\n");
    printf("Color: %f %f %f\n", rt->scene.ambient_light->color->x, rt->scene.ambient_light->color->y, rt->scene.ambient_light->color->z);
    printf("Intensity: %f\n", rt->scene.ambient_light->intensity);
    
    printf("Camera\n");
    printf("Origin: %f %f %f\n", rt->scene.camera->origin->x, rt->scene.camera->origin->y, rt->scene.camera->origin->z);
    printf("Direction: %f %f %f\n", rt->scene.camera->direction->x, rt->scene.camera->direction->y, rt->scene.camera->direction->z);
    printf("Fov: %f\n", rt->scene.camera->fov);
    
}

// int find_elem_id(void *elem)
// {
//     int i;

//     i = 0;
//     if (elem[0]->id == 0)
//         return (i);
//     while (elem[i])
//     {
//         if (elem[i]->id == 0)
//             return (i);
//         i++;
//     }
//     return (i);
// }