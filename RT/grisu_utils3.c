/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grisu_utils3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 14:25:30 by kgriset           #+#    #+#             */
/*   Updated: 2025/07/24 15:40:34 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"

int	emit_integer_format(t_fpconv f, char *dest)
{
	ft_memcpy(dest, f.digits, f.ndigits);
	ft_memset(dest + f.ndigits, '0', f.k);
	return (f.ndigits + f.k);
}

int	emit_decimal_leading_zeros(t_fpconv f, char *dest, int offset)
{
	offset = -(offset);
	dest[0] = '0';
	dest[1] = '.';
	ft_memset(dest + 2, '0', offset);
	ft_memcpy(dest + offset + 2, f.digits, f.ndigits);
	return (f.ndigits + 2 + offset);
}

int	emit_decimal_with_point(t_fpconv f, char *dest, int offset)
{
	ft_memcpy(dest, f.digits, offset);
	dest[offset] = '.';
	ft_memcpy(dest + offset + 1, f.digits + offset, f.ndigits - offset);
	return (f.ndigits + 1);
}

int	emit_scientific_mantissa(t_fpconv f, char *dest, int *idx)
{
	f.ndigits = min(f.ndigits, 18 - f.neg);
	*idx = 0;
	dest[(*idx)++] = (f.digits)[0];
	if (f.ndigits > 1)
	{
		dest[(*idx)++] = '.';
		ft_memcpy(dest + *idx, f.digits + 1, f.ndigits - 1);
		*idx += f.ndigits - 1;
	}
	return (f.ndigits);
}

int	emit_exponent_digits(int exp, char *dest, int idx)
{
	int	cent;
	int	dec;

	cent = 0;
	if (exp > 99)
	{
		cent = exp / 100;
		dest[(idx)++] = cent + '0';
		exp -= cent * 100;
	}
	if (exp > 9)
	{
		dec = exp / 10;
		dest[(idx)++] = dec + '0';
		exp -= dec * 10;
	}
	else if (cent)
	{
		dest[(idx)++] = '0';
	}
	dest[(idx)++] = exp % 10 + '0';
	return (idx);
}
