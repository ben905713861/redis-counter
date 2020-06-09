/*
 * connect.c
 *
 *  Created on: 2020年6月7日
 *      Author: wuxb
 */

#include "connect.h"
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h> 
#include "listint.h"
#include "message_handler.h"

int listenfd;
//连接池
Listint* connectionSet;
//连接描述符集合
fd_set fds;


static void listening();
static void exitListening(int sig);
static void connection_clear(int connection);

void startServer(int port) {
	//连接池初始化
	connectionSet = listint_create();
	//消息处理器初始化
	message_handler_init();
	
	//创建 IPv4协议的字节流套接字,若成功则返回一个套接字描述符
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if (listenfd == -1) {
		perror("create socket error");
		exit(-5);
	}
	
	int flags = fcntl(listenfd, F_GETFL);
	fcntl(listenfd, F_SETFL, flags | O_NONBLOCK);

	//一般是储存地址和端口，用于信息的显示及存储作用
	struct sockaddr_in serv_addr;
	//IPv4协议
	serv_addr.sin_family = AF_INET;
	//将端口号转换为网络字节序，即大端模式
	serv_addr.sin_port = htons(port);
	//将主机无符号长整型数转换成网络字节顺序。INADDR_ANY就是指定地址为0.0.0.0的地址，这个地址事实上表示不确定地址，或"所有地址"、“任意地址”
	serv_addr.sin_addr.s_addr = INADDR_ANY;

	if(bind(listenfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1) {
		perror("bind socket error");
		exit(-2);
	}

	//listen第二个参数连接请求队列数等等
	if(listen(listenfd, MAX_BLOCKING_CONNECT) == -1) {
		perror("listen socket error\n");
		exit(-3);
	}
	
	//注册退出事件
	signal(SIGINT, exitListening);
	signal(SIGTERM, exitListening);
	//监听
	listening();
}


void listening() {
	int maxsock = listenfd;
	//select超时时间
	struct timeval timeout;
	
	printf("======waiting for client's request======\n");
	while (1) {
		//每次循环都要清空集合，否则不能检测描述符变化
		FD_ZERO(&fds);
		//添加描述符用于监听socket端口的accept是否有新connection进来
		FD_SET(listenfd, &fds);
		//添加描述符用于监听每个connection的recv是否有数据流进来
		
		ListintIterator* iterator = listint_iterator(connectionSet);
		while(listint_iterator_hasNext(iterator)) {
			ListintNode* node = listint_iterator_next(iterator);
			int connection = node->value;
			FD_SET(connection, &fds);
		}
		listint_iterator_free(iterator);
		
		//每次轮询都要重新设置超时时间
		timeout.tv_sec = 30;
		timeout.tv_usec = 0;
		//同时监听所有的连接是否来信息和是否有新连接产生
		int ret = select(maxsock+1, &fds, NULL, NULL, &timeout);
		if(ret < 0) {
			//侦听到程序退出信号
			if(errno == EINTR) {
				return;
			}
			perror("select错误");
			exit(-4);
		}
		//轮询等待超时,跳入下一次循环
		else if(ret == 0) {
			continue;
		}
		
		//对所有连接遍历,找出其中有数据流的连接,然后接收数据
		char msg[MAX_RECEIVE_SIZE];
		
		iterator = listint_iterator(connectionSet);
		while(listint_iterator_hasNext(iterator)) {
			ListintNode* node = listint_iterator_next(iterator);
			int connection = node->value;
			if(!FD_ISSET(connection, &fds)) {
				continue;
			}
			//接收数据流
			ret = recv(connection, msg, sizeof(msg), 0);
			//返回长度为0，意味着客户端断开连接，这时服务端也可以断开连接并从连接池中去除该连接
			if(ret <= 0) {
//				printf("客户端断开连接\n");
				connection_clear(connection);
				listint_delete(connectionSet, node);
				continue;
			}
			msg[ret] = '\0';
			
			//需要向客户端返回的字符串
			char returnChars[MAX_SEND_SIZE];
			returnChars[0] = '\0';
			int res = message_action(connection, msg, returnChars);
			if(res != 0) {
				connection_clear(connection);
				listint_delete(connectionSet, node);
//				puts("服务器断开连接");
				continue;
			}
			
			//响应数据流
			send(connection, returnChars, strlen(returnChars), 0);
		}
		listint_iterator_free(iterator);
		
		
		//当有新连接到来时
		struct sockaddr_in client_addr;
		socklen_t sizeof_client_addr = sizeof(client_addr);
		if(FD_ISSET(listenfd, &fds)) {
			int connection = accept(listenfd, (struct sockaddr*) &client_addr, &sizeof_client_addr);
			if(connection <= 0) {
				perror("accept错误");
				continue;
			}
			//加入连接池
			listint_add(connectionSet, connection);
			//这里很重要，一定要判断最大值，因为重新分配的connection值有可能是已经释放过的历史值
			maxsock = connection > maxsock ? connection : maxsock;
//			printf("发现新连接\n");
		}
	}
}


void connection_clear(int connection) {
	close(connection);
	FD_CLR(connection, &fds);
	//清除meeeage_handelr.c中的缓存
	handle_clear(connection);
}



void exitListening(int sig) {
	//退出程序
	ListintIterator* iterator = listint_iterator(connectionSet);
	while(listint_iterator_hasNext(iterator)) {
		ListintNode* node = listint_iterator_next(iterator);
		int connection = node->value;
		connection_clear(connection);
	}
	listint_iterator_free(iterator);
	listint_free(connectionSet);
	close(listenfd);
	message_handler_exit();
	printf("程序正常结束\n");
}

