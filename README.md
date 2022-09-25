# CSCB63 Assignment 4: Hash Maps and Bloom Filters

You will be implemented a dynamic hashmap and bloom filter for this assignment.
Don't worry if you don't know the details of these yet, they will be covered
in lecture soon. If you want to get a headstart though, the test cases and 
function documentation should be enough to give you a rough idea on what is 
expected.

There's a few files in here, so here's a short summary of what each of them do:

- `Hash.*`: Just a bunch of different hash functions, already implemented for 
            you. Don't need to do anything here.

- `Test*.c`: Some basic test cases for your HashMap and BloomFilter. Add your
             own test cases to ensure correctness.

- `HashMap.*`: The actual HashMap implementation. You will need to complete
               the functions in `HashMap.c`.

- `BloomFilter.*`: The actual Bloom Filter implementation. You will need to 
                   complete the functions in `BloomFilter.c`. 

---

To compile, run `make` in your terminal while in this directory. Alternatively,
you can manually compile the executables using:

```
gcc -Wall -Werror -g -o TestHashMap Hash.c HashMap.c TestHashMap.c
gcc -Wall -Werror -g -o TestBloomFilter Hash.c BloomFilter.c TestBloomFilter.c
```

---

## Some notes:

- If you see `Assertion failed ... abort trap`, this just means one of the test
    cases failed. The tests this time around are using the `assert()` functions
    (which you will commonly see in your career)

- Make sure you understand the test cases if you are debugging them, it is 
    best to work out what should happen by hand once you have computed the
    correct hash values.

- When debugging, you may find it helpful to write a really simply hash function
    yourself and use that. For example, look at `hash_first_letter()`.