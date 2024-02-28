#ifndef INDEXES_H
#define INDEXES_H

/**
 * @file indexes.h
 * @brief Indexes generation routines
 * 
 * This file exposes APIs for random index generation.
 */

/**
 * @brief produces a list of PRNG-based random numbers
 * 
 * This function produces size numbers in the range [lowerbound, upperbound)
 * seeding a PRNG with seed. Replacement can be specified.
 * 
 * @param seed seed for the PRNG
 * @param size number of indexes
 * @param indexes pointer to the array of indexes
 * @param lowerbound lowest index value, included
 * @param upperbound highest index value, excluded
 * @param replacement 0 without replacement, otherwise with
 * @return a negative value upon error, either 0 or the time in milliseconds
 * @see PRNG
 * @note if seed is NULL, it will be initialized.
 */
double prng_rand(unsigned long *seed, size_t size, unsigned *indexes, unsigned lowerbound, unsigned upperbound, char replacement);

/**
 * @brief produces a list of PRNG-based random numbers with replacement
 * 
 * This function produces size numbers in the range [lowerbound, upperbound)
 * seeding a PRNG with seed with replacement.
 * 
 * @param seed seed for the PRNG
 * @param size number of indexes
 * @param indexes pointer to the array of indexes
 * @param lowerbound lowest index value, included
 * @param upperbound highest index value, excluded
 * @return a negative value upon error, either 0 or the time in milliseconds
 * @see PRNG
 * @note if seed is NULL, it will be initialized.
 */
double prng_rand_with_replacement(unsigned long *seed, size_t size, unsigned *indexes, unsigned lowerbound, unsigned upperbound);

/**
 * @brief produces a list of PRNG-based random numbers without replacement
 * 
 * This function produces size numbers in the range [lowerbound, upperbound)
 * seeding a PRNG with seed without replacement.
 * 
 * @param seed seed for the PRNG
 * @param size number of indexes
 * @param indexes pointer to the array of indexes
 * @param lowerbound lowest index value, included
 * @param upperbound highest index value, excluded
 * @return a negative value upon error, either 0 or the time in milliseconds
 * @see PRNG
 * @note if seed is NULL, it will be initialized.
 */
double prng_rand_without_replacement(unsigned long *seed, size_t size, unsigned *indexes, unsigned lowerbound, unsigned upperbound);

#endif