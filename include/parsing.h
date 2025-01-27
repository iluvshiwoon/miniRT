#ifndef PARSING_H
# define PARSING_H

# include "miniRT.h"

typedef struct s_file {
    char            *line;
    struct s_file   *next;
}   t_file;

// parsing.c
int parsing_minirt(char *namefile);

// check_args.c
int check_args(int ac, char **av);

// open_file.c
t_file **open_file(char *namefile);
void print_lst_file(t_file **file);
void free_lst_file(t_file **file);

// alloc_rt.c
t_rt *alloc_rt(t_file **file);

// element/element_utils.c
t_vec *parse_color(char *line);
t_vec *parse_vec(char *line);

// element/Ambient_light.c
int parse_amb_light(t_rt *rt, char *line);

// element/atoi_double.c
double ft_atoi_double(char *s);

// element/cylinder.c
int parse_cylinder(t_rt *rt, char *line);

// element/light.c
int parse_light(t_rt *rt, char *line);

// element/parse_camera.c
int parse_camera(t_rt *rt, char *line);

// element/plane.c
int parse_plane(t_rt *rt, char *line); 

// element/sphere.c
int parse_sphere(t_rt *rt, char *line);

#endif