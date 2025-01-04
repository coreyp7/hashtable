#include <stdlib.h>
#include <stdio.h>

typedef struct {
	int key;
	int value;
	// to easily tell if this block in memory exists
	// 1 if alive and instantiated, 0 if not
	int alive;
} HashTableIntNode;

typedef struct {
	HashTableIntNode* array;
	int size;
} HashTableInt;

HashTableInt create_HashTableInt();

// Puts the provided key/value pair into the specified table.
int put(HashTableInt* table, int key, int value);

// Returns value of the provided key as an int pointer from the specified table.
// Will be NULL if the given key it isn't in the table.
int* get(HashTableInt* table, int key);





