/*
 * worker.h
 * redis逻辑处理
 *  Created on: 2020年6月8日
 *      Author: wuxb
 */

#ifndef WORKER_H_
#define WORKER_H_

//命令最大长度
#define MAX_COMMAND_STRLEN 1024
//命令最大参数个数
#define MAX_PARAM_COUNT 4

extern void worker_init();
extern int work(int connection, char commandParam[][MAX_COMMAND_STRLEN], char* returnChars);
extern void worker_exit();

#endif /* WORKER_H_ */
