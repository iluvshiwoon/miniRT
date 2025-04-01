/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_ft_split.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 16:41:18 by kgriset           #+#    #+#             */
/*   Updated: 2025/03/26 13:48:39 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"

static void	rt_free_split(char **split, char *s_trimmed, size_t size)
{
	size_t	i;

	i = 0;
	while (i < size)
	{
		free(split[i]);
		i++;
	}
	free(s_trimmed);
	free(split);
}

static char	*rt_count_split(t_rt *rt, const char *s, char c,
		size_t *sub_str_count)
{
	char	set[2];
	char	*s_trimmed;
	size_t	i;

	i = 0;
	*sub_str_count = 1;
	set[0] = c;
	set[1] = '\0';
	s_trimmed = rt_ft_strtrim(rt, s, set);
	if (!s_trimmed)
		return (NULL);
	if (*s_trimmed == 0)
		(*sub_str_count)--;
	while (s_trimmed[i])
	{
		if (s_trimmed[i] == c && s_trimmed[i + 1] != c)
			(*sub_str_count)++;
		i++;
	}
	return (s_trimmed);
}

static char	**rt_make_split(t_rt *rt, char **split, char *s_trimmed,
		char c)
{
	size_t	i;
	size_t	j;
	size_t	start;

	i = 0;
	j = 0;
	start = 0;
	while (s_trimmed[i])
	{
		start = i;
		while (s_trimmed[i] != c && s_trimmed[i])
			i++;
		split[j] = rt_ft_substr(rt, s_trimmed, start, i - start);
		if (!split[j])
		{
			rt_free_split(split, s_trimmed, j + 1);
			return (NULL);
		}
		j++;
		while (s_trimmed[i] == c && c != '\0')
			i++;
	}
	split[j] = NULL;
	return (split);
}

char	**rt_ft_split(t_rt *rt, char const *s, char c)
{
	size_t	sub_str_count;
	char	**split;
	char	*s_trimmed;

	s_trimmed = rt_count_split(rt, s, c, &sub_str_count);
	split = wrap_malloc(rt,  sizeof(*split)
			* (sub_str_count + 1));
	if (!split)
	{
		free(s_trimmed);
		return (NULL);
	}
	return (rt_make_split(rt, split, s_trimmed, c));
}
