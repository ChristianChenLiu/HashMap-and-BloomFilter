/**
 * A few basic test cases are provided here for your implementations. You will
 * NOT be submitting this as part of the assignment. I strongly recommend you
 * add additional test cases here to ensure correct functionality of your code.
 *
 * Compile using `make` and run all tests using `./TestHashMap`. Additionally, 
 * you can run individual tests by passing in the test number, like:  
 *                                                      `./TestHashMap 3`.
 *
 * CSCB63 Winter 2021 - Assignment 4 
 * (c) Mustafa Quraish
 */

#include "HashMap.h"

#define UNITTEST_DEFAULT_MAIN
#include "unittest.h"

// @brief Prints out the hashmap to hel debug
void printHashMap(HashMap *hm) {
  printf("--- HashMap Contents ---\n");
  for (int i = 0; i < hm->num_buckets; i++) {
    printf("* Bucket #%d: \n", i);
    if (hm->buckets[i] == NULL) printf("    - NULL\n");
    for (BucketNode *cur = hm->buckets[i]; cur != NULL; cur = cur->next) {
      printf("    - KEY: '%s', VALUE: '%s'\n", cur->key, cur->value);
    }
  }
}

// @brief Checks if a BucketNode matches the expected values.
//
// @param bn   BucketNode pointer
// @param key  Expected Key
// @param value  Expected Value
// @param nextNull  Boolean indicating if `next` should be NULL
int checkNode(BucketNode *bn, const char *key, const char *value,
              int nextNull) {
  if (bn == NULL) return 0;
  if (strcmp(bn->key, key) != 0) return 0;
  if (strcmp(bn->value, value) != 0) return 0;
  if (!!bn->next == nextNull) return 0;
  return 1;
}

//////////////////////////////////////////////////////

// @brief Tests the functionality of newHashMap()
TEST(newHashMap) {
  HashMap *hm = newHashMap(5, hash_sdbm);
  assert(hm);
  assert(hm->buckets);
  assert(hm->func == hash_sdbm);
  assert(hm->num_buckets == 5);
  for (int i = 0; i < 5; i++) assert(hm->buckets[i] == NULL);
  HashMap_Free(hm);  // Use valgrind to make sure this works
}

// @brief Tests the Basic functionality of HashMap_Add()
TEST(HashMap_Add_basic) {
  HashMap *hm = newHashMap(7, hash_xor);
  hm = HashMap_Add(hm, "CSC", "B63");
  for (int i = 0; i < 4; i++) assert(hm->buckets[i] == NULL);
  for (int i = 5; i < 7; i++) assert(hm->buckets[i] == NULL);
  assert(checkNode(hm->buckets[4], "CSC", "B63", 1));

  hm = HashMap_Add(hm, "Data", "Structures");
  for (int i = 0; i < 4; i++) assert(hm->buckets[i] == NULL);
  for (int i = 5; i < 7; i++) assert(hm->buckets[i] == NULL);
  assert(checkNode(hm->buckets[4], "Data", "Structures", 0));
  assert(checkNode(hm->buckets[4]->next, "CSC", "B63", 1));

  hm = HashMap_Add(hm, "CSC", "B36");
  assert(checkNode(hm->buckets[4], "Data", "Structures", 0));
  assert(checkNode(hm->buckets[4]->next, "CSC", "B36", 1));

  hm = HashMap_Add(hm, "test", "case");
  assert(checkNode(hm->buckets[2], "test", "case", 1));

  HashMap_Free(hm);  // Use valgrind to make sure this works
}

// @brief Tests the dynamic expansion in HashMap_Add()
TEST(HashMap_Add_expand) {
  HashMap *hm = newHashMap(2, hash_xor);

  hm = HashMap_Add(hm, "CSC", "B63");
  assert(checkNode(hm->buckets[0], "CSC", "B63", 1));

  hm = HashMap_Add(hm, "Data", "Structures");  // Expand!
  assert(hm->num_buckets == 4);
  assert(checkNode(hm->buckets[0], "CSC", "B63", 1));
  assert(checkNode(hm->buckets[3], "Data", "Structures", 1));

  hm = HashMap_Add(hm, "test", "case");
  assert(checkNode(hm->buckets[0], "CSC", "B63", 1));
  assert(checkNode(hm->buckets[1], "test", "case", 1));
  assert(checkNode(hm->buckets[3], "Data", "Structures", 1));

  hm = HashMap_Add(hm, "foo", "bar");  // Expand
  assert(hm->num_buckets == 8);
  assert(checkNode(hm->buckets[1], "test", "case", 0));
  assert(checkNode(hm->buckets[1]->next, "foo", "bar", 1));
  assert(checkNode(hm->buckets[4], "CSC", "B63", 1));
  assert(checkNode(hm->buckets[7], "Data", "Structures", 1));

  hm = HashMap_Add(hm, "PHP", "Sucks");
  assert(checkNode(hm->buckets[1], "test", "case", 0));
  assert(checkNode(hm->buckets[1]->next, "foo", "bar", 1));
  assert(checkNode(hm->buckets[4], "CSC", "B63", 1));
  assert(checkNode(hm->buckets[7], "PHP", "Sucks", 0));
  assert(checkNode(hm->buckets[7]->next, "Data", "Structures", 1));

  HashMap_Free(hm);  // Use valgrind to make sure this works
}

// @brief Tests the functionality of HashMap_Find()
TEST(HashMap_Add_find) {
  HashMap *hm = newHashMap(13, hash_xor);
  hm = HashMap_Add(hm, "CSC", "B63");
  hm = HashMap_Add(hm, "Data", "Structures");
  hm = HashMap_Add(hm, "test", "case");
  hm = HashMap_Add(hm, "Data", "Destroyed");

  assert(strcmp(HashMap_Find(hm, "CSC"), "B63") == 0);
  assert(strcmp(HashMap_Find(hm, "Data"), "Destroyed") == 0);
  assert(HashMap_Find(hm, "Algorithms") == NULL);

  HashMap_Free(hm);  // Use valgrind to make sure this works
}

// @brief Tests basic functionality of in HashMap_Delete()
TEST(HashMap_Delete_basic) {
  HashMap *hm = newHashMap(11, hash_first_letter);

  // All these will have the same hash
  hm = HashMap_Add(hm, "Key1", "Value1");
  hm = HashMap_Add(hm, "Key2", "Value2");
  hm = HashMap_Add(hm, "Key3", "Value3");
  hm = HashMap_Add(hm, "Key4", "Value4");
  hm = HashMap_Add(hm, "Key5", "Value5");
  // Different hash
  hm = HashMap_Add(hm, "Map", "Hash");

  hm = HashMap_Delete(hm, "Key5");
  hm = HashMap_Delete(hm, "Key2");

  assert(checkNode(hm->buckets[0], "Map", "Hash", 1));
  assert(checkNode(hm->buckets[9], "Key4", "Value4", 0));
  assert(checkNode(hm->buckets[9]->next, "Key3", "Value3", 0));
  assert(checkNode(hm->buckets[9]->next->next, "Key1", "Value1", 1));

  HashMap_Free(hm);  // Use valgrind to make sure this works
}

// @brief Tests Dynamic shrinking in HashMap_Delete()
TEST(HashMap_Delete_shrink) {
  // Start of with an array much bigger than needed.
  // Make sure you're NOT shrinking inside HashMap_Add().
  HashMap *hm = newHashMap(17, hash_jenkins_otat);

  hm = HashMap_Add(hm, "Key1", "Value1");
  hm = HashMap_Add(hm, "Key2", "Value2");
  hm = HashMap_Add(hm, "Key3", "Value3");
  hm = HashMap_Add(hm, "Key4", "Value4");
  hm = HashMap_Add(hm, "Key5", "Value5");
  assert(hm->num_buckets == 17);  // Should be unchanged

  hm = HashMap_Delete(hm, "Key5");
  hm = HashMap_Delete(hm, "Key2");  // Shrink
  assert(hm->num_buckets == 8);     // floor(17 / 2) = 8

  assert(checkNode(hm->buckets[3], "Key1", "Value1", 1));
  assert(checkNode(hm->buckets[4], "Key4", "Value4", 1));
  assert(checkNode(hm->buckets[6], "Key3", "Value3", 1));

  HashMap_Free(hm);  // Use valgrind to make sure this works
}