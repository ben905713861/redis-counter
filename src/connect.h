/*
 * connect.h
 * socket启动入口
 *  Created on: 2020年6月7日
 *      Author: wuxb
 */

#ifndef CONNECT_H_
#define CONNECT_H_

//最大同时阻塞的连接数
#define MAX_BLOCKING_CONNECT 100
//接收客服端发来的最大长度
#define MAX_RECEIVE_SIZE 1024
//返回客户端的最大字符长度
#define MAX_SEND_SIZE 102400

extern void startServer(int port);


#endif /* CONNECT_H_ */
