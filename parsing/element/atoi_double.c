/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   atoi_double.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gschwand <gschwand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 13:51:53 by gschwand          #+#    #+#             */
/*   Updated: 2025/03/26 17:11:03 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../miniRT.h"

double	absolut_value(const char *s, double result)
{
	int		decimal_found;
	double	decimal_divisor;

	decimal_found = 0;
	decimal_divisor = 10.0;
	while (*s)
	{
		if (ft_isdigit((unsigned char)*s))
		{
			if (!decimal_found)
				result = result * 10.0 + (*s - '0');
			else
			{
				result += (*s - '0') / decimal_divisor;
				decimal_divisor *= 10.0;
			}
		}
		else if (*s == '.' && !decimal_found)
			decimal_found = 1;
		else
			return (result);
		s++;
	}
	return (result);
}

double	ft_atoi_double(char *s)
{
	double	result;
	double	sign;

	if (!s)
		return (0.0);
	sign = 1.0;
	result = 0.0;
	if (*s == '-')
	{
		sign = -1.0;
		s++;
	}
	else if (*s == '+')
		s++;
	result = absolut_value(s, result) * sign;
	return (result);
}
