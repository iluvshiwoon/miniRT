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


t_file	*lstnew_file(t_rt * rt, char *line)
{
	t_file	*new;

	new = wrap_malloc(rt, sizeof(t_file));
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
			lst_add_back_file(file, node);
		}
	}
}

t_file	**open_file(t_rt * rt, char *namefile)
{
	t_file	**file;
	char	*line;

    file = wrap_malloc(rt, sizeof(t_file *));
	*file = NULL;
	rt->fd_file = open(namefile, O_RDONLY); // check_error
	line = get_next_line(rt->fd_file);
    namefile = line;
    if (namefile)
        line = rt_ft_strdup(rt, namefile);
    free(namefile);
	if (!line)
		return (exit_error(rt, "Error file can't be read\n"), NULL);
	if (line[0] != '\n')
	{
		line[ft_strlen(line) - 1] = '\0';
		*file = lstnew_file(rt, line);
	}
	alloc_file_lst(rt, file, rt->fd_file);
	return (file);
}
