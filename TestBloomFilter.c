/**
 * A few basic test cases are provided here for your implementations. You will
 * NOT be submitting this as part of the assignment. I strongly recommend you
 * add additional test cases here to ensure correct functionality of your code.
 *
 * Compile using `make` and run all tests using `./TestBloomFilter`. 
 * Additionally, you can run individual tests by passing in the test number, 
 * like `./TestBloomFilter 3`.
 *
 * CSCB63 Winter 2021 - Assignment 4 
 * (c) Mustafa Quraish
 */

#include "BloomFilter.h"

#define UNITTEST_DEFAULT_MAIN
#include "unittest.h"

// @brief Prints out the BloomFilter to help debug
void printBloomFilter(BloomFilter *bf) {
  printf("BloomFilter: ");
  for (int i = 0; i < bf->num_bits; i++) {
    printf("%d", GETBIT(bf->data, i));
    if (i % 8 == 7) printf(" ");  // Print out in blocks of 8 bits
  }
  printf("\n");
}

// @brief Checks if the BloomFilter bitmap matches the expected values.
//
// @param bn   BloomFilter
// @param exp  Expected bitmap (stored as unsigned char[])
int checkBitmap(BloomFilter *bf, unsigned char exp[]) {
  int num_chars = (bf->num_bits + 7) / 8;  // ceiling(n / 8)
  // Just check character-wise
  for (int i = 0; i < num_chars; i++) {
    if (bf->data[i] != exp[i]) return 0;
  }
  return 1;
}

//////////////////////////////////////////////////////

// @brief Tests the functionality of newBloomFilter()
TEST(newBloomFilter) {
  HashFunc funs[] = {hash_sdbm, hash_xor, hash_djb2};
  BloomFilter *bf = newBloomFilter(13, 3, funs);

  assert(bf);
  assert(bf->num_bits == 13);
  assert(bf->num_hash == 3);
  assert(bf->funcs[0] == hash_sdbm);
  assert(bf->funcs[1] == hash_xor);
  assert(bf->funcs[2] == hash_djb2);

  unsigned char exp[] = {0, 0};  // All bits 0
  assert(checkBitmap(bf, exp));
}

// @brief Tests the functionality of BloomFilter_Add() with one HashFunc
TEST(BloomFilter_Add_one) {
  HashFunc funs[] = {hash_first_letter};
  BloomFilter *bf = newBloomFilter(8, 1, funs);
  BloomFilter_Add(bf, "a");
  BloomFilter_Add(bf, "c");
  BloomFilter_Add(bf, "g");

  /* printBloomFilter(bf); */
  unsigned char exp[] = {138};  // 01010001
  assert(checkBitmap(bf, exp));
}

// @brief Tests the functionality of BloomFilter_Add() with many HashFuncs
TEST(BloomFilter_Add_many) {
  HashFunc funs[] = {hash_first_letter, hash_xor, hash_superfast};
  BloomFilter *bf = newBloomFilter(24, 3, funs);
  BloomFilter_Add(bf, "a");
  BloomFilter_Add(bf, "c");
  BloomFilter_Add(bf, "g");

  /* printBloomFilter(bf); */
  unsigned char exp[] = {202, 4, 81};  // 01010011 00100000 10001010
  assert(checkBitmap(bf, exp));
}

hash_t hash_second_letter(const char *str) { return str[1]; }

TEST(BloomFilter_Check) {
  HashFunc funs[] = {hash_first_letter, hash_second_letter};
  BloomFilter *bf = newBloomFilter(16, 2, funs);
  BloomFilter_Add(bf, "ab");
  BloomFilter_Add(bf, "cd");
  BloomFilter_Add(bf, "jk");
  BloomFilter_Add(bf, "lm");

  /* printBloomFilter(bf); */
  unsigned char exp[] = {30, 60};  // 01111000 00111100
  assert(checkBitmap(bf, exp));

  // Actually in BF
  assert(BloomFilter_Check(bf, "ab"));
  assert(BloomFilter_Check(bf, "cd"));
  assert(BloomFilter_Check(bf, "jk"));
  assert(BloomFilter_Check(bf, "lm"));

  // False positives
  assert(BloomFilter_Check(bf, "ad"));
  assert(BloomFilter_Check(bf, "jm"));
  assert(BloomFilter_Check(bf, "am"));
  assert(BloomFilter_Check(bf, "lb"));

  // Not in the BloomFilter
  assert(!BloomFilter_Check(bf, "ax"));
  assert(!BloomFilter_Check(bf, "ej"));
  assert(!BloomFilter_Check(bf, "df"));
  assert(!BloomFilter_Check(bf, "mg"));
}