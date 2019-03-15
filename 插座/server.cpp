#include "pch.h"
#include <iostream>
//1 引入头文件，导入静态库
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")

int main()
{
	//	2获取协议版本
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,2),&wsaData);
	if (HIBYTE(wsaData.wVersion) != 2 || LOBYTE(wsaData.wVersion) != 2) {
		printf("请求版本失败!\n");
		return -1;
	}
	printf("请求版本成功!\n");

	//	3创建socket		         协议格式 数据格式 保护模式
	SOCKET serverSocket = socket(AF_INET,SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == serverSocket) {
		printf("创建socket失败!\n");
		return -1;
	}
	printf("创建socket成功!\n");
	//	4创建协议地址族		
	SOCKADDR_IN addr = { 0 };
	addr.sin_family= AF_INET;//必须和socket函数的第一个参数保持一致
	addr.sin_addr.S_un.S_addr= inet_addr("127.0.0.1");//ip地址 字符串和4个字节
	addr.sin_port = htons(10086);//端口
	//	5绑定
	int r = bind(serverSocket,(SOCKADDR *)&addr,sizeof(addr));
	if (r == -1) {
		printf("bind失败!\n");
		return -1;
	}
	printf("bind成功!\n");
	//	6监听
	r = listen(serverSocket, 10);
	if (r == -1) {
		printf("listen失败!\n");
		return -1;
	}
	printf("listen成功!\n");
	//	7等待客户端连接服务器
	SOCKADDR_IN cAddr = {0};
	int len = sizeof(cAddr);
	SOCKET clientSocket = accept(serverSocket,(SOCKADDR *)&cAddr,&len);
	if (clientSocket == SOCKET_ERROR) {
		printf("客户端错误!\n");
		WSACleanup();
		closesocket(serverSocket);
		return -1;
	}
	printf("有客户端连接到服务器了：%s\n", inet_ntoa(cAddr.sin_addr));
	//	8通信	
	char buff[256];
	while (1) {
		r = recv(clientSocket, buff, 255, NULL);
		if (r > 0) {
			buff[r] = '\0';
			printf(">>%s\n", buff);
		}
	}
	//	9关闭socket				
	closesocket(serverSocket);
	closesocket(clientSocket);
	//	10清除版本信息				
	WSACleanup();

}
