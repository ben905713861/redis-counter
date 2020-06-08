/*
 * Listint_int.h
 * 数据结构 - int型list
 *  Created on: 2020年6月7日
 *      Author: wuxb
 */

#ifndef listint_H_
#define listint_H_

#include<stdlib.h>
#include<stdio.h>

typedef unsigned int bool;
#define TRUE 1
#define FALSE 0

typedef struct ListintNode {
	struct ListintNode* nextNode;
	int value;
} ListintNode;

typedef struct Listint {
	ListintNode* headNode;
	ListintNode* lastNode;
	int length;
} Listint;

typedef struct ListintIterator {
	ListintNode* next;
} ListintIterator;

extern Listint* listint_create();
extern void listint_add(Listint* listint, int value);
extern ListintIterator* listint_iterator(Listint* listint);
extern bool listint_iterator_hasNext(ListintIterator* iterator);
extern ListintNode* listint_iterator_next(ListintIterator* iterator);
extern void listint_iterator_free(ListintIterator* iterator);
extern void listint_delete(Listint* listint, ListintNode* node);
extern void listint_free(Listint* listint);
extern void listint_showall(Listint* listint);

#endif /* listint_H_ */
