/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gschwand <gschwand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 16:33:07 by gschwand          #+#    #+#             */
/*   Updated: 2025/07/25 13:08:23 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../miniRT.h"

void    parse_optional_plane1(t_rt *rt, char **tab, int *id)
{
    if (tab[7])
    {
        if (ft_strncmp(tab[7], ".", 2) != 0)
            rt->scene.objects[*id].texture_map_path = rt_ft_strdup(rt, tab[7]);
        if (tab[8])
            rt->scene.objects[*id].texture_scale = parse_vec(rt, tab[8]);
    }
}

void    parse_optional_plane(t_rt *rt, char **tab, int *id)
{
	if (tab[4])
		{
			if (ft_strncmp(tab[4], ".", 2) != 0)
				rt->scene.objects[*id].specular = vec_mult(1.0 / 255, parse_color(rt, tab[4]));
			if (tab[5])
			{
				if (ft_strncmp(tab[5], ".", 2) != 0)
					rt->scene.objects[*id].shininess = ft_atoi_double(tab[5]);
				if (tab[6])
				{
					if (ft_strncmp(tab[6], ".", 2) != 0)
						rt->scene.objects[*id].normal_map_path = rt_ft_strdup(rt, tab[6]);
                    parse_optional_plane1(rt, tab, id);
				}
			}
		}
}

void	parse_plane(t_rt *rt, char *line, int *id)
{
	char	**tab;
	t_plane	*plane;

	plane = wrap_malloc(rt, sizeof(*plane));
	tab = rt_ft_split(rt, line, ' ');
	if (tab[1] && tab[2] && tab[3])
	{
        fill_plane(rt, tab, plane, id);
        parse_optional_plane(rt, tab, id);
		(*id)++;
		return ;
	}
	exit_error(rt, "Error: Invalid number of arguments for planes");
}
