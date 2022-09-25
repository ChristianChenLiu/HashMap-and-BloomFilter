/**
 * This file contains the definition of BloomFilters, along with relevant
 * function declarations and macros for bit manipulations.
 *
 * You shouldn't change any of the code here. You will NOT be submitting this 
 * file as part of your assignment. 
 *
 * CSCB63 Winter 2021 - Assignment 4
 * (c) Mustafa Quraish
 */

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Hash.h"

typedef struct bloom_filter {
  int num_bits;         // Number of bits stored in BF
  int num_hash;         // Number of hash funcs. used
  unsigned char *data;  // (Byte) array containing the bits

  // This one's a bit tricky: a pointer to an *array* of
  // hash function (pointers) to store the ones being used.
  HashFunc *funcs;  // Array of hash func. pointers

} BloomFilter;

// @brief Gets the value of the n^th bit in the bitmap (0 / 1)
#define GETBIT(arr, n) (!!(arr[n / 8] & (1 << (n % 8))))
// @brief Sets the n^th bit in the bitmap (to 1).
#define SETBIT(arr, n) (arr[n / 8] = arr[n / 8] | (1 << (n % 8)))
// @brief Unsets the n^th but in the bitmap (to 0).
#define UNSETBIT(arr, n) (arr[n / 8] = arr[n / 8] & ~(1 << (n % 8)))

BloomFilter *newBloomFilter(int num_bits, int num_hash, HashFunc *funcs);
void BloomFilter_Add(BloomFilter *bf, const char *str);
int BloomFilter_Check(BloomFilter *bf, const char *str);
