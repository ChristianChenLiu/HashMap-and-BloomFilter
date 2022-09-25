/*
 * This file constains the relevant declarations for the Hash Functions that
 * will be used throughout this assignment. 
 *
 * You shouldn't change any of the code here. You will NOT be submitting this 
 * file as part of your assignment.   
 *
 * CSCB63 Winter 2021 - Assignment 4
 *
 * Many of the hash functions defined here and in Hash.c are designed by
 * others, just implemented by me. Be careful about attributions if you
 * ever wish to use these in your own work.
 */

#pragma once

#include <stdint.h>

// Add this typedef so compiler can type-check
typedef unsigned long hash_t;

// This typedef is so we don't have to deal with function pointer syntax later.
typedef hash_t (*HashFunc)(const char *);

// Needed for SuperFastHash. Interpret pointer as uint_16 * and dereference to
// get the value of next 16 bits.
#define Get16Bits(data) (*((const uint16_t *) (data)))

// Most of these hash functions are pulled from here:
// https://stackoverflow.com/questions/7666509/hash-function-for-string
hash_t hash_first_letter(const char *str);
hash_t hash_xor(const char *str);
hash_t hash_djb2(const char *str);
hash_t hash_sdbm(const char *str);
hash_t hash_lose_lose(const char *str);
hash_t hash_jenkins_otat(const char *str);
hash_t hash_murmur(const char *str);
hash_t hash_superfast(const char *str);
