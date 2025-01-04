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

int put(HashTableInt* table, int key, int value);
int _put_no_resize(HashTableInt* table, int key, int value);

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
	return 95;
}

void resize(HashTableInt* table){
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

float get_load_factor(HashTableInt* table){
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

int put(HashTableInt* table, int key, int value){
	int result = _put_no_resize(table, key, value);
	printf("Inserted K:%i, V:%i\n", key, value);
	
	// If load factor is too big, then let's resize the array.
	float loadFactor = get_load_factor(table);
	if(loadFactor > .7){
		printf("table with size %i needs to be resized", table->size);
		printf("load factor is %f\n", loadFactor);
		printf("time to expand array\n");
		resize(table);
		printf("exiting put after resize\n");
	}
	return result;
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
	HashTableIntNode* node; //= &table->array[index];


	for(int i=index; i<table->size; i++){
		node = &table->array[i];

		if(node->alive && node->key == key){
			// Update the key with its new value
			table->array[i] = newNode;
			//node = NULL;
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
			//node = NULL;
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

// return pointer, to differentiate if its found or not.
// use this to check if your map contains a value.
// NULL if not in map, int* to int value given key.
int* get(HashTableInt* table, int key){
	// hash the value
	int index = _hash(key) % table->size;

	// get the struct from the array
	HashTableIntNode* node = &table->array[index];

	// linear search to find value	
	// TODO: change this to for loop because this is UGLY
	while(node < table->array + (sizeof(HashTableIntNode)*table->size)){
		if(node->alive && node->key == key){
			return &node->value;
		}
		node++;
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

void print_dict_array(HashTableInt* table){
	for(int i=0; i<table->size; i++){
		HashTableIntNode node = table->array[i];
		printf("index %i: (%i, %i, %i)\n", i, node.key, node.value, node.alive);
	}
}

int main(){
	// Testing collisions (hash function should be changed to always return 0)
	HashTableInt table = create_HashTableInt(100);

	for(int i=0; i<25; i++){
		put(&table, i, i*5);
	}
	print_dict_array(&table);
	assert(*get(&table, 2) == 10);
	assert(*get(&table, 11) == 55);

	for(int i=0; i<95; i++){
		put(&table, i, i*5);
	}
	print_dict_array(&table);
	for(int i=96; i<195; i++){
		put(&table, i, i*5);
	}
	print_dict_array(&table);
	
	/*
	Remaining scenarios:
	- when the array is too full (use load factor calc I believe)
	- when there's collisions
		- this works as far as I can tell, just test alot
		and see how its handled in edge cases 
	*/

	/*
		Okay; when putting check load factor.
		# elements / nodes in table (size)
		When above, idk, .7 then we should double the size of the array.
		(or prime number, whatever)
	*/
}


