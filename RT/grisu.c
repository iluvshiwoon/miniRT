/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grisu.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 17:53:36 by kgriset           #+#    #+#             */
/*   Updated: 2025/07/24 15:09:13 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../miniRT.h"

 Fp powers_ten[] = {
    { 18054884314459144840U, -1220 }, { 13451937075301367670U, -1193 },
    { 10022474136428063862U, -1166 }, { 14934650266808366570U, -1140 },
    { 11127181549972568877U, -1113 }, { 16580792590934885855U, -1087 },
    { 12353653155963782858U, -1060 }, { 18408377700990114895U, -1034 },
    { 13715310171984221708U, -1007 }, { 10218702384817765436U, -980 },
    { 15227053142812498563U, -954 },  { 11345038669416679861U, -927 },
    { 16905424996341287883U, -901 },  { 12595523146049147757U, -874 },
    { 9384396036005875287U,  -847 },  { 13983839803942852151U, -821 },
    { 10418772551374772303U, -794 },  { 15525180923007089351U, -768 },
    { 11567161174868858868U, -741 },  { 17236413322193710309U, -715 },
    { 12842128665889583758U, -688 },  { 9568131466127621947U,  -661 },
    { 14257626930069360058U, -635 },  { 10622759856335341974U, -608 },
    { 15829145694278690180U, -582 },  { 11793632577567316726U, -555 },
    { 17573882009934360870U, -529 },  { 13093562431584567480U, -502 },
    { 9755464219737475723U,  -475 },  { 14536774485912137811U, -449 },
    { 10830740992659433045U, -422 },  { 16139061738043178685U, -396 },
    { 12024538023802026127U, -369 },  { 17917957937422433684U, -343 },
    { 13349918974505688015U, -316 },  { 9946464728195732843U,  -289 },
    { 14821387422376473014U, -263 },  { 11042794154864902060U, -236 },
    { 16455045573212060422U, -210 },  { 12259964326927110867U, -183 },
    { 18268770466636286478U, -157 },  { 13611294676837538539U, -130 },
    { 10141204801825835212U, -103 },  { 15111572745182864684U, -77 },
    { 11258999068426240000U, -50 },   { 16777216000000000000U, -24 },
    { 12500000000000000000U,   3 },   { 9313225746154785156U,   30 },
    { 13877787807814456755U,  56 },   { 10339757656912845936U,  83 },
    { 15407439555097886824U, 109 },   { 11479437019748901445U, 136 },
    { 17105694144590052135U, 162 },   { 12744735289059618216U, 189 },
    { 9495567745759798747U,  216 },   { 14149498560666738074U, 242 },
    { 10542197943230523224U, 269 },   { 15709099088952724970U, 295 },
    { 11704190886730495818U, 322 },   { 17440603504673385349U, 348 },
    { 12994262207056124023U, 375 },   { 9681479787123295682U,  402 },
    { 14426529090290212157U, 428 },   { 10748601772107342003U, 455 },
    { 16016664761464807395U, 481 },   { 11933345169920330789U, 508 },
    { 17782069995880619868U, 534 },   { 13248674568444952270U, 561 },
    { 9871031767461413346U,  588 },   { 14708983551653345445U, 614 },
    { 10959046745042015199U, 641 },   { 16330252207878254650U, 667 },
    { 12166986024289022870U, 694 },   { 18130221999122236476U, 720 },
    { 13508068024458167312U, 747 },   { 10064294952495520794U, 774 },
    { 14996968138956309548U, 800 },   { 11173611982879273257U, 827 },
    { 16649979327439178909U, 853 },   { 12405201291620119593U, 880 },
    { 9242595204427927429U,  907 },   { 13772540099066387757U, 933 },
    { 10261342003245940623U, 960 },   { 15290591125556738113U, 986 },
    { 11392378155556871081U, 1013 },  { 16975966327722178521U, 1039 },
    { 12648080533535911531U, 1066 }
};

void	get_normalized_boundaries(Fp *fp, Fp *lower, Fp *upper)
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

Fp	multiply(Fp *a, Fp *b)
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

int	emit_digits(t_fpconv f, char *dest)
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

int	filter_special(double fp, char *dest)
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

Fp find_cachedpow10(int exp, int* k)
{
    const double one_log_ten = 0.30102999566398114;

    int approx = -(exp + npowers) * one_log_ten;
    int idx = (approx - firstpower) / steppowers;

    while(1) {
        int current = exp + powers_ten[idx].exp + 64;

        if(current < expmin) {
            idx++;
            continue;
        }

        if(current > expmax) {
            idx--;
            continue;
        }

        *k = (firstpower + idx * steppowers);

        return powers_ten[idx];
    }
}
