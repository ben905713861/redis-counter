/*
 * mapint.h
 * 数据结构 - value为int型的哈希map
 *  Created on: 2020年6月8日
 *      Author: wuxb
 */

#ifndef MAPINT_H_
#define MAPINT_H_

typedef unsigned int bool;
#define TRUE 1
#define FALSE 0

#define MAX_KEY_LEN 32

typedef struct MapintNode {
	char key[MAX_KEY_LEN + 1];
	int value;
	struct MapintNode* nextNode;
} MapintNode;

typedef struct {
	int tableSize;
	int length;
	MapintNode** hashTable;
} Mapint;


extern Mapint* mapint_create(int tableSize);
extern bool mapint_add(Mapint* mapint, char* key, int value);
extern int mapint_get(Mapint* mapint, char* key);
extern bool mapint_delete(Mapint* mapint, char* key);
extern MapintNode* mapint_find(Mapint* mapint, char* key);
extern MapintNode** mapint_getNodes(Mapint* mapint);
extern void mapint_show(Mapint* mapint);
extern void mapint_free(Mapint* mapint);

#endif /* MAPINT_H_ */
