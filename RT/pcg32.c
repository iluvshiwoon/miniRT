/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pcg32.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 17:28:39 by kgriset           #+#    #+#             */
/*   Updated: 2025/07/18 17:59:53 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"

uint32_t	pcg_rotr_32(uint32_t value, unsigned int rot)
{
	return ((value >> rot) | (value << ((-rot) & 31)));
}

uint32_t	pcg_output_xsh_rr_64_32(uint64_t state)
{
	return (pcg_rotr_32(((state >> 18u) ^ state) >> 27u, state >> 59u));
}

uint32_t	pcg_setseq_64_xsh_rr_32_random_r(struct pcg_state_setseq_64 *rng)
{
	uint64_t	oldstate;

	oldstate = rng->state;
	pcg_setseq_64_step_r(rng);
	return (pcg_output_xsh_rr_64_32(oldstate));
}

uint64_t	pcg_advance_lcg_64(uint64_t state, uint64_t delta,
		uint64_t cur_mult, uint64_t cur_plus)
{
	uint64_t	acc_mult;
	uint64_t	acc_plus;

	acc_mult = 1u;
	acc_plus = 0u;
	while (delta > 0)
	{
		if (delta & 1)
		{
			acc_mult *= cur_mult;
			acc_plus = acc_plus * cur_mult + cur_plus;
		}
		cur_plus = (cur_mult + 1) * cur_plus;
		cur_mult *= cur_mult;
		delta /= 2;
	}
	return (acc_mult * state + acc_plus);
}

void	pcg_setseq_64_advance_r(struct pcg_state_setseq_64 *rng, uint64_t delta)
{
	rng->state = pcg_advance_lcg_64(rng->state, delta,
			PCG_DEFAULT_MULTIPLIER_64, rng->inc);
}
