/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wrap_malloc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 21:20:43 by kgriset           #+#    #+#             */
/*   Updated: 2025/04/01 08:32:45 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"

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
    if (rt->parsing_heap)
    {
        rt->parsing_heap->pf_lstclear(&rt->parsing_heap->first_node, free);
        free(rt->parsing_heap);
    }
}

void	*wrap_malloc(t_rt *rt, size_t size) // add memory error msg ?
{
    t_list *new_node;

	if (!rt)
		return (NULL);
	new_node = malloc(sizeof(*new_node));
	if (!new_node)
		return (exit_error(rt,"malloc failed"), NULL);
        *new_node = (t_list){};
	new_node->content = malloc(size);
	if (!new_node->content)
		return (free(new_node), exit_error(rt,"malloc failed"), NULL);
    rt->current_heap->pf_lstadd_back(&rt->current_heap->first_node, new_node);
	return (new_node->content);
}
