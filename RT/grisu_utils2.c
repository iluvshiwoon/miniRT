/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grisu_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 14:25:30 by kgriset           #+#    #+#             */
/*   Updated: 2025/08/14 19:50:52 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"

void	get_normalized_boundaries(t_fp *fp, t_fp *lower, t_fp *upper)
{
	int	u_shift;
	int	l_shift;

	upper->frac = (fp->frac << 1) + 1;
	upper->exp = fp->exp - 1;
	while ((upper->frac & (HIDDENBIT << 1)) == 0)
	{
		upper->frac <<= 1;
		upper->exp--;
	}
	u_shift = 64 - 52 - 2;
	upper->frac <<= u_shift;
	upper->exp = upper->exp - u_shift;
	if (fp->frac == HIDDENBIT)
		l_shift = 2;
	else
		l_shift = 1;
	lower->frac = (fp->frac << l_shift) - 1;
	lower->exp = fp->exp - l_shift;
	lower->frac <<= lower->exp - upper->exp;
	lower->exp = upper->exp;
}

t_fp	multiply(t_fp *a, t_fp *b)
{
	t__fp	fp;

	fp.lomask = 0x00000000FFFFFFFF;
	fp.ah_bl = (a->frac >> 32) * (b->frac & fp.lomask);
	fp.al_bh = (a->frac & fp.lomask) * (b->frac >> 32);
	fp.al_bl = (a->frac & fp.lomask) * (b->frac & fp.lomask);
	fp.ah_bh = (a->frac >> 32) * (b->frac >> 32);
	fp.tmp = (fp.ah_bl & fp.lomask) + (fp.al_bh & fp.lomask) + (fp.al_bl >> 32);
	fp.tmp += 1U << 31;
	fp.fp = (t_fp){fp.ah_bh + (fp.ah_bl >> 32) + (fp.al_bh >> 32)
		+ (fp.tmp >> 32), a->exp + b->exp + 64};
	return (fp.fp);
}

void	round_digit(char *digits, t_digits d)
{
	uint64_t	frac;

	frac = d.wfrac * *(d.unit);
	while (d.part2 < frac && d.delta - d.part2 >= d.one.frac && (d.part2
			+ d.one.frac < frac || frac - d.part2 > d.part2 + d.one.frac
			- frac))
	{
		digits[d.idx - 1]--;
		d.part2 += d.one.frac;
	}
}

void	init_digits_struct(uint64_t *g_tens, t_digits *d, t_grisu2 *g)
{
	d->idx = 0;
	d->wfrac = g->upper.frac - g->w.frac;
	d->delta = g->upper.frac - g->lower.frac;
	d->one.frac = 1ULL << -g->upper.exp;
	d->one.exp = g->upper.exp;
	d->part1 = g->upper.frac >> -d->one.exp;
	d->part2 = g->upper.frac & (d->one.frac - 1);
	d->kappa = 10;
	d->divp = g_tens + 10;
}

int	process_integer_part(t_digits *d, char *digits, int *K)
{
	while (d->kappa > 0)
	{
		d->div = *(d->divp);
		d->digit = d->part1 / d->div;
		if (d->digit || d->idx)
			digits[d->idx++] = d->digit + '0';
		d->part1 -= d->digit * d->div;
		d->kappa--;
		d->tmp = (d->part1 << -d->one.exp) + d->part2;
		if (d->tmp <= d->delta)
		{
			*K += d->kappa;
			while (d->tmp < d->wfrac && d->delta - d->tmp >= d->div << \
				-d->one.exp && ((d->tmp + d->div) << -d->one.exp < d->wfrac
					|| d->wfrac - d->tmp > (d->tmp + d->div) << ((-d->one.exp)
						- d->wfrac)))
			{
				digits[d->idx - 1]--;
				d->tmp += d->div << -d->one.exp;
			}
			return (d->idx);
		}
		d->divp++;
	}
	return (-1);
}
