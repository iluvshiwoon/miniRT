/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pcg32_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 17:53:06 by kgriset           #+#    #+#             */
/*   Updated: 2025/07/18 17:59:36 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"

uint32_t	pcg_setseq_64_xsh_rr_32_boundedrand_r(\
		struct pcg_state_setseq_64 *rng, uint32_t bound)
{
	uint32_t	threshold;
	uint32_t	r;

	threshold = -bound % bound;
	while (true)
	{
		r = pcg_setseq_64_xsh_rr_32_random_r(rng);
		if (r >= threshold)
			return (r % bound);
	}
}

double	double_rng(t_pcg32_random *rng)
{
	return (ldexp(pcg32_random_r(rng), -32));
}

bool	entropy_getbytes(void *dest, size_t size)
{
	int		fd;
	ssize_t	sz;

	fd = open("/dev/urandom", O_RDONLY);
	if (fd < 0)
		return (false);
	sz = read(fd, dest, size);
	return ((close(fd) == 0) && (sz == (ssize_t)size));
}

void	pcg_setseq_64_step_r(struct pcg_state_setseq_64 *rng)
{
	rng->state = rng->state * PCG_DEFAULT_MULTIPLIER_64 + rng->inc;
}

void	pcg_setseq_64_srandom_r(struct pcg_state_setseq_64 *rng,
		uint64_t initstate, uint64_t initseq)
{
	rng->state = 0U;
	rng->inc = (initseq << 1u) | 1u;
	pcg_setseq_64_step_r(rng);
	rng->state += initstate;
	pcg_setseq_64_step_r(rng);
}
