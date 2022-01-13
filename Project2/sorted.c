#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "set.h"

static int search(SET *sp, char *elt, bool *found);

typedef struct set{
	char **data;
	int length;
	int count;
}SET;


/*
 * Time Complexity: O(1) 
 * Description: creates new set
 */
SET *createSet(int maxElts) {
	SET *sp = malloc(sizeof(SET));
	assert(sp!=NULL);
	sp->count = 0;
	sp->length = maxElts;
	sp->data = malloc(sizeof(char*)*maxElts);
	assert(sp->data!=NULL);
	return sp;
}


/* 
 * Time Complexity: O(n) 
 * Description: frees all memory
 */
void destroySet(SET *sp) {
	assert(sp!=NULL);
	int i;
	//frees in this order: data, array, set
	for(i = 0; i < sp->count; i++) 
		free(sp->data[i]);
	free(sp->data);
	free(sp);
}


/*
 * Time Complexity: O(1)
 * Description: returns the number of elements in the set
 */
int numElements(SET *sp) {
	assert(sp!=NULL);
	return sp->count;
}


/*
 * Time Complexity: O(n) 
 * Description: Adds a new element to the ordered data set
 */
void addElement(SET *sp, char *elt) {
	assert((sp!=NULL) && (elt!=NULL) && (sp->count < sp->length));
	bool found;
	char *new;
	int index, i;	
	index = search(sp, elt, &found);
	//verifies no duplicates
	if(!found) {
		//shifts ordered list up by 1 to make space for new element
		for(i = sp->count; i > index; i--) {
			sp->data[i] = sp->data[i-1];
		}
		new = strdup(elt);
		assert(new!=NULL);
		sp->data[index] = new;
		sp->count++;
	}
}


/*
 * Time Complexity: O(n) 
 * Description: Removes an element from the ordered data set
 */
void removeElement(SET *sp, char *elt) {
	assert((sp!=NULL) && (elt!=NULL));
	bool found;
	int index, i;
	index = search(sp, elt, &found);
	//verifying element exists in set
	if(found) {
		free(sp->data[index]);
		//shifts elements down by one
		for(i = index; i < sp->count; i++) {
			sp->data[i] = sp->data[i+1];
		}
		sp->count--;
	}
}


/*
 * Time Complexity: O(logn) 
 * Description: Finds and return pointer to a specified element
 */
char *findElement(SET *sp, char *elt) {
	assert((sp!=NULL) && (elt!=NULL));
	bool found;
	int index = search(sp, elt, &found);
	//returns string pointer if duplicate exists, else returns NULL
	if(found)
		return sp->data[index];
	return NULL;
}


/*
 * Time Complexity: O(n) 
 * Description: Returns a pointer to a copy of the data set
 */
char **getElements(SET *sp) {
	assert(sp!=NULL);
	int i;
	char **data = malloc(sizeof(char*)*sp->length);
	assert(data!=NULL);
	//copies each element individually
	for(i = 0; i < sp->count; i++)
		data[i] = sp->data[i];
	return data;
}


/*
 * Time Complexity: O(logn) 
 * Description: Implements binary search to find a specific element, 
 * or where it would be inserted into the sorted data set
 */
int search(SET *sp, char *elt, bool *found) {	
	int low, high, mid;
	low = 0;
	high = sp->count - 1;
	//utilizing string compare function
	while(low <= high) {
		mid = (low + high)/2;
		if(strcmp(elt, sp->data[mid]) == 0) {
			*found = true;
			return mid;
		}
		if(strcmp(elt, sp->data[mid]) < 0)
			high = mid - 1;
		else
			low = mid + 1;	
	}
	*found = false;
	return low;
}

