#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "HashTableInt.h"

void print_table_arr(HashTableInt* table){
	for(int i=0; i<table->size; i++){
		HashTableIntNode node = table->array[i];
		printf("index %i: (%i, %i, %i)\n", i, node.key, node.value, node.alive);
	}
}

int main(){
	HashTableInt table = create_HashTableInt();

	srand(time(NULL));
	
	int* keys = malloc(sizeof(int) * 295);

	for(int i=0; i<295; i++){
		int randomNumb = rand();
		keys[i] = randomNumb;
		put(&table, randomNumb, randomNumb*5);
		printf("put(%i, %i)\n", i, i*5);
	}

	print_table_arr(&table);

	for(int i=0; i<295; i++){
		int key = keys[i];
		assert(*get(&table, key) == key*5);
	}
}
