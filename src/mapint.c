/*
 * mapint.c
 *
 *  Created on: 2020年6月8日
 *      Author: wuxb
 */


#include "mapint.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static unsigned int hashCode(char* str);
static unsigned int getIndex(char* str, int size);


Mapint* mapint_create(int tableSize) {
	Mapint* mapint = (Mapint*) calloc(1, sizeof(Mapint));
	mapint->tableSize = tableSize;
	mapint->hashTable = (MapintNode**) calloc(tableSize, sizeof(MapintNode*));
	mapint->length = 0;
	if(mapint->hashTable == NULL) {
		puts("内存不足，mapint中创建hashTable失败");
		exit(-1);
	}
	return mapint;
}

bool mapint_add(Mapint* mapint, char* key, int value) {
	unsigned int index = getIndex(key, mapint->tableSize);
	//查出桶对应的链表头节点
	MapintNode* node = mapint->hashTable[index];
	//不是空桶
	if(node != NULL) {
		//遍历查找是否存在相同key
		while(1) {
			//存在相同的key，覆盖value
			if(strcmp(key, node->key) == 0) {
				node->value = value;
				mapint->length ++;
				return TRUE;
			}
			//判断是否到达链尾
			if(node->nextNode == NULL) {
				break;
			}
			//节点+1
			node = node->nextNode;
		}
	}
	//链表不存在相同的key，插入新节点到链头
	MapintNode* newNode = (MapintNode*) calloc(1, sizeof(MapintNode));
	newNode->key = key;
	newNode->value = value;
	newNode->nextNode = mapint->hashTable[index];
	mapint->hashTable[index] = newNode;
	mapint->length ++;
	return TRUE;
}

int mapint_get(Mapint* mapint, char* key) {
	//桶号
	unsigned int index = getIndex(key, mapint->tableSize);

	//获得链表头节点地址
	MapintNode* node = mapint->hashTable[index];
	if(node == NULL) {
		return 0;
	}
	//遍历循环
	while(1) {
		//检查key是否相同
		if(strcmp(key, node->key) == 0) {
			return node->value;
		}
		//遍历到尾部，退出循环
		if(node->nextNode == NULL) {
			break;
		}
		//下一个节点
		node = node->nextNode;
	}
	//不存在key
	return 0;
}

bool mapint_delete(Mapint* mapint, char* key) {
	//桶号
	unsigned int index = getIndex(key, mapint->tableSize);

	//获得链表头节点地址
	MapintNode* node = mapint->hashTable[index];
	if(node == NULL) {
		return FALSE;
	}
	//遍历循环
	MapintNode* lastNode = NULL;
	while(1) {
		//检查key是否相同
		if(strcmp(key, node->key) == 0) {
			if(lastNode == NULL) {
				mapint->hashTable[index] = node->nextNode;
			} else {
				lastNode->nextNode = node->nextNode;
			}
			free(node);
			mapint->length --;
			return TRUE;
		}
		//遍历到尾部，退出循环
		if(node->nextNode == NULL) {
			break;
		}
		//下一个节点
		lastNode = node;
		node = node->nextNode;
	}
	//不存在key
	return FALSE;
}


MapintNode* mapint_find(Mapint* mapint, char* key) {
	//桶号
	unsigned int index = getIndex(key, mapint->tableSize);

	//获得链表头节点地址
	MapintNode* node = mapint->hashTable[index];
	if(node == NULL) {
		return NULL;
	}
	//遍历循环
	while(1) {
		//检查key是否相同
		if(strcmp(key, node->key) == 0) {
			return node;
		}
		//遍历到尾部，退出循环
		if(node->nextNode == NULL) {
			break;
		}
		//下一个节点
		node = node->nextNode;
	}
	//不存在key
	return NULL;
}


//返回全部节点,该数组的最后一个元素为NULL,用以标记结束点
MapintNode** mapint_getNodes(Mapint* mapint) {
	MapintNode** nodes = (MapintNode**) calloc(mapint->length + 1, sizeof(MapintNode*));
	int nodeIndex = 0;
	for(int i = 0; i < mapint->tableSize; i++) {
		//遍历链表
		MapintNode* node = mapint->hashTable[i];
		if(node == NULL) {
			continue;
		}
		while(1) {
			nodes[nodeIndex] = node;
			nodeIndex ++;
			MapintNode* nextNode = node->nextNode;
			if(nextNode == NULL) {
				break;
			}
			node = nextNode;
		}
	}
	return nodes;
}

void mapint_show(Mapint* mapint) {
	for(int i = 0; i < mapint->tableSize; i++) {
		//遍历链表
		MapintNode* node = mapint->hashTable[i];
		if(node == NULL) {
			continue;
		}
		while(1) {
			MapintNode* nextNode = node->nextNode;
			printf("%s : %d\n", node->key, node->value);
			if(nextNode == NULL) {
				break;
			}
			node = nextNode;
		}
	}
}

unsigned int hashCode(char* str) {
	unsigned int h = 0;
	for(int i = 0; ; i++) {
		if(str[i] == 0) {
			break;
		}
		h = 31 * h + str[i];
	}
	return h;
}

unsigned int getIndex(char* str, int size) {
	unsigned int hash = hashCode(str);
	return hash & (size - 1);
}

void mapint_free(Mapint* mapint) {
	for(int i = 0; i < mapint->tableSize; i++) {
		//遍历链表
		MapintNode* node = mapint->hashTable[i];
		if(node == NULL) {
			continue;
		}
		while(1) {
			MapintNode* nextNode = node->nextNode;
			free(node);
			if(nextNode == NULL) {
				break;
			}
			node = nextNode;
		}
	}
	free(mapint->hashTable);
}

