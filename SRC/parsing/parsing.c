/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gschwand <gschwand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 10:48:23 by gschwand          #+#    #+#             */
/*   Updated: 2025/01/23 15:29:48 by gschwand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"

int parsing_minirt(char *namefile)
{
	t_rt rt;
	t_file **file;

	file = open_file(namefile);
	if (!file)
		return (1);
	// print_lst_file(file);
	rt = alloc_rt(file);
	free_lst_file(file);
	return (0);
}

