#include "get_next_line.h"

// only s2 is checked for NULL
// only s1 is freed
char *ft_strjoin_s2(char *s1, char *s2)
{
    char *str;

    int lens1;
    int lens2;
    if (!s2)
        return (NULL);
    if (!s1)
    {
        str = ft_strdup(s2);
        if (str == NULL)
            return (NULL);
        return (str);
    }
    lens1 = ft_strlen(s1);
    lens2 = ft_strlen(s2);
    str = (char *)malloc(sizeof(char) * (lens1 + lens2 + 1));
    if (str == NULL)
        return (NULL);
    ft_strcpy(str, s1);
    ft_strcpy(str + lens1, s2);
    free(s1);
    return (str);
}

char *end_of_line(char *line, char *buffer)
{
    int i;
    char *tmp;

    i = 0;
    i = find_chr(buffer, '\n');
    buffer[i] = '\0';
    line = ft_strjoin_s2(line, buffer);
    if (line == NULL)
        return (NULL);
    line = ft_strjoin_s2(line, "\n");
    if (line == NULL)
        return (NULL);
    i++;
    tmp = ft_strdup(buffer + i);
    if (tmp == NULL)
        return (NULL);
    ft_bezero(buffer, BUFFER_SIZE + 1);
    ft_strcpy(buffer, tmp);
    free(tmp);
    return (line);
}

char *alloc_line(int fd, char *buffer)
{
    char *line;

    line = NULL;
    if (buffer[0] == '\0')
    {
        if (read(fd, buffer, BUFFER_SIZE) == 0)
            return (NULL);
        buffer[BUFFER_SIZE] = '\0';
    }
    while (buffer[0] != '\0')
    {
        if (find_chr(buffer, '\n') != -1)
        {
            line = end_of_line(line, buffer);
            return (line);
        }
        line = ft_strjoin_s2(line, buffer);
        if (line == NULL)
            return (NULL);
        ft_bezero(buffer, BUFFER_SIZE + 1);
        if (read(fd, buffer, BUFFER_SIZE) == 0)
            return (line);
    }
    return (line);
}

char *get_next_line(int fd)
{
    char *line;
    static char buffer[BUFFER_SIZE + 1];

    if (fd < 0 || read(fd, 0, 0) < 0 || BUFFER_SIZE <= 0)
		return (NULL);
    line = alloc_line(fd, buffer);
    return (line);
}

// int main(void)
// {
//     int fd;
//     char *line;
//     int i = 0;

//     fd = open("test.txt", O_RDONLY);
//     if (fd == -1)
//     {
//         printf("Error in opening file\n");
//         return (1);
//     }
//     while (i < 1000)
//     {
//         line = get_next_line(fd);
//         if (line == NULL)
//             break;
//         printf("%s", line);
//         free(line);
//         i++;
//     }
// }
