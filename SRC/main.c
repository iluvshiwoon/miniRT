/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gschwand <gschwand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 10:48:28 by gschwand          #+#    #+#             */
/*   Updated: 2025/01/31 10:33:09 by gschwand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/parsing.h"

int	mini_rt(char *file)
{
	t_rt	*rt;
	
	rt = parsing_minirt(file);
	return (0);
}

int	main(int ac, char **av)
{
	if (check_args(ac, av))
		return (1);
	if (mini_rt(av[1]))
		return (1);
	return (0);
}
