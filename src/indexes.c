/**
 * @file indexes.c
 * @brief Indexes generation routines
 * 
 * This implements the APIs for random index generation.
 */

#ifdef _DEBUG_
#include <stdio.h>
#endif

#include <stdlib.h>
#include <string.h>

#ifdef _SPEED_
#include "../include/tictoc.h"
#else
#include <time.h>
#endif

#include "../include/bits.h"
#include "../include/indexes.h"

double prng_rand(unsigned long *seed, size_t size, unsigned *indexes, unsigned lowerbound, unsigned upperbound, char replacement)
{
    unsigned int index, i;
    unsigned long _seed;

#ifdef _SPEED_
    struct timespec start, end;
#endif
#ifdef _DEBUG_
    if (size < 1)
    {
        printf("error: size < 1\n");

        return -1;
    }

    if (upperbound <= lowerbound)
    {
        printf("error: upperbound <= lowerbound\n");
        // upperbound = lowerbound + size + 1;

        return -1;
    }

    if (!replacement && upperbound - lowerbound < size)
    {
        printf("error: upperbound - lowerbound < size (%u - %u < %zu)\n", upperbound, lowerbound, size);
        // size = upperbound - lowerbound;

        return -1;
    }

    if (!indexes)
    {
        printf("error: indexes is NULL\n");
        // size = upperbound - lowerbound;

        return -1;
    }
#endif

    unsigned char arr[upperbound - lowerbound + 1];
    if (!replacement)
    {
        memset(arr, 0, upperbound - lowerbound + 1);
    }

#ifdef _SPEED_
    TIC(start);
#endif

    /* retrieve or generate or assign PRNG seed */
    if (seed && *seed)
    {
        _seed = *seed;
    }
    else
    {
        _seed = (unsigned long)time(NULL);
        if (seed)
        {
            *seed = _seed;
        }
    }
    srand(_seed);

    /* generate indexes */
    i = 0;
    while (i < size)
    {
        index = (rand() + lowerbound) % upperbound;
        if (!replacement)
        {
            while (char_check_bit(arr, index))
            {
                index = (index + 1) % upperbound;
            }
            char_set_bit(arr, index);
        }
        indexes[i++] = index;
    }

#ifdef _SPEED_
    TOC(end);
    return TIC_TOC(start, end);
#else
    return 0;
#endif
}

double prng_rand_with_replacement(unsigned long *seed, size_t size, unsigned *indexes, unsigned lowerbound, unsigned upperbound)
{
    return prng_rand(seed, size, indexes, lowerbound, upperbound, 1);
}

double prng_rand_without_replacement(unsigned long *seed, size_t size, unsigned *indexes, unsigned lowerbound, unsigned upperbound)
{
    return prng_rand(seed, size, indexes, lowerbound, upperbound, 0);
}