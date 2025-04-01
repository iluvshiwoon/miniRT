/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 23:27:57 by kgriset           #+#    #+#             */
/*   Updated: 2025/03/25 13:42:31 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"

double norm2(const t_vec v)
{
    return (v.x * v.x + v.y * v.y + v.z * v.z);
}

t_vec normalize(const t_vec v)
{
    double norm;
    t_vec n_v;

    norm = sqrt(norm2(v)); 
    n_v.x = v.x / norm;
    n_v.y = v.y /norm;
    n_v.z = v.z /norm;
    return n_v;
}

t_vec vec_m_vec(const t_vec a, const t_vec b)
{
    t_vec m_v;

    m_v.x = a.x * b.x;
    m_v.y = a.y * b.y;
    m_v.z = a.z * b.z;
    return m_v;
}
