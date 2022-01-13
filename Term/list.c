/* 
 * Ben Airola
 * COEN 12
 * Term Project
 * Description: Abstract data type implementation utilizing a linked list of circular queues, represented by arrays.
 * 05/23/2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "list.h"

#define START_LENGTH 15

/* Data type for our structure, containing a list of nodes */
struct list {
	struct node *head;
	struct node *tail;
	int total_count;
};

/* Data type for our nodes, each containing a circular queue represented through an array */
typedef struct node {
	struct node *next;
	struct node *prev;
	void **data;
	int count;
	int first;
}NODE;

static NODE *createNode(NODE *prev, NODE *next);

/* 
 * Time Complexity: O(1)
 * Description: This function generates a new list structure (with a sentinel) and returns its memory location.
 */
LIST *createList(void) {
	LIST *lp = malloc(sizeof(LIST));
	assert(lp!=NULL);
	lp->head = createNode(NULL, NULL);
	lp->tail = lp->head;
	lp->total_count = 0;
	return lp;
}

/* 
 * Time Complexity: O(1)
 * Description: Initializes and returns memory location for a single node. 
 */
static NODE *createNode(NODE *prev, NODE *next) {
	NODE *np = malloc(sizeof(NODE));
	assert(np!=NULL);
	np->data = malloc(sizeof(void*)*(START_LENGTH));
	assert(np->data!=NULL);
	np->prev = prev;
	np->next = next;
	np->count = 0;
	np->first = 0;
	return np;
}

/* 
 * Time Complexity: O(n)
 * Description: This function frees all memory associated with list lp.
 */
void destroyList(LIST *lp) {
	assert(lp!=NULL);
	NODE *pCur = lp->head->next;
	NODE *pDel;
	int i;
	/* Freeing memory of each node individually. */
	while(pCur!=NULL) {
		for(i = 0; i < START_LENGTH; i++)
			free(pCur->data[i]);
		pDel = pCur;
		pCur = pCur->next;
		free(pDel->data);
		free(pDel);
	}
	free(lp->head);
	free(lp);
}

/* 
 * Time Complexity: O(1)
 * Description: Returns the total number of items in list lp.
 */
int numItems(LIST *lp) {
	assert(lp!=NULL);
	return lp->total_count;
}

/* 
 * Time Complexity: O(1)
 * Description: Adds new item to the very first location on the first node after the sentinel. If necessary, a new node is allocated.
 */
void addFirst(LIST *lp, void *item) {
	assert(lp!=NULL && item!=NULL);
	/* Case 1: The list is empty and has no nodes, EXCEPT for the sentinel.  */
	if(lp->head->next == NULL) {
		lp->head->next = createNode(lp->head, NULL);
		lp->tail = lp->head->next;
	}
	/* Case 2: The first node is full, and a new one must be allocated. */
	else if(lp->head->next->count == START_LENGTH) {
		NODE *temp = createNode(lp->head, lp->head->next);
		lp->head->next->prev = temp;
		lp->head->next = temp;
	}
	/* Adding element to first location. */
	int first = (lp->head->next->first + START_LENGTH - 1) % (START_LENGTH);
	lp->head->next->data[first] = item;
	lp->head->next->first = first;
	lp->total_count++;
	lp->head->next->count++;
}

/* 
 * Time Complexity: O(1)
 * Description: Adds new item to the very last location on the last node. If necessary, a new node is allocated.
 */
void addLast(LIST *lp, void *item) {
	assert(lp!=NULL && item!=NULL);
	/* Case 1: The list is empty and has no nodes, EXCEPT for the sentinel. */
	if(lp->head->next == NULL) {
		lp->head->next = createNode(lp->head, NULL);
		lp->tail = lp->head->next;
	}
	/* Case 2: The last node is full, and a new one must be allocated. */
	else if(lp->tail->count == START_LENGTH) {
		NODE *temp = createNode(lp->tail, NULL);
		lp->tail->next = temp;
		lp->tail = temp;
	}
	/* Adding element to last location. */
	int last = (lp->tail->first + lp->tail->count) % (START_LENGTH);
	lp->tail->data[last] = item;
	lp->total_count++;
	lp->tail->count++;
}

/* 
 * Time Complexity: O(1)
 * Description: Removes the very first element of the list. If this action causes a node to be empty, then node will be removed.
 */
void *removeFirst(LIST *lp) {
	assert(lp!=NULL && lp->total_count > 0);
	void *item = lp->head->next->data[lp->head->next->first];
	lp->head->next->first = (lp->head->next->first + 1) % (START_LENGTH);
	lp->head->next->count--;
	lp->total_count--;
	/* Case where deletion of item causes node to be empty. */
	if(lp->head->next->count == 0) {
		/* Case 1: Entire list is empty. Tail pointer must be handled separately. */
		if(lp->total_count == 0) {
			free(lp->head->next->data);
			free(lp->head->next);
			lp->head->next = NULL;
			lp->tail = lp->head;
		/* Case 2: List has other nodes with elements, after deletion. */
		} else {
			NODE *pDel = lp->head->next;
			lp->head->next = lp->head->next->next;
			lp->head->next->prev = lp->head;
			free(pDel->data);
			free(pDel);
		}
	}
	return item;
}

/* 
 * Time Complexity: O(1)
 * Description: Removes very last element of the list. If this action causes a node to be empty, then said node will be removed. 
 */
void *removeLast(LIST *lp) {
	assert(lp!=NULL && lp->total_count > 0);
	int last = (lp->tail->first + lp->tail->count - 1) % (START_LENGTH);
	void *item = lp->tail->data[last];
	lp->tail->count--;
	lp->total_count--;
	/* Case where deletion of item causes node to be empty. */
	if(lp->tail->count == 0) {
		NODE *pDel = lp->tail;
		lp->tail = lp->tail->prev;
		lp->tail->next = NULL;
		free(pDel->data);
		free(pDel);
	}
	return item;
}

/* 
 * Time Complexity: O(n)
 * Description: Returns an item at a specified index. 
 */
void *getItem(LIST *lp, int index) {
	assert(lp!=NULL);
	NODE *pLoc = lp->head->next;
	int i = index;
	/* Finding node containing our item, and adjusting the index along the way. */
	while(i >= pLoc->count) {
		i = i - pLoc->count;
		pLoc = pLoc->next;
	}
	/* Returning the item at the index. */
	int loc = (pLoc->first + i) % (START_LENGTH);
	return pLoc->data[loc];
}

