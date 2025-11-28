#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "../include/bits.h"
#include "../include/indexes.h"
#include "../include/xlock.h"

#define HASH_KEY_BYTES 32
#define TOKEN_BYTES 32

int main()
{
    /* init PRNG */
    srand(time(NULL));

    /* constants */
    unsigned int experiments = 10000;
    unsigned int source_bytes = 8004;
    unsigned int pool_bytes = 32;

    unsigned int key_pre_bits = 80;
    float e_abs = 0.15;
    unsigned int n_xoration = 2;
    unsigned int n_locks = 64;

    /* derived variables */
    unsigned int source_bits = bytes_to_bits(source_bytes);
    unsigned int pool_bits = bytes_to_bits(pool_bytes);
    unsigned int vault_bits = pool_bits * n_locks;
    unsigned int vault_bytes = bits_to_bytes(vault_bits);
    unsigned int key_bytes = HASH_KEY_BYTES;
    unsigned int key_bits = bytes_to_bits(key_bytes);

    /* define */
    unsigned char source[source_bytes];
    unsigned char read[source_bytes];
    unsigned char pool[pool_bytes];
    unsigned char vault[vault_bytes];

    unsigned char key1[key_bytes];
    unsigned char key2[key_bytes];

    unsigned char token[TOKEN_BYTES];

    double res = 0;
#ifdef _SPEED_
    /* prepare measurements */
    double time_gen = 0, time_rep = 0;
    double mean_gen = 0, mean_rep = 0;
#else
    /* for compiler's happiness */
    res = res * 2;
#endif

    unsigned int errors_count = 0;

    unsigned long key1_seed, key2_seed, source_seed, nonce;

    /* seed lookup */
    unsigned int look_up_size = 1024;
    unsigned int key_seeds[look_up_size], source_seeds[look_up_size];
    prng_rand_without_replacement(&key1_seed, look_up_size, key_seeds, 1, pow(2, 20));
    prng_rand_without_replacement(&source_seed, look_up_size, source_seeds, 1, pow(2, 20));

    printf("\n-----------params-----------\nkey_pre\t\t: %u\ne_abs\t\t: %f\nC\t\t: %u\nL\t\t: %d\n-----------params-----------\n", key_pre_bits, e_abs, n_xoration, n_locks);

    unsigned int i, j;
    for (i = 0; i < experiments; i++)
    {
        if (!(i % 1000))
        {
            printf("\riteration %d", i);
            fflush(stdout);
        }

        key1_seed = key_seeds[i % look_up_size];
        if (key1_seed == 0)
            key1_seed++;
        // key2_seed = 0;
        source_seed = source_seeds[i % look_up_size];
        if (source_seed == 0)
            source_seed++;

        init(
            source, &source_seed, source_bits, source_bytes,
            pool, pool_bits, pool_bytes,
            vault, n_locks, n_xoration);

        change_random(source, read, source_bytes, e_abs);

        res = gen(
            read, &source_seed, source_bits, vault,
            key1, &key1_seed, key_bits, key_pre_bits,
            &nonce, token, TOKEN_BYTES,
            pool_bits, n_locks, n_xoration);
#ifdef _SPEED_
        time_gen += res;
#endif

#ifdef _VERBOSE_
        printf("key gen (%u bytes)\t\t\t: ", key_bytes);
        for (int i = 0; i < key_bytes; i++)
        {
            printf("%x", key1[i]);
        }
        printf("\n");
#endif

        change_random(source, read, source_bytes, e_abs);
        key2_seed = key1_seed;

        res = rep(
            read, &source_seed, source_bits, vault,
            key2, &key2_seed, key_bits, key_pre_bits,
            &nonce, token, TOKEN_BYTES,
            pool_bits, n_locks, n_xoration);
#ifdef _SPEED_
        time_rep += res;
#endif
#ifdef _VERBOSE_
        printf("key rep (%u bytes)\t\t\t: ", key_bytes);
        for (int i = 0; i < key_bytes; i++)
        {
            printf("%x", key2[i]);
        }
        printf("\n");
#endif

        for (j = 0; j < key_bytes; j++)
        {
            if (get_bit(key1, j) != get_bit(key2, j))
            {
                errors_count += 1;
                break;
            }
        }
    }

#ifdef _SPEED_
    mean_gen = (double)(time_gen / (double)experiments);
    mean_rep = (double)(time_rep / (double)experiments);
#endif

    printf("\nExperiments\t: %u\n", experiments);
    printf("Errors\t\t: %u (%.3f%%)\n", errors_count, (double)errors_count / (double)experiments);

#ifdef _SPEED_
    printf("Mean Gen ms\t: %f\n", mean_gen);
    printf("Mean Rep ms\t: %f\n", mean_rep);
    printf("Mean Tot ms\t: %f\n", mean_gen + mean_rep);
#endif

    return 0;
}