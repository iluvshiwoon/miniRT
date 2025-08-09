/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_rt.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gschwand <gschwand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 10:48:41 by gschwand          #+#    #+#             */
/*   Updated: 2025/07/25 13:25:04 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/parsing.h"

void	check_if_element(t_rt *rt, char *str)
{
	if (ft_strncmp(str, "sp ", 3) && ft_strncmp(str, "pl ", 3)
		&& ft_strncmp(str, "cy ", 3) && ft_strncmp(str, "co ", 3))
		exit_error(rt, "Error: Invalid line in the file");
}

void	check_files(t_rt *rt, t_file **file)
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
	if (amb_light != 1 || light < 1 || camera != 1)
		exit_error(rt, "Error: Invalid number of elements in the file");
}

void	alloc_element(t_rt *rt, t_file **file)
{
	t_parser	parsers[8];
	t_file		*node;
	int			i;
	int			j;

	creat_struct_parser_minirt(parsers);
	node = *file;
	i = 0;
	while (node)
	{
		j = 0;
		while (parsers[j].id)
		{
			if (!ft_strncmp(node->line, parsers[j].id,
					ft_strlen(parsers[j].id)))
			{
				parsers[j].parse(rt, node->line, &i);
				break ;
			}
			j++;
		}
		node = node->next;
	}
}

void	alloc_rt(t_rt *rt, t_file **file)
{
	creat_scene(rt, file);
	alloc_element(rt, file);
	load_normal_maps(rt);
}
