/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wrap_malloc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gschwand <gschwand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 21:20:43 by kgriset           #+#    #+#             */
/*   Updated: 2025/01/31 11:21:31 by gschwand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/parsing.h"

t_link_list *init_alloc(t_link_list **list)
{
	*list = malloc(sizeof(**list));
	if (!list)
		return (NULL);
	**list = (t_link_list){};
	init_link_list(*list);
	return (*list);
}

void free_heap(t_rt *rt)
{
    if (rt->graphic_heap)
    {
        rt->graphic_heap->pf_lstclear(&rt->graphic_heap->first_node, free);
        free(rt->graphic_heap);
    }
}

void	*wrap_malloc(t_rt *rt, size_t size)
{
    t_list *new_node;

	if (!rt)
		return (NULL);
	new_node = malloc(sizeof(*new_node));
	if (!new_node)
		return (free_heap(rt), exit(EXIT_FAILURE),
			NULL);
	*new_node = (t_list){};
	new_node->content = malloc(size);
	if (!new_node->content)
		return (free(new_node), free_heap(rt),
			exit(EXIT_FAILURE), NULL);
    rt->current_heap->pf_lstadd_back(&rt->current_heap->first_node, new_node);
	return (new_node->content);
}
