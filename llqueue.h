#ifndef _LLQUEUE_H_
#define _LLQUEUE_H_

typedef struct _List
{
	void *elt;
	struct _List *next;
} List;

int enqueue (List **q, void ***data);
int dequeue (List **q, void **data);

#endif
