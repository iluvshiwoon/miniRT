/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 14:25:41 by kgriset           #+#    #+#             */
/*   Updated: 2025/04/01 08:23:05 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef RT_H
# define RT_H

# include "../42_MyLibC/mylibc.h"

typedef struct s_vec t_vec;

typedef struct s_vec {
    double x;
    double y;
    double z;
} t_vec;

typedef struct s_ray {
    t_vec origin;
    t_vec direction;
} t_ray;

typedef struct s_ambient_light {
    double intensity;
    t_vec color;
} t_ambient_light;

typedef struct s_light {
    t_vec origin;
    double intensity;
    t_vec color;
} t_light;

typedef struct s_camera {
    t_vec origin;
    t_vec direction;
    double fov;
} t_camera;

typedef struct s_sphere {
    int id;
    t_vec origin;
    double radius;
    t_vec albedo;
}t_sphere;

typedef struct s_plane {
    int id;
    t_vec origin;
    t_vec normal;
    t_vec albedo;
} t_plane;

typedef struct s_cylinder {
    int id;
    t_vec origin;
    t_vec direction;
    double radius;
    double height;
    t_vec albedo;
} t_cylinder;

typedef struct s_scene {
    t_light light;    
    t_ambient_light ambient_light;
    t_camera camera;

    int spheres_nb;
    t_sphere * spheres;

    int planes_nb;
    t_plane * planes;

    int cylinders_nb;
    t_cylinder * cylinders;

    // int light_nb;
    // t_vec * light;
    // double * light_intensity;

} t_scene; 

typedef struct s_rt {
    int W;
    int H;
    int fd_file;
    double fov;
    t_link_list * graphic_heap;
    t_link_list * parsing_heap;
    t_link_list * current_heap;
    t_scene scene;
} t_rt;

// vector.c 
t_vec vec_plus(const t_vec a, const t_vec b);
t_vec vec_minus(const t_vec a, const t_vec b);
t_vec vec_mult(double k, const t_vec v);
t_vec vec_div(const t_vec v, double k);
double vec_scal(const t_vec a, const t_vec b);

// vector1.c
double norm2(const t_vec v);
t_vec normalize(const t_vec v);
t_vec vec_m_vec(const t_vec a, const t_vec b);

// wrap_malloc.c
void	*wrap_malloc(t_rt *rt, size_t size);
void free_heap(t_rt *rt);
t_link_list *init_alloc(t_link_list **list);

// miniRT.c
// bmp.c
void save_img(t_rt * rt, const unsigned char * pixels, int W, int H); // rgb pixel
// exit_error.c
void exit_error(t_rt * rt, char * msg);
// rt_strdup.c
char	*rt_ft_strdup(t_rt *rt, const char *s);
// rt_ft_strtrim.c
char	*rt_ft_strtrim(t_rt *rt, char const *s1, char const *set);
// rt_ft_substr.c
char	*rt_ft_substr(t_rt *rt, char const *s, unsigned int start, size_t len);
// rt_ft_split.c
char	**rt_ft_split(t_rt *rt, char const *s, char c);
#endif
