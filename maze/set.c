/* 
 * Benjamin Airola
 * COEN 12
 * Project 4
 * 05/09/2021
 * Description: ADT Implementation Utilizing Hashing with Chaining and Circular Linked Lists
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "list.h"
#include "set.h"

#define LENGTH 30

/* Data structure for SET */
struct set {
	int count;
	int length;
	LIST **lists;
	int (*compare)();
	unsigned (*hash)();
};

/*
 * Time Complexity: O(1)
 * Description: Generates a new set structure
 */
SET *createSet(int maxElts, int (*compare)(), unsigned (*hash)()) {
	SET *sp = malloc(sizeof(SET));
	assert(sp!=NULL);
	sp->count = 0;
	sp->length = LENGTH;
	sp->lists = malloc(sizeof(LIST*)*LENGTH);
	assert(sp->lists!=NULL);
	sp->compare = compare;
	sp->hash = hash;

	/* creating list at every location in hash table */
	int i;
	for(i = 0; i < sp->length; i++)
		sp->lists[i] = createList(compare);
	return sp;
}

/*
 * Time Complexity: O(n)
 * Description: Frees all memory associated with LIST lp
 */
void destroySet(SET *sp) {
	assert(sp!=NULL);
	int i;
	for(i = 0; i < sp->length; i++)
		destroyList(sp->lists[i]);
	free(sp->lists);
	free(sp);
}

/*
 *Time Complexity: O(1)
 * Description: Returns the total number of elements in LIST lp
 */
int numElements(SET *sp) {
	assert(sp!=NULL);
	return sp->count;
}

/*
 * Time Complexity: O(1) expected, O(n) worse case
 * Description: Adds unique element to hashed location, and chains if already occupied
 */
void addElement(SET *sp, void *elt) {
	assert(sp!=NULL && elt!=NULL);
	int index = (*sp->hash)(elt) % sp->length;
	void *item = findItem(sp->lists[index], elt);
	if(item == NULL) {
		addFirst(sp->lists[index], elt);
		sp->count++;
	}
}

/*
 * Time Complexity: O(1) expected, O(n) worst case
 * Description: Removes unique element from hashed location
 */
void removeElement(SET *sp, void *elt) {
	assert(sp!=NULL && elt!=NULL && sp->count > 0);
	int index = (*sp->hash)(elt) % sp->length;
	void *item = findItem(sp->lists[index], elt);
	if(item != NULL) {
		removeItem(sp->lists[index], elt);
		sp->count--;
	}	
}

/*
 * Time Complexity: O(1) expected, O(n) worst case
 * Description: Returns a pointer to the specified item
 */
void *findElement(SET *sp, void *elt) {
	assert(sp!=NULL && elt!=NULL);
	int index = (*sp->hash)(elt) % sp->length;
	return findItem(sp->lists[index], elt);
}

/*
 * Time Complexity: O(n)
 * Description: Returns an array copy of everysingle element in SET sp
 */
void *getElements(SET *sp) {
	assert(sp!=NULL);
	void **elts = malloc(sizeof(void*)*sp->count);
	void **tempAlloc;
	int numElts, i;
	int index = 0;
	for(i = 0; i < sp->length; i++) {
		tempAlloc = getItems(sp->lists[i]);
		numElts = numItems(sp->lists[i]);
		memcpy(elts + index, tempAlloc, numElts*sizeof(void*));
		index = index + numElts;
	}
	return elts;
}





