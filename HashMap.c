/**
 *                         Dynamic Hash Maps
 *                              ---
 *  Like Dynamic Arrays, we can also have Dynamic HashMaps. Working out the
 *  math would for the complexity for this structure would give you
 *  amortized expected O(1) for Add(), Find() and Delete(). Try to prove
 *  this if just computing expected and amortized costs on their own wasn't
 *  enough for you! :)
 *                              ---
 *
 * This is the file where all of your implementation for the HashMap goes. 
 * You will be submitting this file (along with BloomFilter.c) to MarkUs.
 *
 * TestHashMap.c provides some test cases to test your implementation. 
 * You can compile it using the Makefile and running the executable produced.
 *
 * CSCB63 Winter 2021 - Assignment 4 
 * (c) Mustafa Quraish
 */

#include "HashMap.h"

// @brief Create and initialize a new HashMap with the given values
//
// @param num_buckets  The number of buckers the HashMap starts off with
// @param func         The hash function to be used for this HashMap
//
// @return             A pointer to the HashMap
//
// @TODO: Implement this function
HashMap *newHashMap(int num_buckets, HashFunc func) {

	HashMap *new_hash_map = malloc(sizeof(HashMap));						//Malloc sizeof(HashMap) for new_hash_map

	if (new_hash_map == NULL) {
		perror("malloc");
		exit(1);
	}
	
	new_hash_map->num_buckets = num_buckets;								//Make new_hash_map->num_buckets be the num_buckets
	new_hash_map->num_elems = 0;											//Make new_hash_map->num_elems be 0 since its empty
	new_hash_map->buckets = malloc(sizeof(BucketNode *) * num_buckets);		//Make new_hash_map->buckets be an array of size num_buckets of BucketNode *

	if (new_hash_map->buckets == NULL) {
		perror("malloc");
		exit(1);
	}

	for (int i = 0; i < num_buckets; i++) new_hash_map->buckets[i] = NULL;	//Setting every base bucket node to be NULL

	new_hash_map->func = func;												//Make new_hash_map->func be the func		

	return new_hash_map;													//Return new_hash_map

}

// @brief Construct a new HashMap of the given size, and re-hash all
//        the values from the old one. This needs to be done when expanding
//        or shrinking the Dynamic HashMap during insertion and deletion.
//
// @param hm         The old HashMap
// @param new_size   Required size of new HashMap
//
// @return           New HashMap (old one must be freed).
//
// @TODO: Implement this function. For the sake of consistency, the automarker
//        will expect you to re-hash all the elements in a specific order. Go
//        through the buckets sequentially from [0 - hm->num_buckets], and for
//        each one insert the nodes in the linked list sequentially. See the
//        test casesfor example behaviour.
HashMap *HashMap_Resize(HashMap *hm, int new_size) {

	HashMap *new_hash_map = newHashMap(new_size, hm->func);					//Create a new_hash_map

	BucketNode *bucket_node_pointer;								

	for (int i=0; i < hm->num_buckets; i++) {								//Loop through every single bucket
		bucket_node_pointer = (hm->buckets)[i];								//Get the pointer to the base of a bucket: the base bucket node

		while (bucket_node_pointer != NULL) {								//While the pointer to a bucket node is not NULL

			new_hash_map = HashMap_Add(new_hash_map, bucket_node_pointer->key, bucket_node_pointer->value);	//For each node in a bucket, hash it to the new_hash_map

			bucket_node_pointer = bucket_node_pointer->next;				//Get the next bucket pointer
		}

	}

	HashMap_Free(hm);														//Free the old useless HashMap

  	return new_hash_map;													//Return the new HashMap
}

// @brief Add a key-value pair to the HashMap. Dynamically resize
//        the hashmap when at capacity.
//
// @param hm     HashMap
// @param key    Key to be inserted
// @param value  Corresponding Value
//
// @return     Pointer to resulting HashMap (useful when you expand)
//
// @TODO: Implement this function. Here are some notes in implementation:
//        (1) If the key exists, replace the value.
//
//         (2) If the key doesn't exist, add the new BucketNode at the
//            *head* of the corresponding linked list
//
//        (3) To get the array into the buckets[] array, use the hash
//            function inside `hm` and then take that value mod the
//            number of buckets:
//                      idx = hash(key) % num_buckets
//
//        (4) AFTER you have inserted the new element, if the number
//            of elements in the HashMap is >= the number of buckets,
//            double the size of the HashMap and re-hash all the elements.
HashMap *HashMap_Add(HashMap *hm, const char *key, const char *value) {

	int idx = (hm->func)(key) % hm->num_buckets;							//Get the index of the bucket in the HashMap

	BucketNode *bucket_node_pointer = (hm->buckets)[idx]; 					//Get the appropriate base bucket node in the HashMap
	BucketNode *temp_bucket_node_pointer = bucket_node_pointer;

	while (temp_bucket_node_pointer != NULL) {								//Search for the case where we only ened to swap values		
		if (strcmp(temp_bucket_node_pointer->key, key) == 0) {				//Check if the temp_bucket_node_pointer->key is the key
			strncpy(temp_bucket_node_pointer->value, value, MAX_STRING_LEN);//If the temp_bucket_node_pointer->key is the key, then swap values
			return hm;
		}

		temp_bucket_node_pointer = temp_bucket_node_pointer->next;			//Get the next bucket node
	}	

	BucketNode *new_bucket_node = malloc(sizeof(BucketNode));				//Create a new bucket node to add since the key is not there

	if (new_bucket_node == NULL) {
		perror("malloc");
		exit(1);
	}

	strncpy(new_bucket_node->key, key, MAX_STRING_LEN);						//Make the new bucket node's key be the given key
	strncpy(new_bucket_node->value, value, MAX_STRING_LEN);					//Make the new bucket node's value be the given value

	new_bucket_node->next = bucket_node_pointer;							//Set the new bucket node's next to be bucket node from before
	(hm->buckets)[idx] = new_bucket_node;									//Set the base bucket node at idx to be the new_bucket_node

	hm->num_elems += 1;														//Increase the number of elements in hm by 1

	//Double the size of the HashMap If the number of elemnts in the HashMap is >= the number of buckets
	if (hm->num_elems >= hm->num_buckets) hm = HashMap_Resize(hm, hm->num_buckets*2);	

  	return hm;
}

// @brief Find the value of the given key in the HashMap
//        Return NULL if key does not exist.
//
// @param hm   HashMap
// @param key  Key to be queried in the hashmap
//
// @return     The corresponding value to the key, or NULL.
//
// @TODO: Implement this function
char *HashMap_Find(HashMap *hm, const char *key) {

	int idx = (hm->func)(key) % hm->num_buckets;							//Get the index of the bucket in the HashMap

	BucketNode *bucket_node_pointer = (hm->buckets)[idx]; 					//Get the appropriate base bucket node in the HashMap

	while (bucket_node_pointer != NULL) {									//For each node in a bucket

			if (strcmp(bucket_node_pointer->key, key) == 0) {				//Check if the bucket node's key is equal to the searching key
				return bucket_node_pointer->value;							//Return the bucket node's value if the key is the searched key
			}

			bucket_node_pointer = bucket_node_pointer->next;				//Go to the next node in the bucket
		}	

  	return NULL;
}

// @brief Delete a key-value pair from the HashMap. Dynamically resize
//        the HashMap if necessary.
//
// @param hm   HashMap
// @param key  Key for which the pair needs to be deleted.
//
// @return     Pointer to resulting HashMap (useful when you shrink)
//
// @TODO: Implement this function. Here are some notes in implementation:
//        (1) If the key doesn't exist, don't do anything.
//
//         (2) Don't reorder the rest of the list when removing the node
//
//         (3) AFTER deleting the element, if num_elems < num_buckets/4, then
//             shrink the HashMap by half. (Similar to Dynamic Arrays). You'll
//             need to re-hash all the elements.
HashMap *HashMap_Delete(HashMap *hm, const char *key) {

	int idx = (hm->func)(key) % hm->num_buckets;									//Get the index of the bucket in the HashMap

	BucketNode *bucket_node_pointer = (hm->buckets)[idx];							//Get the appropriate base bucket node in the HashMap
	BucketNode *temp_bucket_node_pointer;

	if (bucket_node_pointer == NULL) return hm;

	if (strcmp(bucket_node_pointer->key, key) == 0) {								//Lookout for the case where the first bucket node is the one we looking for
		(hm->buckets)[idx] = bucket_node_pointer->next;								//Set the bucket base node to be the bucket_node_pointer->next
		free(bucket_node_pointer);													//Free the "deleted" bucket node
		hm->num_elems -= 1;															//Decrease the number of elements by 1
		bucket_node_pointer = (hm->buckets)[idx];
	}

	if (bucket_node_pointer != NULL) {
		while (bucket_node_pointer->next != NULL) {									//For each node in a bucket

			if (strcmp(bucket_node_pointer->next->key, key) == 0) {					//Check if the bucket node->next's key is equal to the searching key
				temp_bucket_node_pointer = bucket_node_pointer->next;				//Save the bucket_node_pointer->next
				bucket_node_pointer->next = bucket_node_pointer->next->next;		//Set bucket_node_pointer->next's to bucket_node_pointer->next->next
				free(temp_bucket_node_pointer);										//Free the "deleted" bucket node
				hm->num_elems -= 1;													//Decrease the number of elements by 1
			}

			bucket_node_pointer = bucket_node_pointer->next;						//Go to the next node in the bucket
		}	
	}

	//Shrink the size of the HashMap by half If the number of elements in the HashMap is < the number of buckets/4
	if (hm->num_elems < (int)hm->num_buckets/4) hm = HashMap_Resize(hm, (int)hm->num_buckets/2);	

  	return hm;
}

// @brief Frees all the memory used by the HashMap
//
// @param hm   HashMap
//
// @TODO: Implement this function. You will need to use valgrind to properly 
//        check the functionality.
void HashMap_Free(HashMap *hm) {

	BucketNode *bucket_node_pointer, *temp_bucket_node_pointer; 

	for (int i=0; i < hm->num_buckets; i++) {
		bucket_node_pointer = (hm->buckets)[i];
		while (bucket_node_pointer != NULL) {	
			temp_bucket_node_pointer = bucket_node_pointer;
			bucket_node_pointer = bucket_node_pointer->next;
			free(temp_bucket_node_pointer);
		}
	}
	free(hm->buckets);
	free(hm);

  	return;
}
