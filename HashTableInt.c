#include "HashTableInt.h"

int _put_no_resize(HashTableInt* table, int key, int value);
void _resize(HashTableInt* table);
int _hash(int x);
float _get_load_factor(HashTableInt* table);

// Header functions are first, then all the "helper" functions are below.

HashTableInt create_HashTableInt(){
	int size = 100;
	HashTableInt table = {
		calloc(size, sizeof(HashTableIntNode) * size),
		size
	};
	return table;
}


// Put and _put_no_resize were organized like this so that I could
// avoid recursively calling put over and over.
int put(HashTableInt* table, int key, int value){
	int result = _put_no_resize(table, key, value);
	
	// If load factor is too big, then let's resize the array.
	float loadFactor = _get_load_factor(table);
	if(loadFactor > .7){
		_resize(table);
	}
	return result;
}

// Return pointer, to differentiate if its found or not.
// use this to check if your map contains a value.
// NULL if not in map, int* to int value given key.
int* get(HashTableInt* table, int key){
	// hash the value
	int index = _hash(key) % table->size;

	// get the struct from the array
	HashTableIntNode* node = &table->array[index];

	// linear search to find value	
	for(int i=index; i<table->size; i++){
		node = &table->array[i];
		if(node->alive && node->key == key){
			return &node->value;
		}
	}
	
	// Not found in array from index its supposed to be at to end of array.
	// Search from beginning of array.
	for(int i=0; i<index; i++){
		node = &table->array[i];
		if(node->alive && node->key == key){
			return &node->value;
		}
	}

	// Not sure when this will ever happen
	return NULL;
}


void _resize(HashTableInt* table){
	int newSize = table->size * 2;
	HashTableIntNode* newArr = calloc(
		newSize, 
		sizeof(HashTableIntNode) * newSize
	);
	HashTableInt newTable = {newArr, newSize};

	// copy all nodes to the new array, with new hash
	// loop through prev table arr, hash key, put in new place
	for(int i=0; i<table->size; i++){
		HashTableIntNode node = table->array[i];
		if(node.alive == 0){
			continue;
		}

		int newIndex = _hash(node.key) % newSize; 
		_put_no_resize(&newTable, node.key, node.value);
	}
	
	free(table->array);
	table->array = newArr;
	table->size = newSize;
}

float _get_load_factor(HashTableInt* table){
	// # alive nodes / size
	int aliveNodes = 0;
	int size = table->size;
	for(int i=0; i<size; i++){
		if(table->array[i].alive == 1){
			aliveNodes++;	
		}
	}
	float loadFactor = ((float)aliveNodes/size);
	return loadFactor;
}


int _put_no_resize(HashTableInt* table, int key, int value){

	// hash the value, get index
	int index = _hash(key) % table->size;
	
	if(index > table->size){
		// something went terribly wrong
		printf("!hey dude something wrong with hashing the value!\n");
		return -1;
	}

	HashTableIntNode newNode = {key, value, 1};
	HashTableIntNode* node;


	for(int i=index; i<table->size; i++){
		node = &table->array[i];

		if(node->alive && node->key == key){
			// Update the key with its new value
			table->array[i] = newNode;
			return 0;
		}

		if(!node->alive){
			// This is an empty node, we can put K/V here.
			table->array[i] = newNode;
			return 0;
		}	
	}

	// Failed to put in array from index to end, all nodes are full.
	// Start searching from beginning of array for a place for this node.
	for(int i=0; i<index; i++){
		// TODO: put into function, this is duplicate code
		node = &table->array[i];

		if(node->alive && node->key == key){
			// Update the key with its new value
			table->array[i] = newNode;
			return 0;
		}

		if(!node->alive){
			// This is an empty node, we can put K/V here.
			table->array[i] = newNode;
			return 0;
		}	

	}

	// This should never be reached, since we dynamically expand the array
	// when the load factor is too high (> .7).
	// Check if this ever returns 1 when testing.
	return 1;
}

// https://stackoverflow.com/questions/664014/what-integer-hash-function-are-good-that-accepts-an-integer-hash-key
int _hash(int x){
	x = ((x >> 16) ^ x) * 0x45d9f3b;
	x = ((x >> 16) ^ x) * 0x45d9f3b;
 	x = (x >> 16) ^ x;
	return x;	
}



