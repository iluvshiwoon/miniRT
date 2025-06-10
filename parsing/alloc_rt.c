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
}

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
        else
            check_if_element(rt, node->line);
		node = node->next;
	}
	if (amb_light != 1 || light != 1 || camera != 1) // on peut render sans amb_light nn ? ou c juste histoire de respecter une certaine syntax dans le fichier d'input
        exit_error(rt, "Error: Invalid number of elements in the file");
}

void	alloc_scene(t_rt *rt, t_file **file)
{
	t_file	*node;

	node = *file;
	while (node)
	{
		if (!ft_strncmp(node->line, "sp ", 3))
            rt->scene.total_objects++;
		else if (!ft_strncmp(node->line, "pl ", 3))
            rt->scene.total_objects++;
		else if (!ft_strncmp(node->line, "cy ", 3))
            rt->scene.total_objects++;
		node = node->next;
	}
    rt->scene.objects = wrap_malloc(rt, sizeof(*rt->scene.objects) * rt->scene.total_objects);
}

void	creat_scene(t_rt *rt, t_file **file)
{
	rt->scene = (t_scene){0};
    check_files(rt,file);
    alloc_scene(rt,file);
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

void alloc_element(t_rt *rt, t_file **file)
{
    t_parser parsers[7];
    t_file *node;
    int i;
    int j;
    
    creat_struct_parser_minirt(parsers);
    node = *file;
    i = 0;
    while (node)
    {
        j = 0;
        while (parsers[j].id)
        {
            if (!ft_strncmp(node->line, parsers[j].id, ft_strlen(parsers[j].id)))
            {
                parsers[j].parse(rt, node->line, &i);
                break;
            }
            j++;
        }
        node = node->next;
    }
}

void alloc_rt(t_rt * rt, t_file **file)
{
    creat_scene(rt,file);
    alloc_element(rt, file);
}
