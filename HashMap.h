/*
 * This file contains the definition of a Dynamic HashMap and the associated 
 * linked list structure used while chaining. Relevant functions are also
 * declared.
 *
 * You shouldn't change any of the code here. You will NOT be submitting this 
 * file as part of your assignment. 
 *
 * CSCB63 Winter 2021 - Assignment 4
 * (c) Mustafa Quraish
 */

#pragma once

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Hash.h"

#define MAX_STRING_LEN 1024

typedef struct bucket_node {
  char key[MAX_STRING_LEN];   // Key being stored
  char value[MAX_STRING_LEN]; // Corresponding value for the key
  struct bucket_node *next;   // Pointer to the next element in the Linked list
} BucketNode;

typedef struct hash_map {
  int num_buckets;       // Number of buckets
  int num_elems;         // Number of elements in the HashMap
  BucketNode **buckets;  // Pointer to array of buckets
  HashFunc func;         // Hash function for the HashMap
} HashMap;

HashMap *newHashMap(int num_buckets, HashFunc func);
HashMap *HashMap_Add(HashMap *hm, const char *key, const char *value);
HashMap *HashMap_Resize(HashMap *hm, int new_size);
char *HashMap_Find(HashMap *hm, const char *key);
HashMap *HashMap_Delete(HashMap *hm, const char *key);
void HashMap_Free(HashMap *hm);
