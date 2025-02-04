/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gschwand <gschwand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 10:48:23 by gschwand          #+#    #+#             */
/*   Updated: 2025/01/31 11:20:10 by gschwand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"

t_rt	*parsing_minirt(char *namefile)
{
	t_rt	*rt;
	t_file	**file;

	file = open_file(namefile);
	if (!file)
		return (NULL);
	// print_lst_file(file);
	rt = alloc_rt(file);
	if (!rt)
		return (NULL);
	print_scene(rt);
	free_heap(rt);
	free(rt);
	free_lst_file(file);
	return (rt);
}
