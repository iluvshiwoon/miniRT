/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 23:27:57 by kgriset           #+#    #+#             */
/*   Updated: 2025/01/19 23:38:20 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void init_vec(t_vec * vec)
{
    *vec = (t_vec){};
    vec->pf_vec_div=vec_div;
    vec->pf_vec_mult=vec_mult;
    vec->pf_vec_plus=vec_plus;
    vec->pf_vec_minus=vec_minus;
    vec->pf_vec_scal=vec_scal;
    vec->pf_norm2=norm2;
    vec->pf_normalize=normalize;
}

double norm2(const t_vec v)
{
    return (v.x * v.x + v.y * v.y + v.z * v.z);
}

void normalize(t_vec * v)
{
    double norm;

    norm = sqrt(norm2(*v)); 
    v->x /= norm;
    v->y /= norm;
    v->z /= norm;
}
