/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone_intersection_utils2.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 19:25:57 by kgriset           #+#    #+#             */
/*   Updated: 2025/08/20 14:55:00 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"

bool	cone_cap_intersection_s(t_co_cap_args args)
{
	t_cone_cap	co;

	co.cap_plane.origin = vec_plus(args.cone.origin, vec_mult(args.cone.height,
				args.cone.dir));
	co.cap_plane.normal = vec_mult(-1, args.cone.dir);
	co.min_t = -1.0;
	cone_cap_calc_s((t_co_cap_calc_args){&co, args.ray, args.cone,
		args.normal});
	if (co.min_t > 0)
	{
		*(args.t) = co.min_t;
		return (true);
	}
	return (false);
}

void	init_cone_intersection(t_cone_inter *co, const t_object obj)
{
	co->cone = *((t_cone *)obj.obj);
	co->has_sol = false;
	co->final_t = -1;
	co->hit_cap = false;
}
