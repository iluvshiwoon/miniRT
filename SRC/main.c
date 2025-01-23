/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gschwand <gschwand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 10:48:28 by gschwand          #+#    #+#             */
/*   Updated: 2025/01/23 10:48:29 by gschwand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/parsing.h"

int miniRT(char *file)
{
	// t_rt rt;

    parsing_minirt(file);
    // if (!rt)
    //     return (1);
    return (0);
}

int main(int ac, char **av)
{
    if(check_args(ac, av))
        return (1);
    if (miniRT(av[1]))
		return (1);
	return (0);
}