/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_error.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 16:18:50 by kgriset           #+#    #+#             */
/*   Updated: 2025/04/01 08:32:39 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"

void exit_error(t_rt * rt, char * msg)
{
    ft_printf_fd(2,"%s\n",msg);
    if (rt->fd_file)
        close(rt->fd_file);
    free_heap(rt);
    exit(12);
}
