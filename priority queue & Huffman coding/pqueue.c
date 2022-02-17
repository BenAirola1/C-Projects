/* 
 * Benjamin Airola
 * COEN 12 
 * Project 5
 * 05/22/2021
 * Description: Implementing a priority queue using a min binary heap, represented through an array.
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "pqueue.h"

#define left(x) (2*(x) + 1)
#define right(x) (2*(x) + 2)
#define parent(x) (((x)-1)/2)

/* Data type for our priority queue */
struct pqueue {
	int count;
	int length;
	void **data;
	int (*compare)();
};

/* 
 * Time Complexity: O(1)
 * Description: Generates and returns new priority queue
 */
PQ *createQueue(int (*compare)()) {
	PQ *pq = malloc(sizeof(PQ));
	assert(pq!=NULL);
	pq->count = 0;
	pq->length = 10;
	pq->data = malloc(sizeof(void*)*10);
	assert(pq->data!=NULL);
	pq->compare = compare;
	return pq;
}

/* 
 * Time Complexity: O(n)
 * Description: Frees all memory associated with priority queue
 */
void destroyQueue(PQ *pq) {
	assert(pq!=NULL);
	free(pq->data);
	free(pq);
}

/* 
 * Time Complexity: O(1)
 * Description: Returns the number of entries in the priority queue
 */
int numEntries(PQ *pq) {
	assert(pq!=NULL);
	return pq->count;
}

/* 
 * Time Complexity: O(logn)
 * Description: Adds entry to priority queue, and reheaps up to maintain order
 */
void addEntry(PQ *pq, void *entry) {
	assert(pq!=NULL && entry!=NULL);
	/* Re-allocating dynamic memory if capacity is exceeded */
	if(pq->count == pq->length) {
		pq->data = realloc(pq->data, sizeof(void*)*(pq->length)*2);
		assert(pq->data!=NULL);
		pq->length = pq->length*2;
	}
	int index = pq->count;
	/* Reheaping up */
	while(index!=0 && pq->compare(entry, pq->data[parent(index)]) < 0) {
		pq->data[index] = pq->data[parent(index)];
		index = parent(index);
	}
	pq->data[index] = entry;
	pq->count++;
}


/* 
 * Time Complexity: O(logn)
 * Description: Removes specified entry and reheaps down to maintain order
 */
void *removeEntry(PQ *pq) {
	assert(pq!=NULL && pq->count > 0);
	void *entry = pq->data[0];
	int index = 0; 
	int smaller;
	/* Reheaping down */
	while(right(index) < pq->count){
		smaller = (pq->compare(pq->data[left(index)], pq->data[right(index)]) <= 0) ? left(index) : right(index);
		if(pq->compare(pq->data[smaller], pq->data[pq->count - 1]) > 0)
			break;
		pq->data[index] = pq->data[smaller];
		index = smaller;
	}
	/* Moving last entry to proper location */
	pq->data[index] = pq->data[pq->count - 1];
	pq->count--;
	return entry;
}

 
























