/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gschwand <gschwand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 10:48:32 by gschwand          #+#    #+#             */
/*   Updated: 2025/04/02 15:26:01 by gschwand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"

void	print_lst_file(t_file **file)
{
	t_file	*tmp;

	tmp = *file;
	while (tmp)
	{
		printf("%s\n", tmp->line);
		tmp = tmp->next;
	}
}

// void	free_lst_file(t_file **file)
// {
// 	t_file	*tmp;
// 	t_file	*tmp2;
//
// 	tmp = *file;
// 	while (tmp)
// 	{
// 		tmp2 = tmp;
// 		tmp = tmp->next;
// 		free(tmp2->line);
// 		free(tmp2);
// 	}
// 	free(file);
// }

t_file	*lstnew_file(t_rt * rt, char *line)
{
	t_file	*new;

	// new = malloc(sizeof(t_file));
	new = wrap_malloc(rt, sizeof(t_file));
	// if (!new)
	// 	return (NULL);
	new->line = line;
	new->next = NULL;
	return (new);
}

void	lst_add_back_file(t_file **file, t_file *node)
{
	t_file	*tmp;

	if (!*file)
	{
		*file = node;
		return ;
	}
	tmp = *file;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = node;
}

// int	alloc_file_lst(t_rt * rt, t_file **file, int fd)
void	alloc_file_lst(t_rt * rt, t_file **file, int fd)
{
	t_file	*node;
	char	*line;
    char *temp;

	node = *file;
	while (!*file || node->line)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
        temp = line;
        line = rt_ft_strdup(rt, line);
        free(temp);
		if (line[0] != '\n')
		{
			if (line[ft_strlen(line) - 1] == '\n')
				line[ft_strlen(line) - 1] = '\0';
			node = lstnew_file(rt, line);
			// if (!node)
			// 	return (free_lst_file(file), ft_putstr_fd("Error memory\n", 2),
			// 		1);
			lst_add_back_file(file, node);
		}
	}
	// return (0);
}

t_file	**open_file(t_rt * rt, char *namefile)
{
	t_file	**file;
	t_file	*node;
	char	*line;

	// file = malloc(sizeof(t_file *));
    file = wrap_malloc(rt, sizeof(t_file *));
	// if (!file)
	// 	return (ft_putstr_fd("Error memory\n", 2), NULL);
	*file = NULL;
	rt->fd_file = open(namefile, O_RDONLY);
	line = get_next_line(rt->fd_file);
    namefile = line;
    if (namefile)
        line = rt_ft_strdup(rt, namefile);
    free(namefile);
	if (!line)
		return (exit_error(rt, "Error file can't be read\n"), NULL);
		// return (ft_putstr_fd("Error file can't be read\n", 2), NULL);
	if (line[0] != '\n')
	{
		line[ft_strlen(line) - 1] = '\0';
		*file = lstnew_file(rt, line);
	}
	alloc_file_lst(rt, file, rt->fd_file);
	// if (alloc_file_lst(rt, file, fd))
	// 	return (NULL);
	return (file);
}
