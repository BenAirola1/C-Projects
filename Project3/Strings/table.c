#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "set.h"

static unsigned strhash(char *s);
static int search(SET *sp, char *elt, bool *found);

struct set{
	char **data;
	char *flag;
	int count;
	int length;
};


/*
 * Time Complexity: O(n)
 * Description: Creates a new set
 */
SET *createSet(int maxElts) {
	SET *sp = malloc(sizeof(SET));
	assert(sp!=NULL);
	sp->length = maxElts;
	sp->count = 0;
	sp->data = malloc(sizeof(char*)*maxElts);
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
	int i;
	for(i = 0; i < sp->length; i++) {
		if(sp->flag[i] == 'F')
			free(sp->data[i]);
	}
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
void addElement(SET *sp, char *elt) {
	assert((sp!=NULL) && (elt!=NULL) && (sp->count < sp->length));
	bool found;
	int index = search(sp, elt, &found);
	if((!found) && index > -1) {
		sp->data[index] = strdup(elt);
		sp->flag[index] = 'F';
		sp->count++;
	}
}


/*
 *  Time Complexity: O(n) [expected O(1)]
 *  Description: Removes specified element from set SP
 */
void removeElement(SET *sp, char *elt) {
	assert((sp!=NULL) && (elt!=NULL) && (sp->count > 0));
	bool found;
	int index = search(sp, elt, &found);
	if(found) {
		free(sp->data[index]);
		sp->flag[index] = 'D';
		sp->count--;
	}
} 


/*
 *  Time Complexity: O(n) [expected O(1)]
 *  Description: Returns location of speficied element in SET sp
 */
char *findElement(SET *sp, char *elt) {
	assert((sp!=NULL) && (elt!=NULL) && (sp->count >= 0));
	bool found;
	int index = search(sp, elt, &found);
	if(found)
		return sp->data[index];
	return NULL;
}


/*
 *  Time Complexity: O(n)
 *  Description: Returns a copy of all elements in SET sp
 */
char **getElements(SET *sp) {
	assert(sp!=NULL);
	char **copy = malloc(sizeof(char*)*sp->count);
	int i; 
	int j = 0;
	for(i = 0; i < sp->length; i++) {
		if(sp->flag[i] == 'F') {
			copy[j] = sp->data[i];
			j++;
		}
	}
	return copy;
}


/*
 *  Time Complexity: O(n)
 *  Description: simple hash function for mapping elements to location in table
 */
unsigned strhash(char *s) {
	unsigned hash = 0;
	while(*s != '\0')
		hash = 31*hash + *s++;
	return hash;
}


/*
 *  Time Complexity: O(n) [expected O(1)]
 *  Description: Locates a specific element in the hash table using linear probing, 
 *  returns the closest empty location to the home index if element is not found
 */
int search(SET *sp, char *elt, bool *found) {
	int home = strhash(elt) % (sp->length);
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
		else if(strcmp(sp->data[index], elt) == 0) {
			*found = true; /* element was found, returns location */
			return index;
		}
	}
	*found = false; /* element not found, returns first occurence of deleted location */
	return deleted;
}


