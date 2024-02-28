/**
 * @file xlock.c
 * @brief Implementation of X-Lock
 *
 * This file implements the APIs of X-Lock, a secure xor-bsed fuzzy extractor for
 * resource constrained devices.
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <math.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <openssl/sha.h>

#include "../include/bits.h"
#include "../include/tictoc.h"
#include "../include/indexes.h"
#include "../include/xlock.h"

unsigned char get_bit(unsigned char *b, int i)
{
    return (unsigned char)((b[i / 8] >> i % 8) & 1);
}

/**
 * @brief retrieves the value of a bit in a 2-D array

 * This function retrieves the value of the bit in position ij
 * in a 2-D array of bits.
 *
 * @param b array of bits
 * @param i bit position in first dimension
 * @param j bit position in second dimension
 * @param jj size of second dimension
 * @return the value of bit ij in b
 */
unsigned char get_bit_2D(
    unsigned char *b,
    int i,
    int j,
    int jj)
{
    return get_bit(b, i * jj + j);
}

/**
 * @brief retrieves the value of a bit in a 3-D array

 * This function retrieves the value of the bit in position ijk
 * in a 3-D array of bits.
 *
 * @param b array of bits
 * @param i bit position in first dimension
 * @param j bit position in second dimension
 * @param k bit position in third dimension
 * @param di delta between first dimension elements
 * @param kk size of third dimension
 * @return the value of bit ijk in b
 */
unsigned char get_bit_3D(
    unsigned char *b,
    int i,
    int j,
    int k,
    int di,
    int kk)
{
    return get_bit(b, i * di + j * kk + k);
}

/**
 * @brief retrieves the value of a number in a 3-D array

 * This function retrieves the value of the number in position ijk
 * in a 3-D array of unsigned int.
 *
 * @param b array of unisgned int
 * @param i position in first dimension
 * @param j position in second dimension
 * @param k position in third dimension
 * @param di delta between first dimension elements
 * @param kk size of third dimension
 * @return the value of number ijk in b
 */
unsigned int get_int_3D(
    unsigned int *b,
    int i,
    int j,
    int k,
    int di,
    int kk)
{
    return b[i * di + j * kk + k];
}

/**
 * @brief sets the value of a bit in a 1-D array

 * This function sets the value of the bit in position i
 * in a 1-D array of bits.
 *
 * @param b array of bits
 * @param i bit position
 * @param v bit value
 * @return void
 */
void set_bit_v(
    unsigned char *b,
    int i,
    unsigned char v)
{
    unsigned int t0 = i / 8;
    unsigned char t1 = b[t0];
    t1 ^= (-v ^ b[t0]) & (1UL << (i % 8));
    b[t0] = t1;
}

/**
 * @brief sets the value of a bit in a 2-D array

 * This function sets the value of the bit in position ij
 * in a 2-D array of bits.
 *
 * @param b array of bits
 * @param i bit position in first dimension
 * @param j bit position in second dimension
 * @param jj size of second dimension
 * @param v bit value
 * @return void
 */
void set_bit_v_2D(
    unsigned char *b,
    int i,
    int j,
    int jj,
    unsigned char v)
{
    set_bit_v(b, i * jj + j, v);
}

/**
 * @brief sets the value of a bit in a 3-D array

 * This function sets the value of the bit in position ijk
 * in a 3-D array of bits.
 *
 * @param b array of bits
 * @param i bit position in first dimension
 * @param j bit position in second dimension
 * @param k bit position in third dimension
 * @param di delta between first dimension elements
 * @param kk size of third dimension
 * @param v bit value
 * @return void
 */
void set_bit_v_3D(
    unsigned char *b,
    int i,
    int j,
    int k,
    int di,
    int kk,
    unsigned char v)
{
    set_bit_v(b, i * di + j * kk + k, v);
}

/**
 * @brief sets the value of a number in a 3-D array

 * This function sets the value of the number in position ijk
 * in a 3-D array of unsigned int.
 *
 * @param b array of unisgned int
 * @param i position in first dimension
 * @param j position in second dimension
 * @param k position in third dimension
 * @param di delta between first dimension elements
 * @param v unsigned int value
 * @return void
 */
void set_int_3D(
    unsigned int *b,
    int i,
    int j,
    int k,
    int di,
    int kk,
    unsigned int v)
{
    b[i * di + j * kk + k] = v;
}

/**
 * @brief Generates a random value bounded by repr.bits
 *
 * @param bits maximum size bits
 * @return random value bounded by repr.bits
 * @note maximum 32 bits
 */
unsigned int get_random_bounded(int bits)
{
    unsigned int out = ((rand() & 0xff) |
                        ((rand() & 0xff) << 8) |
                        ((rand() & 0xff) << 16) |
                        ((rand() & 0xff) << 24));
    return out >> (32 - bits);
}

void init_random(unsigned char *b, int size)
{
    int i;
    for (i = 0; i < size; i++)
        b[i] = rand();
}

void change_random(
    unsigned char *b,
    unsigned char *out,
    int size,
    float e_abs)
{

    unsigned char t;
    unsigned char thres = 256 * e_abs;
    int i, j;
    for (i = 0; i < size; i++)
    {
        t = 0;
        for (j = 0; j < 8; j++)
        {
            if (((unsigned char)rand()) < thres)
                t = t | (1 << j);
        }
        out[i] = b[i] ^ t;
    }
}

/**
 * @brief Counts the number of bits set
 *
 * @param b array of bits
 * @param size size of b in bytes
 * @return number of bits set
 */
int count_ones(unsigned char *b, int size)
{
    unsigned char t;
    int i, j, out = 0;
    for (i = 0; i < size; i++)
    {
        t = b[i];
        for (j = 0; j < 8; j++)
            out += ((t >> i) & 1);
    }
    return out;
}

/**
 * @brief Prints an array of bits
 *
 * @param b array of bits
 * @param size size of b in bytes
 * @return void
 */
void printb(unsigned char *b, int size)
{
    int i, j;
    for (i = size - 1; i >= 0; i--)
    {
        for (j = 7; j >= 0; j--)
        {
            printf("%u", (b[i] >> j) & 1);
        }
    }
}

void lock(
    unsigned char *source,
    unsigned int *source_indexes,
    unsigned char *pool,
    unsigned int pool_bits,
    unsigned int n_locks,
    unsigned int n_xoration,
    unsigned char *vault)
{
    unsigned char b, t0;
    unsigned int i, j, k, ijk = 0;
    for (i = 0; i < pool_bits; i++)
    {
        b = get_bit(pool, i);
        for (j = 0; j < n_locks; j++)
        {
            t0 = b;
            for (k = 0; k < n_xoration; k++)
            {
                t0 ^= get_bit(source, source_indexes[ijk++]);
            }
            set_bit_v_2D(vault, i, j, n_locks, t0);
        }
    }
}

void unlock(
    unsigned char *source,
    unsigned int *source_indexes,
    unsigned char *vault,
    unsigned char *key,
    unsigned int *key_indexes,
    unsigned int key_bits,
    unsigned int n_locks,
    unsigned int n_xoration)
{
    unsigned char b;
    unsigned int i0, i1;
    unsigned int i, j, k;
    int c;
    int mid = n_locks / 2;
    int di = n_locks * n_xoration;

    for (i = 0; i < key_bits; i++)
    {
        i0 = key_indexes[i];
        c = 0;
        for (j = 0; j < n_locks; j++)
        {
            b = get_bit_2D(vault, i0, j, n_locks);
            for (k = 0; k < n_xoration; k++)
            {
                i1 = get_int_3D(source_indexes, i0, j, k, di, n_xoration);
                b ^= get_bit(source, i1);
            }
            if (b)
                c++;
        }
        if (c > mid)
            set_bit_v(key, i, 1);
        else
            set_bit_v(key, i, 0);
    }
}

void init(
    unsigned char *source,
    unsigned long *source_seed,
    unsigned int source_bits,
    unsigned int source_bytes,
    unsigned char *pool,
    unsigned int pool_bits,
    unsigned int pool_bytes,
    unsigned char *vault,
    unsigned int n_locks,
    unsigned int n_xoration)
{
    unsigned int source_indexes[pool_bits * n_locks * n_xoration];

    init_random(source, source_bytes);
    init_random(pool, pool_bytes);
    prng_rand_without_replacement(source_seed, pool_bits * n_locks * n_xoration, source_indexes, 0, source_bits);
    lock(source, source_indexes, pool, pool_bits, n_locks, n_xoration, vault);
}

double gen(
    unsigned char *read,
    unsigned long *source_seed,
    unsigned int source_bits,
    unsigned char *vault,
    unsigned char *key,
    unsigned long *key_seed,
    unsigned int key_bits,
    unsigned int key_pre_bits,
    unsigned long *nonce,
    unsigned char *token,
    unsigned int token_bytes,
    unsigned int pool_bits,
    unsigned int n_locks,
    unsigned int n_xoration)
{
    unsigned char key_pre[bits_to_bytes(key_pre_bits)];
    unsigned int source_indexes[pool_bits * n_locks * n_xoration], key_indexes[key_pre_bits], md_len;

#ifdef _SPEED_
    /* start execution time evaluation */
    struct timespec start, end;
    TIC(start);
#endif

    /* generate sets of indexes */
    prng_rand_without_replacement(source_seed, pool_bits * n_locks * n_xoration, source_indexes, 0, source_bits);
    prng_rand_without_replacement(key_seed, key_pre_bits, key_indexes, 0, pool_bits);

    /* generate key_pre */
    unlock(
        read, source_indexes, vault,
        key_pre, key_indexes, key_pre_bits,
        n_locks, n_xoration);

#ifdef _VERBOSE_
    printf("key pre gen (%u bytes)\t\t\t: ", bits_to_bytes(key_pre_bits));
    for (int i = 0; i < bits_to_bytes(key_pre_bits); i++)
    {
        printf("%x", key_pre[i]);
    }
    printf("\n");
#endif

    /* generate nonce for final key */
    srand(time(NULL));
    *nonce = (unsigned long)rand();

    /* key = hash(key_pre, noce) */
    md_len = bits_to_bytes(key_bits);
    HMAC(EVP_sha256(), nonce, sizeof(unsigned long), key_pre, bits_to_bytes(key_pre_bits), key, &md_len);

    /* token = hash(key, key_seed) */
    md_len = token_bytes;
    HMAC(EVP_sha256(), key_seed, sizeof(unsigned long), key, bits_to_bytes(key_bits), token, &md_len);

#ifdef _VERBOSE_
    printf("robustness token (%u bytes)\t\t: ", token_bytes);
    for (int i = 0; i < token_bytes; i++)
    {
        printf("%x", token[i]);
    }
    printf("\n");
#endif

#ifdef _SPEED_
    /* stop execution time evaluation */
    TOC(end);
    return TIC_TOC(start, end);
#else
    return 0;
#endif
}

double rep(
    unsigned char *read,
    unsigned long *source_seed,
    unsigned int source_bits,
    unsigned char *vault,
    unsigned char *key,
    unsigned long *key_seed,
    unsigned int key_bits,
    unsigned int key_pre_bits,
    unsigned long *nonce,
    unsigned char *token,
    unsigned int token_bytes,
    unsigned int pool_bits,
    unsigned int n_locks,
    unsigned int n_xoration)
{
    unsigned char key_pre[bits_to_bytes(key_pre_bits)], T[token_bytes];
    unsigned int i, md_len, source_indexes[pool_bits * n_locks * n_xoration], key_indexes[key_pre_bits];

#ifdef _SPEED_
    /* start execution time evaluation */
    struct timespec start, end;
    TIC(start);
#endif

    /* generate sets of indexes */
    prng_rand_without_replacement(source_seed, pool_bits * n_locks * n_xoration, source_indexes, 0, source_bits);
    prng_rand_without_replacement(key_seed, key_pre_bits, key_indexes, 0, pool_bits);

    /* generate key_pre */
    unlock(
        read, source_indexes, vault,
        key_pre, key_indexes, key_pre_bits,
        n_locks, n_xoration);

#ifdef _VERBOSE_
    printf("key pre rep (%u bytes)\t\t\t: ", bits_to_bytes(key_pre_bits));
    for (i = 0; i < bits_to_bytes(key_pre_bits); i++)
    {
        printf("%x", key_pre[i]);
    }
    printf("\n");
#endif

    /* key = hash(key_pre, noce) */
    md_len = bits_to_bytes(key_bits);
    HMAC(EVP_sha256(), nonce, sizeof(unsigned long), key_pre, bits_to_bytes(key_pre_bits), key, &md_len);

    /* token = hash(key, key_seed) */
    md_len = token_bytes;
    HMAC(EVP_sha256(), key_seed, sizeof(unsigned long), key, bits_to_bytes(key_bits), T, &md_len);

    /* check if T != T' */
    if (strncmp((char *)T, (char *)token, token_bytes))
    {
#ifdef _VERBOSE_
        printf("T != computed T\n");
#endif
        for (i = 0; i < bits_to_bytes(key_bits); i++)
        {
            key[i] = 0;
        }
        key = NULL;
    }

#ifdef _SPEED_
    /* stop execution time evaluation */
    TOC(end);
    return TIC_TOC(start, end);
#else
    return 0;
#endif
}