/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   element_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gschwand <gschwand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 09:38:28 by gschwand          #+#    #+#             */
/*   Updated: 2025/04/04 15:24:43 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../miniRT.h"

void	check_size_tab(t_rt *rt, char **tab)
{
	int	i;

	i = 0;
	if (!tab)
        exit_error(rt, "Error: Invalid number of arguments");
	while (tab[i])
		i++;
	if (i != 3)
        exit_error(rt, "Error: Invalid number of arguments");
}

void	check_color(t_rt * rt,char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (!ft_isdigit(line[i]))
            exit_error(rt, "Error: Invalid color");
		i++;
	}
}

void	check_vec(t_rt * rt, char *line)
{
	int	i;
	int	point;

	i = 0;
	point = 0;
	while (line[i])
	{
		if (!ft_isdigit(line[i]) && i == 0 && line[i] != '-')
            exit_error(rt,"Error: Invalid vector");
		else if (line[i] == '.')
			point++;
		i++;
	}
	if (point > 1)
        exit_error(rt,"Error: Invalid vector");
}

t_vec	parse_color(t_rt *rt, char *line)
{
	t_vec	color;
	char	**tab;

    tab = rt_ft_split(rt,line, ',');
	check_size_tab(rt,tab);
    check_color(rt, tab[0]);
    color.x = ft_atoi(tab[0]);
    check_color(rt, tab[1]);
	color.y = ft_atoi(tab[1]);
    check_color(rt , tab[2]);
	color.z = ft_atoi(tab[2]);
    if (color.x < 0 || color.y < 0 || color.z < 0 || color.x > 255
		|| color.y > 255 || color.z > 255)
        exit_error(rt, "Error: Invalid color");
	return (color);
}

t_vec	parse_vec(t_rt *rt, char *line)
{
    t_vec vec;
	char	**tab;

    vec = (t_vec){};
    tab = rt_ft_split(rt, line, ',');
	check_size_tab(rt, tab);
    check_vec(rt, tab[0]);
	vec.x = ft_atoi_double(tab[0]);
    check_vec(rt, tab[1]);
	vec.y = ft_atoi_double(tab[1]);
    check_vec(rt, tab[2]); 
    vec.z = ft_atoi_double(tab[2]);
	return (vec);
}

void print_sphere(t_rt *rt , int id)
{
    t_sphere sphere;
    t_vec albedo;

    albedo = rt->scene.objects[id].albedo;
    sphere = *((t_sphere *){rt->scene.objects[id].obj});
    printf("ID: %d ; sp %f,%f,%f %f %f,%f,%f\n",\
           id,
           sphere.origin.x, sphere.origin.y, sphere.origin.z,\
           sphere.radius,\
           albedo.x, albedo.y, albedo.z);
}

void print_plane(t_rt *rt , int id)
{
    t_plane plane;
    t_vec albedo;

    albedo = rt->scene.objects[id].albedo;
    plane = *((t_plane *){rt->scene.objects[id].obj});
    printf("ID: %d ; sp %f,%f,%f %f,%f,%f %f,%f,%f\n",\
           id,
           plane.origin.x, plane.origin.y, plane.origin.z,\
           plane.normal.x, plane.normal.y, plane.normal.z,\
           albedo.x, albedo.y, albedo.z);
}

void print_cylinder(t_rt *rt , int id)
{
    t_cylinder cylinder;
    t_vec albedo;

    albedo = rt->scene.objects[id].albedo;
    cylinder = *((t_cylinder *){rt->scene.objects[id].obj});
    printf("ID: %d ; sp %f,%f,%f %f,%f,%f %f %f %f,%f,%f\n",\
           id,
           cylinder.origin.x, cylinder.origin.y, cylinder.origin.z,\
           cylinder.direction.x, cylinder.direction.y, cylinder.direction.z,\
           cylinder.radius,\
           cylinder.height,\
           albedo.x, albedo.y, albedo.z);
}

void print_light(t_rt * rt)
{
    t_light light;

    light = rt->scene.light;
    printf("L %f,%f,%f %f %f,%f,%f\n",\
           light.origin.x, light.origin.y, light.origin.z,\
           light.intensity,\
           light.color.x, light.color.y, light.color.z);
}

void print_ambiant_light(t_rt * rt)
{
    t_ambient_light ambient_light;

    ambient_light = rt->scene.ambient_light;
    printf("A %f,%f,%f %f\n",\
           ambient_light.color.x, ambient_light.color.y, ambient_light.color.z,\
           ambient_light.intensity);
}

void print_camera(t_rt *rt)
{
    t_camera camera;

    camera = rt->scene.camera;
    printf("C %f,%f,%f %f,%f,%f %f\n",\
           camera.origin.x, camera.origin.y, camera.origin.z,\
           camera.direction.x, camera.direction.y, camera.direction.z,\
           camera.fov);
}

void	print_scene(t_rt *rt)
{
	int	i;

	i = -1;
    print_ambiant_light(rt);
    print_camera(rt);
    print_light(rt);
    while (++i < rt->scene.total_objects)
        rt->scene.objects[i].debug_print(rt, i);
}
