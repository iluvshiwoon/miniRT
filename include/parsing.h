/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gschwand <gschwand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 11:01:40 by gschwand          #+#    #+#             */
/*   Updated: 2025/04/01 08:23:27 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H
# ifndef RT_H
#  include "rt.h"
# endif
typedef struct s_file {
    char            *line;
    struct s_file   *next;
}   t_file;

// typedef struct s_parser {
//     char *id;
//     int (*parse)(t_rt *rt, char *line);
// }  t_parser;
typedef struct s_parser {
    char *id;
    void (*parse)(t_rt *rt, char *line);
}  t_parser;

// parsing.c
void parsing_minirt(t_rt *rt, char *namefile);

// check_args.c
int check_args(int ac, char **av);

// open_file.c
t_file	**open_file(t_rt * rt, char *namefile);
void print_lst_file(t_file **file);
// void free_lst_file(t_file **file);

// alloc_rt.c
void alloc_rt(t_rt * rt, t_file **file);

// element/element_utils.c
// t_vec *parse_color(t_rt *rt, char *line);
t_vec	parse_color(t_rt *rt, char *line);
t_vec parse_vec(t_rt *rt, char *line);
// void free_tab_char(char **tab);
void print_scene(t_rt *rt);

// element/Ambient_light.c
// int parse_amb_light(t_rt *rt, char *line);
void	parse_amb_light(t_rt *rt, char *line);

// element/atoi_double.c
double ft_atoi_double(char *s);

// element/cylinder.c
// int parse_cylinder(t_rt *rt, char *line);
void	parse_cylinder(t_rt *rt, char *line);

// element/light.c
// int parse_light(t_rt *rt, char *line);
void	parse_light(t_rt *rt, char *line);

// element/parse_camera.c
// int parse_camera(t_rt *rt, char *line);
void parse_camera(t_rt *rt, char *line);

// element/plane.c
// int parse_plane(t_rt *rt, char *line); 
void	parse_plane(t_rt *rt, char *line);

// element/sphere.c
void	parse_sphere(t_rt *rt, char *line);
// int parse_sphere(t_rt *rt, char *line);

#endif
