/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 16:42:39 by kgriset           #+#    #+#             */
/*   Updated: 2025/03/25 16:43:02 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"

char	*rt_ft_strdup(t_rt *rt, const char *s)
{
	char			*s_dupe;
	unsigned int	i;

	i = 0;
	while (s[i])
		i++;
	s_dupe = wrap_malloc(rt, (i + 1) * sizeof(char));
	if (!s_dupe)
		return (0);
	i = 0;
	while (s[i])
	{
		s_dupe[i] = s[i];
		i++;
	}
	s_dupe[i] = s[i];
	return (s_dupe);
}
