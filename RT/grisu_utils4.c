/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grisu_utils4.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 14:25:30 by kgriset           #+#    #+#             */
/*   Updated: 2025/08/20 14:54:57 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"

int	process_fractional_part(uint64_t *g_tens, t_digits *d, \
		char *digits, int *K)
{
	d->unit = g_tens + 18;
	while (true)
	{
		d->part2 *= 10;
		d->delta *= 10;
		d->kappa--;
		d->digit = d->part2 >> -d->one.exp;
		if (d->digit || d->idx)
			digits[d->idx++] = d->digit + '0';
		d->part2 &= d->one.frac - 1;
		if (d->part2 < d->delta)
		{
			*K += d->kappa;
			round_digit(digits, *d);
			return (d->idx);
		}
		d->unit--;
	}
}

int	generate_digits(uint64_t *g_tens, t_grisu2 *g, char *digits, int *K)
{
	t_digits	d;
	int			result;

	init_digits_struct(g_tens, &d, g);
	result = process_integer_part(&d, digits, K);
	if (result != -1)
		return (result);
	return (process_fractional_part(g_tens, &d, digits, K));
}

int	grisu2(double d, char *digits, int *K)
{
	int			k;
	t_fp		cp;
	t_grisu2	g;
	uint64_t	*g_tens;

	g_tens = (uint64_t[]){10000000000000000000U, \
	1000000000000000000U, 100000000000000000U, 10000000000000000U, \
	1000000000000000U, 100000000000000U, 10000000000000U, 1000000000000U, \
	100000000000U, 10000000000U, 1000000000U, 100000000U, 10000000U, \
	1000000U, 100000U, 10000U, 1000U, 100U, 10U, 1U};
	g.w = build_fp(d);
	get_normalized_boundaries(&(g.w), &(g.lower), &(g.upper));
	_normalize(&(g.w));
	cp = find_cachedpow10(g.upper.exp, &k);
	g.w = multiply(&(g.w), &cp);
	g.upper = multiply(&(g.upper), &cp);
	g.lower = multiply(&(g.lower), &cp);
	g.lower.frac++;
	g.upper.frac--;
	*K = -k;
	return (generate_digits(g_tens, &g, digits, K));
}

int	emit_scientific_exponent(t_fpconv f, char *dest, int idx)
{
	char	sign;
	int		exp;

	dest[(idx)++] = 'e';
	if (f.k + f.ndigits - 1 < 0)
		sign = '-';
	else
		sign = '+';
	dest[(idx)++] = sign;
	exp = absv(f.k + f.ndigits - 1);
	return (emit_exponent_digits(exp, dest, idx));
}

int	emit_scientific_format(t_fpconv f, char *dest)
{
	int	idx;

	emit_scientific_mantissa(f, dest, &idx);
	return (emit_scientific_exponent(f, dest, idx));
}
