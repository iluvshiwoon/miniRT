#include "get_next_line.h"


int find_chr(char *str, char c)
{
    int i;

    i = 0;
    while (str[i] != '\0')
    {
        if (str[i] == c)
            return (i);
        i++;
    }
    return (-1);
}

void ft_bezero(char *str, int n)
{
    int i;

    i = 0;
    while (i < n)
    {
        str[i] = '\0';
        i++;
    }
}

int ft_strlen(char *str)
{
    int i;

    i = 0;
    while (str[i] != '\0')
        i++;
    return (i);
}

void ft_strcpy(char *dest, char *src)
{
    int i;

    i = 0;
    while (src[i] != '\0')
    {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

char *ft_strdup(char *str)
{
    char *new_str;

    new_str = (char *)malloc(sizeof(char) * (ft_strlen(str) + 1));
    if (new_str == NULL)
        return (NULL);
    ft_strcpy(new_str, str);
    return (new_str);
}