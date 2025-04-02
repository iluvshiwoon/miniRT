/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gschwand <gschwand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 10:48:23 by gschwand          #+#    #+#             */
/*   Updated: 2025/04/02 15:26:37 by gschwand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"

void    parsing_minirt(t_rt *rt,char *namefile)
{
	// t_rt	*rt;
	t_file	**file;

    rt->scene = (t_scene){};
	file = open_file(rt, namefile);
	// if (!file)
		// return (NULL);
	// print_lst_file(file);
	alloc_rt(rt, file);
	// if (!rt)
		// return (NULL);
	print_scene(rt);
	// free_heap(rt);
	// free(rt);
	// free_lst_file(file);
	// return (rt);
}
