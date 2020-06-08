/*
 * message_handler.c
 *
 *  Created on: 2020年6月7日
 *      Author: wuxb
 */

#include "message_handler.h"
#include "worker.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char connection2tempcommand[MAX_CONNECTION][MAX_COMMAND_STRLEN + 1] = {0};


static int decode1(char* command, char commandParam[][MAX_COMMAND_STRLEN]);
static int decode2(char* command, int* startIndex, char* decodeCommand);


void message_handler_init() {
	worker_init();
}

void message_handler_exit() {
	worker_exit();
}

int message_action(int connection, char* command, char* returnChars) {
	char* tempcommand = connection2tempcommand[connection];
	//将命令字符拼接到历史命令上
	if(strlen(tempcommand) + strlen(command) > MAX_COMMAND_STRLEN) {
		puts("接参超长了");
		return -1;
	}
	strcat(tempcommand, command);
	
	//解析好的命令
	char commandParam[MAX_PARAM_COUNT][MAX_COMMAND_STRLEN] = {0};
	int res = decode1(tempcommand, commandParam);
	if(res == 1) {
		puts("本次发送数据不全,暂停解码");
		strcpy(returnChars, "");
	} else if(res == 0) {
		res = work(connection, commandParam, returnChars);
		//清掉command缓存
		handle_clear(connection);
		//参数错误,需告诉connect.c中断连接
		if(res != 0) {
			puts(returnChars);
			return -2;
		}
		
	} else {
		//出现异常，需告诉connect.c中断连接
		handle_clear(connection);
		//清掉command缓存
		tempcommand[0] = '\0';
		return -2;
	}
	return 0;
}

bool checkOneLine(char* command, int startIndex) {
	bool isLine = FALSE;
	int commandLen = strlen(command);
	for(int i = startIndex; i < commandLen - 1; i++) {
		if(command[i] == '\r' && command[i+1] == '\n') {
			isLine = TRUE;
			break;
		}
	}
	return isLine;
}

int decode1(char* command, char commandParam[][MAX_COMMAND_STRLEN]) {
	if(checkOneLine(command, 0) == FALSE) {
		puts("没有接收够一行数据,退出1");
		return 1;
	}
	if(command[0] != '*') {
		puts("不能识别的命令头，第一位必须是*");
		return -1;
	}
	if(strlen(command) <= 3) {
		puts("*后没有数字");
		return -2;
	}
	
	//命令游标
	int startIndex = 0;
	//数组长度
	int arrayLength = 0;
	while(1) {
		startIndex ++;
		if(command[startIndex] == '\r') {
			continue;
		}
		if(command[startIndex-1] == '\r' && command[startIndex] == '\n') {
			break;
		}
		//char数字字符转int数字
		int lentemp = command[startIndex] - '0';
		if(lentemp < 0 || lentemp > 9) {
			//不是合法数字,异常，需要断开连接
			puts("*后不是合法数字");
			return -3;
		}
		arrayLength = arrayLength * 10 + lentemp;
	}
	if(arrayLength == 0) {
		puts("*后数字不得为0");
		return -4;
	}
	if(arrayLength > MAX_PARAM_COUNT) {
		puts("传参参数个数过多了");
		return -5;
	}
	
	//解码子元素
	for(int i = 0; i < arrayLength; i++) {
		int res = decode2(command, &startIndex, commandParam[i]);
		if(res == 0) {
			continue;
		}
		return res;
	}
	
	return 0;
}


int decode2(char* command, int* startIndex, char* decodeCommand) {
	(*startIndex) += 1;
	
	if(checkOneLine(command, *startIndex) == FALSE) {
		puts("没有接收够一行数据,退出2");
		return 1;
	}
	if(command[*startIndex] != '$') {
		puts("不能识别的命令头，第一位必须是$");
		return -1;
	}
	int commandLen = strlen(command);
	if(commandLen <= 3) {
		puts("$后没有数字");
		return -2;
	}
	
	int length = 0;
	while(1) {
		(*startIndex) ++;
		if(command[*startIndex] == '\r') {
			continue;
		}
		if(command[(*startIndex)-1] == '\r' && command[*startIndex] == '\n') {
			break;
		}
		//char数字字符转int数字
		int lentemp = command[*startIndex] - '0';
		if(lentemp < 0 || lentemp > 9) {
			//不是合法数字,异常，需要断开连接
			puts("$后不是合法数字");
			return -3;
		}
		length = length * 10 + lentemp;
	}
	//接下来要接的字符串少于定义的长度,退出不处理
	if(commandLen - ((*startIndex)+1) < length + 2) {
		puts("没有接收够一行数据,退出");
		return 1;
	}
	for(int i = 0; i < length; i++) {
		(*startIndex) ++;
		decodeCommand[i] = command[*startIndex];
	}
	decodeCommand[length] = '\0';
	(*startIndex) += 2;
	
	return 0;
}


void handle_clear(int connection) {
	connection2tempcommand[connection][0] = '\0';
}
