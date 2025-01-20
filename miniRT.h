/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 20:37:42 by kgriset           #+#    #+#             */
/*   Updated: 2025/01/20 22:59:25 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H
# include <stdio.h>
# include <error.h>
# include <string.h>
# include <math.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdbool.h>
# include "42_MyLibC/mylibc.h"


typedef struct s_rt {
    int W;
    int H;
    double fov;
    t_link_list * graphic_heap;
    t_link_list * current_heap;
} t_rt;

typedef struct s_vec t_vec;

typedef struct s_vec {
    double x;
    double y;
    double z;
} t_vec;

typedef struct s_ray {
    t_vec origin;
    t_vec direction;
} t_ray ;

typedef struct s_sphere {
    t_vec origin;
    double radius;
}t_sphere;

// vector.c 
t_vec vec_plus(const t_vec a, const t_vec b);
t_vec vec_minus(const t_vec a, const t_vec b);
t_vec vec_mult(double k, const t_vec v);
t_vec vec_div(const t_vec v, double k);
double vec_scal(const t_vec a, const t_vec b);

// vector1.c
double norm2(const t_vec v);
t_vec normalize(const t_vec v);

// wrap_malloc.c
void	*wrap_malloc(t_rt *rt, size_t size);
void free_heap(t_rt *rt);
t_link_list *init_alloc(t_link_list **list);

// miniRT.c
// bmp.c
void save_img(t_rt * rt, const unsigned char * pixels, int W, int H); // rgb pixel
#endif
