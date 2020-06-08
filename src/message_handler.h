/*
 * message_handler.h
 * 消息接收处理
 *  Created on: 2020年6月7日
 *      Author: wuxb
 */

#ifndef MESSAGE_HANDLER_H_
#define MESSAGE_HANDLER_H_

typedef unsigned int bool;
#define TRUE 1
#define FALSE 0

//最大连接数
#define MAX_CONNECTION 1024

extern void message_handler_init();
extern int message_action(int connection, char* msg, char* returnChars);
extern void handle_clear(int connection);
extern void message_handler_exit();

#endif /* MESSAGE_HANDLER_H_ */
