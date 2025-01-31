/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gschwand <gschwand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 10:48:32 by gschwand          #+#    #+#             */
/*   Updated: 2025/01/31 09:43:20 by gschwand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"

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

void	free_lst_file(t_file **file)
{
	t_file	*tmp;
	t_file	*tmp2;

	tmp = *file;
	while (tmp)
	{
		tmp2 = tmp;
		tmp = tmp->next;
		free(tmp2->line);
		free(tmp2);
	}
}

t_file	*lstnew_file(char *line)
{
	t_file	*new;

	new = malloc(sizeof(t_file));
	if (!new)
		return (NULL);
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

int	alloc_file_lst(t_file **file, int fd)
{
	t_file	*node;
	char	*line;

	node = *file;
	while (!*file || node->line)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		if (line[0] != '\n')
		{
			line[ft_strlen(line) - 1] = '\0';
			node = lstnew_file(line);
			if (!node)
				return (free_lst_file(file), ft_putstr_fd("Error memory\n", 2),
					1);
			lst_add_back_file(file, node);
		}
		else
			free(line);
	}
	return (0);
}

t_file	**open_file(char *namefile)
{
	int		fd;
	t_file	**file;
	t_file	*node;
	char	*line;

	file = malloc(sizeof(t_file *));
	if (!file)
		return (ft_putstr_fd("Error memory\n", 2), NULL);
	*file = NULL;
	fd = open(namefile, O_RDONLY);
	line = get_next_line(fd);
	if (!line)
		return (ft_putstr_fd("Error file can't be read\n", 2), NULL);
	if (line[0] != '\n')
	{
		line[ft_strlen(line) - 1] = '\0';
		*file = lstnew_file(line);
	}
	if (alloc_file_lst(file, fd))
		return (NULL);
	return (file);
}
