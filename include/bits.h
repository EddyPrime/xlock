#ifndef BITS_H
#define BITS_H

/**
 * @file bits.h
 * @brief Bit manipulations
 * 
 * This file exposes macros for bits manipulation and computation.
 */

/**
 * @brief Returns the smallest integer greater than or equal to a/b
 */
#define CEIL(a, b) (((a) / (b)) + (((a) % (b)) > 0 ? 1 : 0))

/**
 * @brief Returns the bytes corresponding to bits.
 */
#define bits_to_bytes(bits) CEIL(bits, 8)

/**
 * @brief Returns the bits corresponding to bytes.
 */
#define bytes_to_bits(bytes) bytes * 8

/**
 * @brief Returns the value of bit in n.
 */
#define check_bit(n, bit) ((n & 1ULL) >> bit & 1)

/**
 * @brief Returns the value n with bit set to 1.
 */
#define set_bit(n, bit) (n & 1ULL) | (1 << bit)

/**
 * @brief Returns the value n with bit set to 0.
 */
#define clear_bit(n, bit) (n & 1ULL) & ~(1 << bit)

/**
 * @brief Returns the value n with bit flipped.
 */
#define toggle_bit(n, bit) (n & 1ULL) ^ (1 << bit)

/**
 * @brief Returns whether n is a power of 2.
 */
#define is_power_of_two(n) n &(n - 1) == 0 and n != 0

/**
 * @brief Returns the value of bit in arr.
 */
#define char_check_bit(arr, bit) (1ULL & (arr[(bit) / 8] >> (bit) % 8) & 1)

/**
 * @brief Sets to 1 the value of bit in arr.
 */
#define char_set_bit(arr, bit) (arr[(bit) / 8] = set_bit(arr[(bit) / 8], (bit) % 8))

/**
 * @brief Sets to 0 the value of bit in arr.
 */
#define char_clear_bit(arr, bit) (arr[(bit) / 8] = clear_bit(arr[(bit) / 8], (bit) % 8))

/**
 * @brief Flips the value of bit in arr.
 */
#define char_toggle_bit(arr, bit) (arr[(bit) / 8] = toggle_bit(arr[(bit) / 8], (bit) % 8))

#endif
