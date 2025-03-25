/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 23:14:21 by kgriset           #+#    #+#             */
/*   Updated: 2025/03/25 13:42:31 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"

t_vec vec_plus(const t_vec a, const t_vec b)
{
    t_vec v;

    v.x = a.x + b.x;
    v.y = a.y + b.y;
    v.z = a.z + b.z;
    return v;
}

t_vec vec_minus(const t_vec a, const t_vec b)
{
    t_vec v;

    v.x = a.x - b.x;
    v.y = a.y - b.y;
    v.z = a.z - b.z;
    return v;
}

t_vec vec_mult(double k, const t_vec v)
{
    t_vec vec;

    vec.x = k * v.x;
    vec.y = k * v.y;
    vec.z = k * v.z;
    return vec;
}

t_vec vec_div(const t_vec v, double k)
{
    t_vec vec;

    vec.x = v.x / k;
    vec.y = v.y / k;
    vec.z = v.z / k;
    return vec;
}

double vec_scal(const t_vec a, const t_vec b)
{
    return a.x*b.x + a.y*b.y + a.z*b.z;
}
