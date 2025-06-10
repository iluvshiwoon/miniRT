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
	t_file	**file;

	file = open_file(rt, namefile);
	alloc_rt(rt, file);
	print_scene(rt);
}
