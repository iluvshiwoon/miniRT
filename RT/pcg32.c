#include "../miniRT.h"
// #include <stddef.h>
// #include <stdlib.h>
// #include <stdint.h>
// #include <stdbool.h>
// #include <time.h>
//
// #include "pcg_variants.h"
// #include "pcg_spinlock.h"
//
// #include "entropy.h"
//
// #ifndef IS_UNIX
// #if !defined(_WIN32) && (defined(__unix__)  || defined(__unix) \
//                          || (defined(__APPLE__) && defined(__MACH__)))
//     #define IS_UNIX 1
// #else
//     #define IS_UNIX 0
// #endif
// #endif
//
// /* If HAVE_DEV_RANDOM is set, we use that value, otherwise we guess */
// #ifndef HAVE_DEV_RANDOM
// #define HAVE_DEV_RANDOM         IS_UNIX
// #endif
//
// #if HAVE_DEV_RANDOM
//     #include <fcntl.h>
//     #include <unistd.h>
// #endif
//
// #if HAVE_DEV_RANDOM 
/* entropy_getbytes(dest, size):
 *     Use /dev/urandom to get some external entropy for seeding purposes.
 *
 * Note:
 *     If reading /dev/urandom fails (which ought to never happen), it returns
 *     false, otherwise it returns true.  If it fails, you could instead call
 *     fallback_entropy_getbytes which always succeeds.
 */

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

// inline uint32_t pcg_rotr_32(uint32_t value, unsigned int rot)
// {
//
// #if PCG_USE_INLINE_ASM && __clang__ && (__x86_64__  || __i386__)
//     asm ("rorl   %%cl, %0" : "=r" (value) : "0" (value), "c" (rot));
//     return value;
// #else
//     return (value >> rot) | (value << ((- rot) & 31));
// #endif
// }

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


// #else
// bool entropy_getbytes(void* dest, size_t size)
// {
//     fallback_entropy_getbytes(dest, size);
//     return true;
// }
// #endif

/* fallback_entropy_getbytes(dest, size):
 *     Works like the /dev/random version above, but avoids using /dev/random.
 *     Instead, it uses a private RNG (so that repeated calls will return
 *     different seeds).  Makes no attempt at cryptographic security.
 */
//
// void fallback_entropy_getbytes(void* dest, size_t size)
// {
//     /* Most modern OSs use address-space randomization, meaning that we can
//        use the address of stack variables and system library code as
//        initializers.  It's not as good as using /dev/random, but probably
//        better than using the current time alone. */
//
//     static PCG_SPINLOCK_DECLARE(mutex);
//     PCG_SPINLOCK_LOCK(mutex);
//
//     static int intitialized = 0;
//     static pcg32_random_t entropy_rng;
//     
//     if (!intitialized) {
//         int dummyvar;
//         pcg32_srandom_r(&entropy_rng,
//                         time(NULL) ^ (intptr_t)&fallback_entropy_getbytes, 
//                         (intptr_t)&dummyvar);
//         intitialized = 1;
//     }
//     
//     char* dest_cp = (char*) dest;
//     for (size_t i = 0; i < size; ++i) {
//         dest_cp[i] = (char) pcg32_random_r(&entropy_rng);
//     }
//
//     PCG_SPINLOCK_UNLOCK(mutex);
// }
