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
	/*
	x = ((x >> 16) ^ x) * 0x45d9f3b;
	x = ((x >> 16) ^ x) * 0x45d9f3b;
 	x = (x >> 16) ^ x;
	return x;	
	*/
	return 0;
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
	HashTableIntNode* node; //= &table->array[index];


	printf("index is %i\n", index);
	for(int i=index; i<table->size; i++){
		node = &table->array[i];

		if(node->key == key){
			// Update the key with its new value
			table->array[i] = newNode;
			node = NULL;
			return 0;
		}

		if(!node->alive){
			// This is an empty node, we can put K/V here.
			table->array[i] = newNode;
			return 0;
		}	
		
	}

	/*
	while(node < table->array + (sizeof(HashTableIntNode)*table->size)){
		if(!node->alive || node->key == key){
			// TODO: change this to a for loop so we can easily
			// print/observe the current index.
			printf("%i || %i, setting at node\n", !node->alive, node->key == key);
			table->array[index] = newNode;
			node = NULL;
			return 0;
		}
		node++;
	}
	*/
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

void print_dict_array(HashTableInt* table){
	// NOTE: only printing alive nodes
	for(int i=0; i<table->size; i++){
		HashTableIntNode node = table->array[i];
		//if(node.alive){
			printf("index %i: (%i, %i, %i)\n", i, node.key, node.value, node.alive);
		//}
	}
}

int main(){
	// Testing collisions (hash function should be changed to always return 0)
	HashTableInt table = create_HashTableInt(100);
	printf("put return: %i\n", put(&table, 4, 248724)); // index 0
	print_dict_array(&table);
	assert(get(&table, 4) != NULL);
	assert(*get(&table, 4) == 248724);

	put(&table, 2, 98654); // index 1
	print_dict_array(&table);
	assert(get(&table, 2) != NULL);
	assert(*get(&table, 2) == 98654); // will linear search until found
	put(&table, 2, 7); // index 1, replace the old value
	assert(*get(&table, 2) == 7); // will linear search until found
	
	/*
	Remaining scenarios:
	- when the array is too full (use load factor calc I believe)
	- when there's collisions
		- change the hash function to just return 0 everytime and
		do it alot
	*/
}


