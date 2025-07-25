/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 13:23:24 by kgriset           #+#    #+#             */
/*   Updated: 2025/07/25 13:23:53 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"

void	alloc_scene(t_rt *rt, t_file **file)
{
	t_file	*node;

	node = *file;
	while (node)
	{
		rt->scene.total_objects++;
		node = node->next;
	}
	rt->scene.objects = wrap_malloc(rt, sizeof(*rt->scene.objects)
			* rt->scene.total_objects);
}

void	creat_scene(t_rt *rt, t_file **file)
{
	rt->scene = (t_scene){0};
	check_files(rt, file);
	alloc_scene(rt, file);
}

void	creat_struct_parser_minirt(t_parser *parser)
{
	parser[0] = (t_parser){"sp ", parse_sphere};
	parser[1] = (t_parser){"pl ", parse_plane};
	parser[2] = (t_parser){"cy ", parse_cylinder};
	parser[3] = (t_parser){"A ", parse_amb_light};
	parser[4] = (t_parser){"C ", parse_camera};
	parser[5] = (t_parser){"L ", parse_light};
	parser[6] = (t_parser){NULL, NULL};
}
