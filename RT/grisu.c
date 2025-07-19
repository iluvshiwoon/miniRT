/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grisu.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 17:53:36 by kgriset           #+#    #+#             */
/*   Updated: 2025/07/19 18:23:52 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../miniRT.h"
#include "fpconv.h"
#include "powers.h"

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

static uint64_t			g_tens[] = {10000000000000000000U, 1000000000000000000U,
				100000000000000000U, 10000000000000000U, 1000000000000000U,
				100000000000000U, 10000000000000U, 1000000000000U,
				100000000000U, 10000000000U, 1000000000U, 100000000U, 10000000U,
				1000000U, 100000U, 10000U, 1000U, 100U, 10U, 1U};


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
	uint64_t	lomask;
	uint64_t		ah_bl;
	uint64_t		al_bh;
	uint64_t		al_bl;
	uint64_t		ah_bh;
	uint64_t		tmp;
	Fp				fp;
}t_Fp;

static Fp	multiply(Fp *a, Fp *b)
{
	t_Fp fp;
	fp.lomask = 0x00000000FFFFFFFF;

	fp.ah_bl = (a->frac >> 32) * (b->frac & fp.lomask);
	fp.al_bh = (a->frac & fp.lomask) * (b->frac >> 32);
	fp.al_bl = (a->frac & fp.lomask) * (b->frac & fp.lomask);
	fp.ah_bh = (a->frac >> 32) * (b->frac >> 32);
	fp.tmp = (fp.ah_bl & fp.lomask) + (fp.al_bh & fp.lomask) + (fp.al_bl >> 32);
	fp.tmp += 1U << 31;
	fp.fp = (Fp){fp.ah_bh + (fp.ah_bl >> 32) + (fp.al_bh >> 32) + (fp.tmp >> 32), a->exp
		+ b->exp + 64};
	return (fp.fp);
}

static void	round_digit(char *digits, int ndigits, uint64_t delta, uint64_t rem,
		uint64_t kappa, uint64_t frac)
{
	while (rem < frac && delta - rem >= kappa && (rem + kappa < frac || frac
			- rem > rem + kappa - frac))
	{
		digits[ndigits - 1]--;
		rem += kappa;
	}
}

typedef struct s_digits {
	uint64_t	wfrac;
	uint64_t	delta;
	Fp			one;
	uint64_t	part1;
	uint64_t	part2;
	int			idx;
	int kappa;
	uint64_t	*divp;
	uint64_t	div;
	unsigned	digit;
	uint64_t	tmp;
	uint64_t	*unit;
} t_digits;

static int	generate_digits(Fp *fp, Fp *upper, Fp *lower, char *digits, int *K)
{
	t_digits d;

	d.idx = 0;
	d.wfrac = upper->frac - fp->frac;
	d.delta = upper->frac - lower->frac;
	d.one.frac = 1ULL << -upper->exp;
	d.one.exp = upper->exp;
	d.part1 = upper->frac >> -d.one.exp;
	d.part2 = upper->frac & (d.one.frac - 1);
	d.idx = 0, d.kappa = 10;
	for (d.divp = g_tens + 10; d.kappa > 0; d.divp++)
	{
		d.div = *d.divp;
		d.digit = d.part1 / d.div;
		if (d.digit || d.idx)
		{
			digits[d.idx++] = d.digit + '0';
		}
		d.part1 -= d.digit * d.div;
		d.kappa--;
		d.tmp = (d.part1 << -d.one.exp) + d.part2;
		if (d.tmp <= d.delta)
		{
			*K += d.kappa;
			round_digit(digits, d.idx, d.delta, d.tmp, d.div << -d.one.exp, d.wfrac);
			return (d.idx);
		}
	}
	d.unit = g_tens + 18;
	while (true)
	{
		d.part2 *= 10;
		d.delta *= 10;
		d.kappa--;
		d.digit = d.part2 >> -d.one.exp;
		if (d.digit || d.idx)
			digits[d.idx++] = d.digit + '0';
		d.part2 &= d.one.frac - 1;
		if (d.part2 < d.delta)
		{
			*K += d.kappa;
			round_digit(digits, d.idx, d.delta, d.part2, d.one.frac, d.wfrac * *d.unit);
			return (d.idx);
		}
		d.unit--;
	}
}

static int	grisu2(double d, char *digits, int *K)
{
	Fp	w;
	int	k;
	Fp	cp;

	w = build_fp(d);
	Fp lower, upper;
	get_normalized_boundaries(&w, &lower, &upper);
	_normalize(&w);
	cp = find_cachedpow10(upper.exp, &k);
	w = multiply(&w, &cp);
	upper = multiply(&upper, &cp);
	lower = multiply(&lower, &cp);
	lower.frac++;
	upper.frac--;
	*K = -k;
	return (generate_digits(&w, &upper, &lower, digits, K));
}

static int	emit_digits(char *digits, int ndigits, char *dest, int K, bool neg)
{
	int		exp;
	int		max_trailing_zeros;
	int		offset;
	int		idx;
	char	sign;
	int		cent;
	int		dec;

	exp = absv(K + ndigits - 1);
	max_trailing_zeros = 7;
	if (neg)
	{
		max_trailing_zeros -= 1;
	}
	if (K >= 0 && (exp < (ndigits + max_trailing_zeros)))
	{
		ft_memcpy(dest, digits, ndigits);
		ft_memset(dest + ndigits, '0', K);
		return (ndigits + K);
	}
	if (K < 0 && (K > -7 || exp < 4))
	{
		offset = ndigits - absv(K);
		if (offset <= 0)
		{
			offset = -offset;
			dest[0] = '0';
			dest[1] = '.';
			ft_memset(dest + 2, '0', offset);
			ft_memcpy(dest + offset + 2, digits, ndigits);
			return (ndigits + 2 + offset);
		}
		else
		{
			ft_memcpy(dest, digits, offset);
			dest[offset] = '.';
			ft_memcpy(dest + offset + 1, digits + offset, ndigits - offset);
			return (ndigits + 1);
		}
	}
	ndigits = min(ndigits, 18 - neg);
	idx = 0;
	dest[idx++] = digits[0];
	if (ndigits > 1)
	{
		dest[idx++] = '.';
		ft_memcpy(dest + idx, digits + 1, ndigits - 1);
		idx += ndigits - 1;
	}
	dest[idx++] = 'e';
	if (K + ndigits - 1 < 0)
		sign = '-';
	else
		sign = '+';
	dest[idx++] = sign;
	cent = 0;
	if (exp > 99)
	{
		cent = exp / 100;
		dest[idx++] = cent + '0';
		exp -= cent * 100;
	}
	if (exp > 9)
	{
		dec = exp / 10;
		dest[idx++] = dec + '0';
		exp -= dec * 10;
	}
	else if (cent)
	{
		dest[idx++] = '0';
	}
	dest[idx++] = exp % 10 + '0';
	return (idx);
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
	{
		return (0);
	}
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
	char	digits[18];
	int		str_len;
	bool	neg;
	int		spec;
	int		K;
	int		ndigits;

	str_len = 0;
	neg = false;
	if (get_dbits(d) & SIGNMASK)
	{
		dest[0] = '-';
		str_len++;
		neg = true;
	}
	spec = filter_special(d, dest + str_len);
	if (spec)
	{
		return (str_len + spec);
	}
	K = 0;
	ndigits = grisu2(d, digits, &K);
	str_len += emit_digits(digits, ndigits, dest + str_len, K, neg);
	return (str_len);
}
