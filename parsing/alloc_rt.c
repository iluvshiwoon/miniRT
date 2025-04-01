/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_rt.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gschwand <gschwand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 10:48:41 by gschwand          #+#    #+#             */
/*   Updated: 2025/03/31 14:56:11 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/parsing.h"

void	check_if_element(t_rt * rt, char *str)
{
    if (ft_strncmp(str, "sp ", 3) && ft_strncmp(str, "pl ", 3) && ft_strncmp(str, "cy ", 3))
        exit_error(rt, "Error: Invalid line in the file");

	// if (!ft_strncmp(str, "sp ", 3))
	// 	return (0);
	// else if (!ft_strncmp(str, "pl ", 3))
	// 	return (0);
	// else if (!ft_strncmp(str, "cy ", 3))
	// 	return (0);
	// else
	// {
	// 	ft_putstr_fd("Error: Invalid line in the file\n", 2);
	// 	return (1);
	// }
}

// int	check_files(t_rt * rt, t_file **file)
void	check_files(t_rt * rt, t_file **file)
{
	t_file	*node;
	int		amb_light;
	int		light;
	int		camera;

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
		// else if (check_if_element(node->line))
		// 	return (1);
        else
            check_if_element(rt, node->line);
		node = node->next;
	}
	if (amb_light != 1 || light != 1 || camera != 1) // on peut render sans amb_light nn ? ou c juste histoire de respecter une certaine syntax dans le fichier d'input
        exit_error(rt, "Error: Invalid number of elements in the file");
		// return (ft_putstr_fd("Error: Invalid number of elements in the file\n",
		// 		2), 1);
	// return (0);
}

void	init_null_element(t_rt *rt)
{
	int	i;
	i = 0;
	while (i < rt->scene.spheres_nb)
	{
		rt->scene.spheres[i].id = 0;
		i++;
	}
	i = 0;
	while (i < rt->scene.planes_nb)
	{
		rt->scene.planes[i].id = 0;
		i++;
	}
	i = 0;
	while (i < rt->scene.cylinders_nb)
	{
		rt->scene.cylinders[i].id = 0;
		i++;
	}
}

// int	alloc_nbr_element(t_rt *rt)
void	alloc_nbr_element(t_rt *rt)
{
	rt->scene.spheres = wrap_malloc(rt, sizeof(t_sphere) * rt->scene.spheres_nb);
	// if (!rt->scene.spheres)
	// 	return (ft_putstr_fd("Error: Memory allocation failed\n", 2), 1);
	rt->scene.planes = wrap_malloc(rt, sizeof(t_plane) * rt->scene.planes_nb);
	// if (!rt->scene.planes)
	// {
	// 	return (ft_putstr_fd("Error: Memory allocation failed\n", 2), 1);
	// }
	rt->scene.cylinders = wrap_malloc(rt, sizeof(t_cylinder) * rt->scene.cylinders_nb);
	// if (!rt->scene.cylinders)
	// {
	// 	return (ft_putstr_fd("Error: Memory allocation failed\n", 2), 1);
	// }
	init_null_element(rt);
	// return (0);
}

// int	alloc_single_element(t_rt *rt)
// {
// 	// rt->scene.ambient_light = wrap_malloc(rt, sizeof(t_ambient_light));
// 	// if (!rt->scene.ambient_light)
// 	// 	return (ft_putstr_fd("Error: Memory allocation failed\n", 2), 1);
// 	// rt->scene.light = wrap_malloc(rt, sizeof(t_light));
// 	// if (!rt->scene.light)
// 	// {
// 	// 	return (ft_putstr_fd("Error: Memory allocation failed\n", 2), 1);
// 	// }
// 	rt->scene.camera = wrap_malloc(rt, sizeof(t_camera));
// 	if (!rt->scene.camera)
// 	{
// 		return (ft_putstr_fd("Error: Memory allocation failed\n", 2), 1);
// 	}
// 	return (0);
// }

// int	alloc_scene(t_rt *rt, t_file **file)
void	alloc_scene(t_rt *rt, t_file **file)
{
	t_file	*node;

	node = *file;
	while (node)
	{
		if (!ft_strncmp(node->line, "sp ", 3))
			rt->scene.spheres_nb++;
		else if (!ft_strncmp(node->line, "pl ", 3))
			rt->scene.planes_nb++;
		else if (!ft_strncmp(node->line, "cy ", 3))
			rt->scene.cylinders_nb++;
		node = node->next;
	}
    alloc_nbr_element(rt);
	// if (alloc_nbr_element(rt))
		// return (1);
	// if (alloc_single_element(rt))
	// 	return (1);
	// return (0);
}

// int	creat_scene(t_rt *rt, t_file **file)
void	creat_scene(t_rt *rt, t_file **file)
{
	rt->scene = (t_scene){0};
    check_files(rt,file);
    alloc_scene(rt,file);
	// if (check_files(rt, file))
	// 	return (1);
	// if (alloc_scene(rt, file))
	// 	return (1);
	// return (0);
}

void creat_struct_parser_minirt(t_parser *parser)
{
    parser[0] = (t_parser){"sp ", parse_sphere};
    parser[1] = (t_parser){"pl ", parse_plane};
    parser[2] = (t_parser){"cy ", parse_cylinder};
    parser[3] = (t_parser){"A ", parse_amb_light};
    parser[4] = (t_parser){"C ", parse_camera};
    parser[5] = (t_parser){"L ", parse_light};
    parser[6] = (t_parser){NULL, NULL};
}

// int alloc_element(t_rt *rt, t_file **file)
void alloc_element(t_rt *rt, t_file **file)
{
    t_parser parsers[7];
    t_file *node;
    int i;
    
    creat_struct_parser_minirt(parsers);
    node = *file;
    while (node)
    {
        i = 0;
        while (parsers[i].id)
        {
            if (!ft_strncmp(node->line, parsers[i].id, ft_strlen(parsers[i].id)))
            {
                // if (parsers[i].parse(rt, node->line))
                //     return (1);
                parsers[i].parse(rt, node->line);
                break;
            }
            i++;
        }
        node = node->next;
    }
    // return (0);
}

void alloc_rt(t_rt * rt, t_file **file)
{
	// t_rt	*rt;
	//
	// rt = malloc(sizeof(t_rt));
	// if (!rt)
	// {
	// 	ft_putstr_fd("Error: Memory allocation failed\n", 2);
	// 	return (NULL);
	// }
	// rt->current_heap = init_alloc(&rt->graphic_heap);
    creat_scene(rt,file);
    alloc_element(rt, file);
	// if (!creat_scene(rt, file))
 //        alloc_element(rt, file);
	// 	return (NULL);
	// if (alloc_element(rt, file))
	// 	return (NULL);
}
