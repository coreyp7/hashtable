#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

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


HashTableInt create_HashTableInt(int size){
	HashTableInt table = {
		calloc(size, sizeof(HashTableIntNode) * size),
		size
	};
	return table;
}

// https://stackoverflow.com/questions/664014/what-integer-hash-function-are-good-that-accepts-an-integer-hash-key
int _hash(int x){
	x = ((x >> 16) ^ x) * 0x45d9f3b;
	x = ((x >> 16) ^ x) * 0x45d9f3b;
 	x = (x >> 16) ^ x;
	return x;	
}

int put(HashTableInt* table, int key, int value){
	// hash the value, get index
	int index = _hash(key) % table->size;
	
	if(index > table->size){
		// something went terribly wrong
		printf("hey dude something wrong with hashing the value\n");
		return -1;
	}

	HashTableIntNode newNode = {key, value, 1};
	HashTableIntNode* node = &table->array[index];

	while(node < table->array + (sizeof(HashTableIntNode)*table->size)){
		if(!node->alive || node->key == key){
			table->array[index] = newNode;
			node = NULL;
			return 0;
		}
		node++;
	}
	return 1;
}

// return pointer, to differentiate if its found or not.
// use this to check if your map contains a value.
// NULL if not in map, int* to int value given key.
int* get(HashTableInt* table, int key){
	// hash the value
	int index = _hash(key) % table->size;

	// get the struct from the array
	HashTableIntNode* node = &table->array[index];
	// linear search if necessary to find value	
	while(node < table->array + (sizeof(HashTableIntNode)*table->size)){
		if(node->alive && node->key == key){
			return &node->value;
		}
		node++;
	}

	return NULL;
}

int main(){
	HashTableInt table = create_HashTableInt(100);
	put(&table, 4, 248724);
	assert(get(&table, 4) != NULL);
	assert(*get(&table, 4) == 248724);
}


