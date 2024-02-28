#ifndef TICTOC_H
#define TICTOC_H

/**
 * @file tictoc.h
 * @brief Execution time evaluation
 *
 * This file exposes macros for easy and precise execution time evaluation
 * with programs.
 */

#include <time.h>

/**
 * @brief Starts the time evaluation.
 */
#define TIC(start) clock_gettime(CLOCK_MONOTONIC_RAW, &start)

/**
 * @brief Stops the time evaluation.
 */
#define TOC(end) clock_gettime(CLOCK_MONOTONIC_RAW, &end)

/**
 * @brief Returns the time interval "TOC - TIC" in milliseconds.
 */
#define TIC_TOC(start, end) (((double)end.tv_sec * 1e9 + end.tv_nsec) - ((double)start.tv_sec * 1e9 + start.tv_nsec)) / 1000000

#endif