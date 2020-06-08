/*
 * listinit.c
 *
 *  Created on: 2020年6月7日
 *      Author: wuxb
 */

#include "listint.h"


Listint* listint_create() {
	Listint* listint = (Listint*) malloc(sizeof(Listint));
	if(listint == NULL) {
		puts("Listint创建失败，内存不足");
		exit(-100);
	}
	return listint;
}


void listint_add(Listint* listint, int value) {
	ListintNode* node = (ListintNode*) malloc(sizeof(ListintNode));
	if(listint == NULL) {
		puts("Node创建失败，内存不足");
		exit(-100);
	}
	node->value = value;
	if(listint->length == 0) {
		listint->headNode = node;
	} else {
		listint->lastNode->nextNode = node;
	}
	listint->lastNode = node;
	listint->length ++;
}

ListintIterator* listint_iterator(Listint* listint) {
	ListintIterator* iterator = (ListintIterator*) malloc(sizeof(ListintIterator));
	if(iterator == NULL) {
		puts("ListintIterator创建失败，内存不足");
		exit(-100);
	}
	if(listint->length == 0) {
		iterator->next = NULL;
	} else {
		iterator->next = listint->headNode;
	}
	return iterator;
}

bool listint_iterator_hasNext(ListintIterator* iterator) {
	if(iterator->next == NULL) {
		return FALSE;
	}
	return TRUE;
}

ListintNode* listint_iterator_next(ListintIterator* iterator) {
	ListintNode* node = iterator->next;
	iterator->next = node->nextNode;
	return node;
}

void listint_iterator_free(ListintIterator* iterator) {
	free(iterator);
}

void listint_delete(Listint* listint, ListintNode* node) {
	if(listint->length == 0) {
		return;
	}
	if(listint->length == 1) {
		free(node);
		listint->headNode = NULL;
		listint->lastNode = NULL;
		listint->length = 0;
		return;
	}
	if(listint->headNode == node) {
		listint->headNode = node->nextNode;
		free(node);
		listint->length --;
		return;
	}
	//用遍历法
	ListintNode* iteratorNode = listint->headNode;
	while(1) {
		if(iteratorNode->nextNode == NULL) {
			puts("待删除节点不属于该list");
			return;
		}
		if(iteratorNode->nextNode == node) {
			break;
		}
		iteratorNode = iteratorNode->nextNode;
	}
	iteratorNode->nextNode = node->nextNode;
	free(node);
	listint->length --;
}

void listint_free(Listint* listint) {
	ListintNode* iteratorNode = listint->headNode;
	while(1) {
		if(iteratorNode == NULL) {
			break;
		}
		ListintNode* nextnode = iteratorNode->nextNode;
		free(iteratorNode);
		iteratorNode = nextnode;
	}
	free(listint);
}

void listint_showall(Listint* listint) {
	ListintNode* iteratorNode = listint->headNode;
	while(1) {
		if(iteratorNode == NULL) {
			break;
		}
		printf("%d, ", iteratorNode->value);
		iteratorNode = iteratorNode->nextNode;
	}
	printf("\n");
}

