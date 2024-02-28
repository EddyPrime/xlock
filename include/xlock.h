#ifndef XLOCK_H
#define XLOCK_H

/**
 * @file xlock.h
 * @brief Implementation of X-Lock
 *
 * This file exposes APIs of X-Lock, a secure xor-bsed fuzzy extractor for
 * resource constrained devices.
 */

/**
 * @brief retrieves the value of a bit in a 1-D array

 * This function retrieves the value of the bit in position i
 * in a 1-D array of bits.
 * 
 * @param b array of bits
 * @param i bit position
 * @return the value of ith bit in b
 */
unsigned char get_bit(unsigned char *b, int i);

/**
 * @brief randomly initializes b
 * 
 * This function randomly initializes an array of bits containing size bytes.
 * 
 * @param b array of bits
 * @param size size of b in bytes
 * @return void
 */
void init_random(unsigned char *b, int size);

/**
 * @brief randomly changes b
 * 
 * This function randomly changes b by modifying some of it bytes. e_abs
 * speicifies the probability that a bit is flipped.
 * 
 * @param b input array of bits
 * @param out output array of bits
 * @param size size of b and out in bytes
 * @param e_abs absolute error probability
 * @return void
 */
void change_random(
    unsigned char *b,
    unsigned char *out,
    int size,
    float e_abs);

/**
 * @brief creates the vault needed for the fuzzy extractor
 *
 * This function encrypts pool by means of subsets of bits from
 * source according to source_indexes. The result is stored in
 * vault.
 *
 * @param source preferred state of source
 * @param source_indexes source indexes to unlock vault
 * @param pool random pool
 * @param pool_bits pool length in bits
 * @param n_locks number of locks per bit-locker
 * @param n_xoration number of bits per XOR-ation
 * @param vault encrypted vault
 * @return void
 */
void lock(
    unsigned char *source,
    unsigned int *source_indexes,
    unsigned char *pool,
    unsigned int pool_bits,
    unsigned int n_locks,
    unsigned int n_xoration,
    unsigned char *vault);

/**
 * @brief unlocks the vault and retrieves key_pre
 *
 * This function decrypts pool by means of subsets of bits from
 * source according to source_indexes. The result is not stored.
 * After the decryption, the function builds key_pre according
 * to key_indexes.
 * 
 * @param source reference source
 * @param source_indexes source indexes to unlock vault
 * @param vault reference vault
 * @param key reference key
 * @param key_indexes vault indexes to form the key
 * @param key_bits key length in bits
 * @param n_locks number of locks per bit-locker
 * @param n_xoration number of bits per XOR-ation
 * @return void
 */
void unlock(
    unsigned char *source,
    unsigned int *source_indexes,
    unsigned char *vault,
    unsigned char *key,
    unsigned int *key_indexes,
    unsigned int key_bits,
    unsigned int n_locks,
    unsigned int n_xoration);

/**
 * @brief initializes source, pool and vault
 * 
 * This function randomly initializes the source state and
 * the pool that will be encrypted in the vault. Moreover,
 * the function provides the indexes to build the vault and
 * initializes the vault iteself.
 *
 * @param source preferred source state
 * @param source_seed source seed for indexes to unlock vault
 * @param source_bits source length in bits
 * @param source_bytes source length in bytes
 * @param pool random pool
 * @param pool_bits pool length in bits
 * @param pool_bytes pool length in bytes
 * @param vault encrypted vault
 * @param n_locks number of locks per bit-locker
 * @param n_xoration number of bits per XOR-ation
 * @return void
 */
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
    unsigned int n_xoration);

/**
 * @brief gen procedure of the fuzzy extractor
 * 
 * The function generates the final key by decrypting the vault
 * and retrieving key_pre. The function also produces the indexes
 * for key_pre, the nonce for the final key and the robustness
 * token. 
 *
 * @param read reading from source
 * @param source_seed source seed for indexes to unlock vault
 * @param source_bits source length in bits
 * @param vault encrypted vault
 * @param key key storage
 * @param key_seed key seed for indexes that form the key
 * @param key_bits key length in bits
 * @param key_pre_bits key_pre length in bits
 * @param nonce nonce for final key generation
 * @param token robustness token
 * @param token_bytes robustness token length in bytes
 * @param pool_bits pool length in bits
 * @param n_locks number of locks per bit-locker
 * @param n_xoration number of bits per XOR-ation
 * @return either 0 or the time in milliseconds
 * @note if seeds are not specified or are 0, the function
 * initializes them
 */
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
    unsigned int n_xoration);

/**
 * @brief rep procedure of the fuzzy extractor
 * 
 * The function reproduces the final key by decrypting the vault
 * and retrieving key_pre. Seeds should match those produced by
 * or provided to the generation procedure. The function verifies
 * whether rhe reproduction was successful thanks to the
 * robustness token. If this is the case, key wil contain the key.
 * Otherwise, it will be nullified.
 *
 * @param read reading from source
 * @param source_seed source seed for indexes to unlock vault
 * @param source_bits source length in bits
 * @param vault encrypted vault
 * @param key key storage
 * @param key_seed key seed for indexes that form the key
 * @param key_bits key length in bits
 * @param key_pre_bits key_pre length in bits
 * @param nonce nonce for final key generation
 * @param token robustness token
 * @param token_bytes robustness token length in bytes
 * @param pool_bits pool length in bits
 * @param n_locks number of locks per bit-locker
 * @param n_xoration number of bits per XOR-ation
 * @return either 0 or the time in milliseconds
 */
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
    unsigned int n_xoration);

#endif