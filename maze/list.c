/* 
 * Benjamin Airola
 * COEN 12
 * Project 4
 * 05/09/2021
 * Description: Dequeue ADT Implementation Utilizing Doubly Linked Circular List
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "list.h"

/* Data structure for LIST */
struct list {
	int count;
	int (*compare)();
	struct node *head;
};

/* data type for NODE  */
typedef struct node {
	void *data;
	struct node *next;
	struct node *prev;
}NODE;

/*
 * Time Complexity: O(1)
 * Description: Generates a doubly linked list with a dummy node absent of data
 */
LIST *createList(int (*compare)()) {
	LIST *lp = malloc(sizeof(LIST));
	assert(lp!=NULL);
	lp->count = 0;
	lp->compare = compare;
	lp->head = malloc(sizeof(NODE));
	assert(lp->head!=NULL);
	lp->head->next = lp->head;
	lp->head->prev = lp->head;
	return lp;
}

/*
 * Time Complexity: O(n)
 * Description: Frees all memory associated with LIST lp
 */
void destroyList(LIST *lp) {
	assert(lp!=NULL);
	NODE *pCur = lp->head;
	while(pCur->next != pCur) {
		pCur->next = pCur->next->next;
		free(pCur->next->prev);
		pCur->next->prev = pCur;
	}
	free(lp->head);
	free(lp);
}

/*
 *Time Complexity: O(1)
 * Description: Returns the total number of elements in LIST lp
 */
int numItems(LIST *lp) {
	assert(lp!=NULL);
	return lp->count;
}

/*
 * Time Complexity: O(1)
 * Description: Adds element to the first location in our list, after head
 */
void addFirst(LIST *lp, void *item) {
	assert(lp!=NULL && item!=NULL);
	NODE *temp = malloc(sizeof(NODE));
	assert(temp!=NULL);
	temp->data = item;
	temp->next = lp->head->next;
	temp->prev = lp->head;
	lp->head->next->prev = temp;
	lp->head->next = temp;
	lp->count++;
}

/*
 * Time Complexity: O(1)
 * Description: Adds element to the last location in our list, preceding head
 */
void addLast(LIST *lp, void *item) {
	assert(lp!=NULL && item!=NULL);
	NODE *temp = malloc(sizeof(NODE));
	assert(temp!=NULL);
	temp->data = item;
	temp->next = lp->head;
	temp->prev = lp->head->prev;
	lp->head->prev->next = temp;
	lp->head->prev = temp;
	lp->count++;
}

/*
 * Time Complexity: O(1)
 * Description: Removes first element, after head
 */
void *removeFirst(LIST *lp) {
	assert(lp!=NULL && lp->count > 0);
	NODE *pDel = lp->head->next;
	lp->head->next = pDel->next;
	pDel->next->prev = lp->head;
	void *data = pDel->data;
	free(pDel);
	lp->count--;
	return data;
}

/*
 * Time Complexity: O(1)
 * Description: Removes last element, preceding head
 */
void *removeLast(LIST *lp) {
	assert(lp!=NULL && lp->count > 0);
	NODE *pDel = lp->head->prev;
	lp->head->prev = pDel->prev;
	pDel->prev->next = lp->head;
	void *data = pDel->data;
	free(pDel);
	lp->count--;
	return data;
}

/*
 * Time Complexity: O(1)
 * Description: Returns the first element, following head
 */
void *getFirst(LIST *lp) {
	assert(lp!=NULL && lp->count > 0);
	return lp->head->next->data;
}

/*
 * Time Complexity: O(1)
 * Description: Returns the last element, preceding head
 */
void *getLast(LIST *lp) {
	assert(lp!=NULL && lp->count > 0);
	return lp->head->prev->data;
}

/*
 * Time Complexity: O(n)
 * Description: Removes a specific element from LIST lp, implementing sequential search
 */
void removeItem(LIST *lp, void *item) {
	assert(lp!=NULL && item!=NULL && lp->compare!=NULL);
	NODE *pCur = lp->head->next;
	int i;
	for(i = 0; i < lp->count; i++) {
        	if(lp->compare(pCur->data, item) == 0) {
            		pCur->prev->next = pCur->next;
			pCur->next->prev = pCur->prev;
			free(pCur);
            		lp->count--;
			return;
		}
		pCur = pCur->next;
    	}
}

/*
 * Time Complexity: O(n) 
 * Description: Finds specific element in list LP and returns the data
 */
void *findItem(LIST *lp, void *item) {
	assert(lp!=NULL && item!=NULL && lp->compare!=NULL);
	NODE *pCur = lp->head->next;
    	int i;
    	for(i = 0; i < lp->count; i++) {
        	if(lp->compare(item, pCur->data) == 0)
            		return pCur->data;
        	pCur = pCur->next;
    	}
    	return NULL;
}

/*
 * Time Complexity: O(n)
 * Description: Returns an array data copy of LIST lp
 */
void *getItems(LIST *lp) {
    	assert(lp!=NULL);
    	void **data = malloc(sizeof(void*)*lp->count);
    	int i;
    	NODE *pCur = lp->head->next;
    	for(i = 0; i<lp->count; i++) {
        	data[i] = pCur->data;
        	pCur = pCur->next;
    	}
    	return data;
}
