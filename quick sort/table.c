/* 
 * Benjamin Airola
 * COEN 12
 * Project 6
 * 06/3/2021
 * Description: This is an abstract data type implementing a quick sort algorithm as one of the functions.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "set.h"

static int search(SET *sp, void *elt, bool *found);

static void quickSort(void *data[], int (*compare)(), int start, int length);

static void partition(void *data[], int (*compare)(), int start, int length);

static void swap(void *data[], int first, int second);

struct set{
	void **data;
	char *flag;
	int count;
	int length;
	int (*compare)();
	unsigned (*hash)();
};


/*
 * Time Complexity: O(n)
 * Description: Creates a new set
 */
SET *createSet(int maxElts, int (*compare)(), unsigned (*hash)()) {
	SET *sp = malloc(sizeof(SET));
	assert(sp!=NULL);
	sp->length = maxElts;
	sp->count = 0;
	sp->compare = compare;
	sp->hash = hash;
	sp->data = malloc(sizeof(void*)*maxElts);
	assert(sp->data!=NULL);
	sp->flag = malloc(sizeof(char)*maxElts);
	assert(sp->flag!=NULL);
	int i;
	for(i = 0; i < maxElts; i++)
		sp->flag[i] = 'E';
	return sp;
}


/*
 *  Time Complexity: O(n)
 *  Description: Frees all memory associated with SET sp
 */
void destroySet(SET *sp) {
	assert(sp!=NULL);
	free(sp->data);
	free(sp->flag);
	free(sp);
}


/*
 *  Time Complexity: O(1)
 *  Description: Returns the number of elements in SET sp
 */
int numElements(SET *sp) {
	assert(sp!=NULL);
	return sp->count;	
}


/*
 *  Time Complexity: O(n) [expected O(1)]
 *  Description: Adds new element to hashed location in SET sp
 */
void addElement(SET *sp, void *elt) {
	assert((sp!=NULL) && (elt!=NULL) && (sp->count < sp->length));
	bool found;
	int index = search(sp, elt, &found);
	if((!found) && index > -1) {
		sp->data[index] = elt;
		sp->flag[index] = 'F';
		sp->count++;
	}
}


/*
 *  Time Complexity: O(n) [expected O(1)]
 *  Description: Removes specified element from set SP
 */
void removeElement(SET *sp, void *elt) {
	assert((sp!=NULL) && (elt!=NULL) && (sp->count > 0));
	bool found;
	int index = search(sp, elt, &found);
	if(found) {
		sp->flag[index] = 'D';
		sp->count--;
	}
} 


/*
 *  Time Complexity: O(n) [expected O(1)]
 *  Description: Returns location of speficied element in SET sp
 */
void *findElement(SET *sp, void *elt) {
	assert((sp!=NULL) && (elt!=NULL) && (sp->count >= 0));
	bool found;
	int index = search(sp, elt, &found);
	if(found)
		return sp->data[index];
	return NULL;
}


/*
 *  Time Complexity: O(n)
 *  Description: Returns an ordered copy of all elements in SET sp
 */
void *getElements(SET *sp) {
	assert(sp!=NULL);
	void **copy = malloc(sizeof(void*)*sp->count);
	assert(copy!=NULL);
	int i; 
	int j = 0;
	for(i = 0; i < sp->length; i++) {
		if(sp->flag[i] == 'F') {
			copy[j] = sp->data[i];
			j++;
		}
	}
	quickSort(copy, sp->compare, 0, sp->count);
	return copy;
}

/* 
 * Time Complexity: O(nlogn)
 * Description: Sorts data set using the quick sort method.
 */
void quickSort(void *data[], int (*compare)(), int start, int length) {
	if(length > 1) {
		partition(data, compare, start, length);
		int i = start;
		int j = start + length;
		while(i < j-1) {
			while(compare(data[i+1], data[start]) < 0)
				i++;
			while(compare(data[j-1], data[start]) > 0)
				j--;
			if(i < j-1) {
				swap(data, i+1, j-1);
			}
		}
		swap(data, start, i);
		quickSort(data, compare, 0, i+1);
		quickSort(data, compare, j, length-j);	
	}
}

/* 
 * Time Complexity: O(1)
 * Description: Supplemental function for quick sort in order to locate an optimal pivot.
 */
void partition(void *data[], int (*compare)(), int start, int length) {
	int mid = (start + length - 1)/2;
	int end = start + length - 1;
	if(compare(data[start], data[mid]) > 0) {
		swap(data, start, mid);
	}
	if(compare(data[start], data[end]) > 0) {
		swap(data, start, end);
	}
	if(compare(data[mid], data[end]) > 0) {
		swap(data, mid, end);
	}
	swap(data, start, mid);
}

/* 
 * Time Complexity: O(1)
 * Description: Swaps two elements in data set.
 */
void swap(void *data[], int first, int second) {
	void *temp = data[first];
	data[first] = data[second];
	data[second] = temp;
}	

/*
 *  Time Complexity: O(n) [expected O(1)]
 *  Description: Locates a specific element in the hash table using linear probing, 
 *  returns the closest empty location to the home index if element is not found
 */
static int search(SET *sp, void *elt, bool *found) {
	int home = (*sp->hash)(elt) % (sp->length);
	int deleted = -1;
	int index, i;
	for(i = 0; i < sp->length; i++) {
		index = (home + i) % sp->length;
		if(sp->flag[index] == 'D') {
			if(deleted == -1)  /* saves first occurrence of deleted index */
				deleted = index;
		}
		else if(sp->flag[index] == 'E') {
			*found = false;  /* element was not found, returns first occurence of empty location */ 
			if(deleted > -1)
				return deleted;
			return index;
		}
		else if((*sp->compare)(sp->data[index], elt) == 0) {
			*found = true; /* element was found, returns location */
			return index;
		}
	}
	*found = false; /* element not found, returns first occurence of deleted location */
	return deleted;
}


