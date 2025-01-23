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
t_rt alloc_rt(t_file **file);

#endif