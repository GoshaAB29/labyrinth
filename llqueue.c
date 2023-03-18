#include <stdio.h>
#include <stdlib.h>
#include "llqueue.h"

int enqueue (List ** queue, void *** elt) {
	List * new = (List*)malloc (sizeof(List));
	if (!new) return -1;
    new->next = NULL;

	*elt = &new->elt;

	if (!*queue) {
		new->next = new;
	} else {
		new->next = (*queue)->next;
		(*queue)->next = new;
	}

	*queue = new;
	return 0;
}

int dequeue (List** queue, void **elt) {
	List *old;
	if (!*queue) return -1;

	old = (*queue)->next;
	(*queue)->next = old->next;

    *elt = old->elt;

	if (old == *queue)
		*queue = NULL;

	free (old);
	return 0;
}


