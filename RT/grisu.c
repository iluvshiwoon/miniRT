/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grisu.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 17:53:36 by kgriset           #+#    #+#             */
/*   Updated: 2025/07/23 19:11:45 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../miniRT.h"
#include "powers.h"
#include <stdint.h>

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

static inline uint64_t	get_dbits(double d)
{
	union u_dbl_bits	dbl_bits;

	dbl_bits = (union u_dbl_bits){d};
	return (dbl_bits.i);
}

static Fp	build_fp(double d)
{
	uint64_t	bits;
	Fp			fp;

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

static void	_normalize(Fp *fp)
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

static void	get_normalized_boundaries(Fp *fp, Fp *lower, Fp *upper)
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

typedef struct s_Fp
{
	uint64_t			lomask;
	uint64_t			ah_bl;
	uint64_t			al_bh;
	uint64_t			al_bl;
	uint64_t			ah_bh;
	uint64_t			tmp;
	Fp					fp;
}						t_Fp;

typedef struct s_digits
{
	uint64_t			wfrac;
	uint64_t			delta;
	Fp					one;
	uint64_t			part1;
	uint64_t			part2;
	int					idx;
	int					kappa;
	uint64_t			*divp;
	uint64_t			div;
	unsigned int		digit;
	uint64_t			tmp;
	uint64_t			*unit;
}						t_digits;

static Fp	multiply(Fp *a, Fp *b)
{
	t_Fp	fp;

	fp.lomask = 0x00000000FFFFFFFF;
	fp.ah_bl = (a->frac >> 32) * (b->frac & fp.lomask);
	fp.al_bh = (a->frac & fp.lomask) * (b->frac >> 32);
	fp.al_bl = (a->frac & fp.lomask) * (b->frac & fp.lomask);
	fp.ah_bh = (a->frac >> 32) * (b->frac >> 32);
	fp.tmp = (fp.ah_bl & fp.lomask) + (fp.al_bh & fp.lomask) + (fp.al_bl >> 32);
	fp.tmp += 1U << 31;
	fp.fp = (Fp){fp.ah_bh + (fp.ah_bl >> 32) + (fp.al_bh >> 32)
		+ (fp.tmp >> 32), a->exp + b->exp + 64};
	return (fp.fp);
}

static void	round_digit(char *digits, t_digits d)
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

typedef struct s_grisu2
{
	Fp					w;
	Fp					lower;
	Fp					upper;
}						t_grisu2;

static void	init_digits_struct(uint64_t *g_tens, t_digits *d, t_grisu2 *g)
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

static int	process_integer_part(t_digits *d, char *digits, int *K)
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

static int	process_fractional_part(uint64_t *g_tens, t_digits *d, \
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

static int	generate_digits(uint64_t *g_tens, t_grisu2 *g, char *digits, int *K)
{
	t_digits	d;
	int			result;

	init_digits_struct(g_tens, &d, g);
	result = process_integer_part(&d, digits, K);
	if (result != -1)
		return (result);
	return (process_fractional_part(g_tens, &d, digits, K));
}

static int	grisu2(double d, char *digits, int *K)
{
	int			k;
	Fp			cp;
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

typedef struct s_fpconv
{
	char				digits[18];
	int					str_len;
	bool				neg;
	int					spec;
	int					k;
	int					ndigits;
}						t_fpconv;

typedef struct s_emit_digits
{
	int					exp;
	int					max_trailing_zeros;
	int					offset;
	int					idx;
	char				sign;
	int					cent;
	int					dec;
}						t_emit_digits;

static int	emit_integer_format(t_fpconv f, char *dest)
{
	ft_memcpy(dest, f.digits, f.ndigits);
	ft_memset(dest + f.ndigits, '0', f.k);
	return (f.ndigits + f.k);
}

static int	emit_decimal_leading_zeros(t_fpconv f, char *dest, int offset)
{
	offset = -(offset);
	dest[0] = '0';
	dest[1] = '.';
	ft_memset(dest + 2, '0', offset);
	ft_memcpy(dest + offset + 2, f.digits, f.ndigits);
	return (f.ndigits + 2 + offset);
}

static int	emit_decimal_with_point(t_fpconv f, char *dest, int offset)
{
	ft_memcpy(dest, f.digits, offset);
	dest[offset] = '.';
	ft_memcpy(dest + offset + 1, f.digits + offset, f.ndigits - offset);
	return (f.ndigits + 1);
}

static int	emit_scientific_mantissa(t_fpconv f, char *dest, int *idx)
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

static int	emit_exponent_digits(int exp, char *dest, int idx)
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

static int	emit_scientific_exponent(t_fpconv f, char *dest, int idx)
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

static int	emit_scientific_format(t_fpconv f, char *dest)
{
	int	idx;

	emit_scientific_mantissa(f, dest, &idx);
	return (emit_scientific_exponent(f, dest, idx));
}

static int	emit_digits(t_fpconv f, char *dest)
{
	t_emit_digits	e;

	e.exp = absv(f.k + f.ndigits - 1);
	e.max_trailing_zeros = 7;
	if (f.neg)
		e.max_trailing_zeros -= 1;
	if (f.k >= 0 && (e.exp < (f.ndigits + e.max_trailing_zeros)))
		return (emit_integer_format(f, dest));
	if (f.k < 0 && (f.k > -7 || e.exp < 4))
	{
		e.offset = f.ndigits - absv(f.k);
		if (e.offset <= 0)
			return (emit_decimal_leading_zeros(f, dest, e.offset));
		else
			return (emit_decimal_with_point(f, dest, e.offset));
	}
	return (emit_scientific_format(f, dest));
}

static int	filter_special(double fp, char *dest)
{
	uint64_t	bits;
	bool		nan;

	if (fp == 0.0)
	{
		dest[0] = '0';
		return (1);
	}
	bits = get_dbits(fp);
	nan = (bits & EXPMASK) == EXPMASK;
	if (!nan)
		return (0);
	if (bits & FRACMASK)
	{
		dest[0] = 'n';
		dest[1] = 'a';
		dest[2] = 'n';
	}
	else
	{
		dest[0] = 'i';
		dest[1] = 'n';
		dest[2] = 'f';
	}
	return (3);
}

int	fpconv_dtoa(double d, char dest[24])
{
	t_fpconv	f;

	f.str_len = 0;
	f.neg = false;
	if (get_dbits(d) & SIGNMASK)
	{
		dest[0] = '-';
		(f.str_len)++;
		f.neg = true;
	}
	f.spec = filter_special(d, dest + f.str_len);
	if (f.spec)
	{
		return (f.str_len + f.spec);
	}
	f.k = 0;
	f.ndigits = grisu2(d, f.digits, &(f.k));
	f.str_len += emit_digits(f, dest + f.str_len);
	return (f.str_len);
}
