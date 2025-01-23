#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <stdio.h>

# define BUFFER_SIZE 1024

int find_chr(char *str, char c);
void ft_bezero(char *str, int n);
int ft_strlen(char *str);
void ft_strcpy(char *dest, char *src);
char *ft_strdup(char *str);
char *get_next_line(int fd);

#endif