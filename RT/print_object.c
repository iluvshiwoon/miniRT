/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_object.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 17:33:52 by kgriset           #+#    #+#             */
/*   Updated: 2025/08/20 14:55:17 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/rt.h"

char	*vec_toa(t_rt *rt, t_vec vec)
{
	char	*dest;
	char	*r_value;

	dest = (char [24 + 1]){};
	fpconv_dtoa(vec.x, dest);
	r_value = rt_ft_strjoin(rt, dest, ",");
	dest = (char [24 + 1]){};
	fpconv_dtoa(vec.y, dest);
	r_value = rt_ft_strjoin(rt, r_value, dest);
	r_value = rt_ft_strjoin(rt, r_value, ",");
	dest = (char [24 + 1]){};
	fpconv_dtoa(vec.z, dest);
	r_value = rt_ft_strjoin(rt, r_value, dest);
	return (r_value);
}
