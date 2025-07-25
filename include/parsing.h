/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gschwand <gschwand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 11:01:40 by gschwand          #+#    #+#             */
/*   Updated: 2025/07/25 14:31:02 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H
# ifndef RT_H
#  include "rt.h"
# endif

typedef struct s_file
{
	char			*line;
	struct s_file	*next;
}					t_file;

typedef struct s_parser
{
	char			*id;
	void			(*parse)(t_rt *rt, char *line, int *id);
}					t_parser;

// parsing.c
void				parsing_minirt(t_rt *rt, char *namefile);

// check_args.c
int					check_args(int ac, char **av);

// open_file.c
t_file				**open_file(t_rt *rt, char *namefile);
void				print_lst_file(t_file **file);

// alloc_rt.c
void				alloc_rt(t_rt *rt, t_file **file);
void				check_files(t_rt *rt, t_file **file);
// scene_utils.c
void				creat_struct_parser_minirt(t_parser *parser);
void				creat_scene(t_rt *rt, t_file **file);
// element/element_utils.c
t_vec				parse_color(t_rt *rt, char *line);
t_vec				parse_vec(t_rt *rt, char *line);
void				print_scene(t_rt *rt);

// element/Ambient_light.c
void				parse_amb_light(t_rt *rt, char *line, int *id);

// element/atoi_double.c
double				ft_atoi_double(char *s);

// element/cylinder.c
void				parse_cylinder(t_rt *rt, char *line, int *id);

// element/light.c
void				parse_light(t_rt *rt, char *line, int *id);

// element/parse_camera.c
void				parse_camera(t_rt *rt, char *line, int *id);

// element/plane.c
void				parse_plane(t_rt *rt, char *line, int *id);

// element/sphere.c
void				parse_sphere(t_rt *rt, char *line, int *id);

#endif
