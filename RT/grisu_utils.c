/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grisu_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 14:25:30 by kgriset           #+#    #+#             */
/*   Updated: 2025/07/25 14:58:57 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"

int	absv(int n)
{
	if (n < 0)
		return (-1 * n);
	return (n);
}

int	min(int a, int b)
{
	if (a < b)
		return (a);
	return (b);
}

uint64_t	get_dbits(double d)
{
	union u_dbl_bits	dbl_bits;

	dbl_bits = (union u_dbl_bits){d};
	return (dbl_bits.i);
}

t_fp	build_fp(double d)
{
	uint64_t	bits;
	t_fp		fp;

	bits = get_dbits(d);
	fp.frac = bits & FRACMASK;
	fp.exp = (bits & EXPMASK) >> 52;
	if (fp.exp)
	{
		fp.frac += HIDDENBIT;
		fp.exp -= EXPBIAS;
	}
	else
	{
		fp.exp = -EXPBIAS + 1;
	}
	return (fp);
}

void	_normalize(t_fp *fp)
{
	int	shift;

	while ((fp->frac & HIDDENBIT) == 0)
	{
		fp->frac <<= 1;
		fp->exp--;
	}
	shift = 64 - 52 - 1;
	fp->frac <<= shift;
	fp->exp -= shift;
}
