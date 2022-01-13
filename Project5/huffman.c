/*
 * Benjamin Airola
 * COEN 12
 * Project 5
 * 05/27/2021
 * Description: Implementaiton of huffman coding to compress text files, using a binary tree and priority queue.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include "pqueue.h"
#include "pack.h"

typedef struct node NODE;

/* 
 * Time Complexity: O(logn)
 * Description: Returns the depth of any given leaf node on our Huffman tree.
 */
int depth(NODE *leaf) {
	int count = 0;
	while(leaf->parent != NULL) {
		count++;
		leaf = leaf->parent;
	}
	return count;
}

/* 
 * Time Complexity: O(1)
 * Description: Compare function that is passed to our priority queue.
 */
int compare(NODE *first, NODE *second) {
	return (first->count < second->count) ? -1 : 1;
}

/* 
 * Time Complexity: O(1)
 * Description: Makes new node given left and right children, and combines their data.
 */
NODE *mkNode(NODE *first, NODE *second) {
	NODE *temp = malloc(sizeof(NODE));
	assert(temp!=NULL);
	first->parent = temp;
	second->parent = temp;
	temp->count = first->count + second->count;
	return temp;
}

/* 
 * Description: Implements huffman coding to compress a text file, using a priority queue and binary huffman tree.
 */
int main(int argc, char *argv[]) {
	assert(argc > 1);
	FILE *fp = fopen(argv[1], "r");
	assert(fp!=NULL);
	
	/* Creates integer array tracking occurences of every character in file. */
	int count[257] = {0};
	while(1) {
		int c = fgetc(fp);
		if(feof(fp))
			break;
		count[c]++;
	}
	fclose(fp);
	PQ *pq = createQueue(compare);
	
	/* Creates an array of leaf nodes associated with each character and its frequency, and adds them to the priority queue. */
	int i;	
	NODE *leaves[257] = {NULL};
	for(i = 0; i < 257; i++) {
		if(count[i] > 0) {
			NODE *temp = malloc(sizeof(NODE));
			assert(temp!=NULL);
			temp->parent = NULL;
			temp->count = count[i];
			leaves[i] = temp;
			addEntry(pq, temp);
		}
	}

	/* Adding zero count to leaves array and priority queue. */
	NODE *zero = malloc(sizeof(NODE));
	assert(zero!=NULL);
	zero->parent = NULL;
	zero->count = 0;
	leaves[256] = zero;
	addEntry(pq, zero);
	
	/* Building the Huffman tree. */
	while(numEntries(pq) > 1) {
		NODE *first = removeEntry(pq);
		NODE *second = removeEntry(pq);
		NODE *parent = mkNode(first, second);
		addEntry(pq, parent);
	}

	/* Prints out required bits for each character. */
	for(i = 0; i < 257; i++) {
		if(leaves[i] != NULL) {
			int d = depth(leaves[i]);
			if(isprint(i))
				printf("'%c' : %d * %d = %d bits.\n", i, count[i], d, count[i] * d);
			else
				printf("%o : %d * %d = %d bits.\n", i, count[i], d, count[i] * d);
		}
	}
	
	pack(argv[1], argv[2], leaves);
	return 0;
}


















