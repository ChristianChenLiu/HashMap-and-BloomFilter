CC = gcc
CFLAGS = -Wall -Werror -g
OBJ = Hash.o HashMap.o BloomFilter.o TestHashMap.o TestBloomFilter.o

all: TestHashMap TestBloomFilter

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

TestHashMap: Hash.o HashMap.o TestHashMap.o
	$(CC) -o $@ $^ $(CFLAGS)

TestBloomFilter: Hash.o BloomFilter.o TestBloomFilter.o
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(OBJ) ./TestHashMap ./TestBloomFilter

