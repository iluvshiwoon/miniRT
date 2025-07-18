/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pcg32.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 17:28:39 by kgriset           #+#    #+#             */
/*   Updated: 2025/07/18 17:28:47 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"

uint32_t pcg_setseq_64_xsh_rr_32_boundedrand_r(struct pcg_state_setseq_64* rng,
                                      uint32_t bound)
{
    uint32_t threshold = -bound % bound;
    for (;;) {
        uint32_t r = pcg_setseq_64_xsh_rr_32_random_r(rng);
        if (r >= threshold)
            return r % bound;
    }
}

double double_rng(t_pcg32_random * rng)
{
    return(ldexp(pcg32_random_r(rng),-32));
}

bool entropy_getbytes(void* dest, size_t size)
{
    int fd = open("/dev/urandom", O_RDONLY);
    if (fd < 0)
        return false;
    ssize_t sz = read(fd, dest, size);
    return (close(fd) == 0) && (sz == (ssize_t) size);
}


void pcg_setseq_64_step_r(struct pcg_state_setseq_64* rng)
{
    rng->state = rng->state * PCG_DEFAULT_MULTIPLIER_64 + rng->inc;
}

void pcg_setseq_64_srandom_r(struct pcg_state_setseq_64* rng, uint64_t initstate, uint64_t initseq)
{
    rng->state = 0U;
    rng->inc = (initseq << 1u) | 1u;
    pcg_setseq_64_step_r(rng);
    rng->state += initstate;
    pcg_setseq_64_step_r(rng);
}

uint32_t pcg_rotr_32(uint32_t value, unsigned int rot)
{
    return (value >> rot) | (value << ((- rot) & 31));
}

uint32_t pcg_output_xsh_rr_64_32(uint64_t state)
{
    return pcg_rotr_32(((state >> 18u) ^ state) >> 27u, state >> 59u);
}

uint32_t pcg_setseq_64_xsh_rr_32_random_r(struct pcg_state_setseq_64* rng)
{
    uint64_t oldstate = rng->state;
    pcg_setseq_64_step_r(rng);
    return pcg_output_xsh_rr_64_32(oldstate);
}

uint64_t pcg_advance_lcg_64(uint64_t state, uint64_t delta, uint64_t cur_mult,
                            uint64_t cur_plus)
{
    uint64_t acc_mult = 1u;
    uint64_t acc_plus = 0u;
    while (delta > 0) {
        if (delta & 1) {
            acc_mult *= cur_mult;
            acc_plus = acc_plus * cur_mult + cur_plus;
        }
        cur_plus = (cur_mult + 1) * cur_plus;
        cur_mult *= cur_mult;
        delta /= 2;
    }
    return acc_mult * state + acc_plus;
}

void pcg_setseq_64_advance_r(struct pcg_state_setseq_64* rng, uint64_t delta)
{
    rng->state = pcg_advance_lcg_64(rng->state, delta,
                                    PCG_DEFAULT_MULTIPLIER_64, rng->inc);
}
