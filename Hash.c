/**
 * This file constains the implementations for the Hash Functions that
 * will be used throughout this assignment. Not all of them will be used
 * for test cases either, they are simply provided to you if you are 
 * interested in looking at how they perform compared to each other (on
 * your own time).
 *
 * You shouldn't change any of the code here. You will NOT be submitting this 
 * file as part of your assignment.   
 *
 * CSCB63 Winter 2021 - Assignment 4
 *
 * Many of the hash functions defined here and in Hash.h are designed by
 * others, just implemented by me. Be careful about attributions if you
 * ever wish to use these in your own work.
 */

#include "Hash.h"

// @brief A terrible hash function you should never use in practice,
//        but convenient for testing...
hash_t hash_first_letter(const char *str) {
  hash_t hash = *str;
  return hash;
}

// @brief Basic XOR hash
hash_t hash_xor(const char *str) {
  hash_t hash = 0xdeadbeef;  // Just some random value
  for (; (*str); str++) {
    hash = hash ^ *str;
  }
  return hash;
}

// @brief DJB2 hash by Dan. Bernstein
//        http://www.cse.yorku.ca/~oz/hash.html
hash_t hash_djb2(const char *str) {
  hash_t hash = 5381;
  for (; (*str); str++) {
    hash = ((hash << 5) + hash) + (*str);
  }
  return hash;
}

// @bried SDBM Hashing algorithm.
//        http://www.cse.yorku.ca/~oz/hash.html
hash_t hash_sdbm(const char *str) {
  hash_t hash = 0;
  for (; (*str); str++) {
    hash = (hash << 6) + (hash << 16) - hash + (*str);
  }
  return hash;
}

// @brief Lose-Lose Hashing
//        http://www.cse.yorku.ca/~oz/hash.html
//
//        Note: Extremely simple, but terrible performance
//              (collision-wise) compared to the others.
hash_t hash_lose_lose(const char *str) {
  hash_t hash = 0;
  for (; (*str); str++) {
    hash = hash + (*str);
  }
  return hash;
}

// @brief Jenkin's One-at-a-time Hash Function
//        https://en.wikipedia.org/wiki/Jenkins_hash_function
hash_t hash_jenkins_otat(const char *str) {
  hash_t hash = 0;
  for (; (*str); str++) {
    hash = hash + (*str);
    hash = hash + (hash << 10);
    hash = hash ^ (hash >> 6);
  }
  hash = hash + (hash << 3);
  hash = hash ^ (hash >> 11);
  hash = hash + (hash << 15);
  return hash;
}

// @brief Murmur Hashing algorithm
//        https://en.wikipedia.org/wiki/MurmurHash
hash_t hash_murmur(const char *str) {
  hash_t hash = 525201411107845655;
  for (; (*str); str++) {
    hash = hash ^ (*str);
    hash = hash * 0x5bd1e9955bd1e995;
    hash = hash ^ (hash >> 47);
  }
  return hash;
}

// @brief SuperFastHash by Paul Hsieh.
//        Reconstructed from http://www.azillionmonkeys.com/qed/hash.html.
//        Appropriate Licence details available there if you wish to reuse
//        this code.
//
//        Note: Not actually super-fast because of implementation details :(
hash_t hash_superfast(const char *str) {
  hash_t temp;

  int i, len = 0;  // Need length of string here
  while ((str[len])) {
    len++;
  }
  hash_t hash = len;

  for (i = 0; i < len / 4; i++) {
    hash = hash + Get16Bits(str);
    temp = (Get16Bits(str + 2) << 11) ^ hash;
    hash = (hash << 16) ^ temp;
    hash = hash + (hash >> 11);
    str = str + 4;  // In general, 2 * sizeof(uint_16)
  }

  switch (len & 3) {  // Equivalent to len % 4
    case 1:
      hash = hash + (str[0]);
      hash = hash ^ (hash << 10);
      hash = hash + (hash >> 1);
      break;
    case 2:
      hash = hash + Get16Bits(str);
      hash = hash ^ (hash << 11);
      hash = hash + (hash >> 17);
      break;
    case 3:
      hash = hash + Get16Bits(str);
      hash = hash ^ (hash << 16);
      hash = hash ^ (str[2] << 18);
      hash = hash + (hash >> 11);
      break;
  }

  hash = hash ^ (hash << 3);
  hash = hash + (hash >> 5);
  hash = hash ^ (hash << 4);
  hash = hash + (hash >> 17);
  hash = hash ^ (hash << 25);
  hash = hash + (hash >> 6);

  return hash;
}
