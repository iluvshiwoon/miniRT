/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   atoi_double.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gschwand <gschwand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 13:51:53 by gschwand          #+#    #+#             */
/*   Updated: 2025/01/29 14:00:25 by gschwand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/parsing.h"

double absolut_value(const char *s)
{
    double result;
    int decimal_found;
    double decimal_divisor;

    result = 0.0;
    decimal_found = 0;
    decimal_divisor = 10.0;
    while (*s) 
    {
        if (isdigit((unsigned char)*s)) 
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

double  ft_atoi_double(char *s)
{
    if (!s)
        return (0.0);
    double result;
    double sign;
    sign = 1.0;
    while (isspace((unsigned char)*s))
        s++;
    if (*s == '-') 
    {
        sign = -1.0;
        s++;
    } 
    else if (*s == '+')
        s++;
    result = absolut_value(s) * sign;
    return (result);
}
