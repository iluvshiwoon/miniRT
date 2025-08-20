/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 14:58:17 by kgriset           #+#    #+#             */
/*   Updated: 2025/08/20 14:58:18 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"

void	parsing_minirt(t_rt *rt, char *namefile)
{
	t_file	**file;

	file = open_file(rt, namefile);
	alloc_rt(rt, file);
}
