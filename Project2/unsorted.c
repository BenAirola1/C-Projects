#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "set.h"

static int search(SET *sp, char *elt);

typedef struct set{
	char **data;
	int length;
	int count;
}SET;


/* 
 * Time Complexity: O(1) 
 * Description: creates new set
 * */
SET *createSet(int maxElts) {
	SET *sp = malloc(sizeof(SET));
	assert(sp!=NULL);
	sp->count = 0;
	sp->length = maxElts;
	sp->data = malloc(sizeof(char*)*maxElts);
	assert(sp->data != NULL);
	return sp;
}


/* 
 * Time Complexity: O(n) 
 * Description: frees memory of entire data set
 * */
void destroySet(SET *sp) {
	assert(sp!=NULL);
	int i;
	//frees in order: strings >> data >> set
	for(i = 0; i < sp->count; i++) 
		free(sp->data[i]);
	free(sp->data);
	free(sp);
}


/* 
 * Time Complexity: O(1) 
 * Description: returns number of elements in data set
 * */
int numElements(SET *sp) {
	assert(sp!=NULL);
	return sp->count;
}


/* 
 * Time Complexity: O(n) 
 * Description: adds new element to unordered data set
 * */
void addElement(SET *sp, char *elt) {
	assert((sp!=NULL) && (elt!=NULL) && (sp->count < sp->length));
	//copies element from buffer and adds it to end of data
	char *new;
	if(search(sp, elt) == -1) {
		new = strdup(elt);
		assert(new!=NULL);
		sp->data[sp->count] = new;
		sp->count++;
	}
}


/* 
 * Time Complexity: O(n) 
 * Description: Removes specified element from unordered data set
 * */
void removeElement(SET *sp, char *elt) {
	assert((sp!=NULL) && (elt!=NULL));
	int index = search(sp, elt);
	//verifies element exists
	if(index != -1) {
		free(sp->data[index]);
		sp->data[index] = sp->data[sp->count - 1];
		sp->count--;
	}
}


/* 
 * Time Complexity: O(n) 
 * Description: returns address of specified element in data set
 * */
char *findElement(SET *sp, char *elt) {
	assert(sp!=NULL);
	int index = search(sp, elt);
	//returns string pointer if element exists, else returns NULL
	if(index != -1)
		return sp->data[index];
	return NULL;
}


/* 
 * Time Complexity: O(n) 
 * Description: Returns pointer to copy of data set
 * */
char **getElements(SET *sp) {
	assert(sp!=NULL);
	int i;
	char **data = malloc(sizeof(char*)*sp->length);
	assert(data!=NULL);
	for(i = 0; i < sp->count; i++)
		data[i] = sp->data[i];
	return data;
}


/*
 * Time Complexity: O(n)
 * Description: Implements sequential search to find specified element in data set
 */
int search(SET *sp, char *elt) {	
	int i;
	//returns index of element being searched, else returns -1 if not found
	for(i = 0; i < sp->count; i++) {
		if(strcmp(elt, sp->data[i]) == 0)
			return i;
	}	
	return -1;
}

