//server.cpp
//测试程序，inject注入首先注入此程序
//然后再执行socket_client.cpp编译后的exe

#define _WINSOCK_DEPRECATED_NO_WARNINGS 1
#define  _CRT_SECURE_NO_WARNINGS 1
//error C4996: 'inet_addr': Use inet_pton() or InetPton() instead 
//or define _WINSOCK_DEPRECATED_NO_WARNINGS to disable deprecated API warnings

#include <stdio.h>
#include <winsock2.h>
#pragma comment (lib, "ws2_32.lib")  //加载 ws2_32.dll

#define FILE_PATH_SEND "1.c"
#define FILE_PATH_RECV "2.1_test.c"

#define BUF_SIZE 1024


void client() {

	FILE *fp;
	fp = fopen(FILE_PATH_RECV, "w");
	//初始化DLL
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	//创建套接字
	SOCKET sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	SOCKET sock1 = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	//向服务器发起请求
	struct sockaddr_in sockAddr;		//这里是ipv4对应的结构
	memset(&sockAddr, 0, sizeof(sockAddr));				 //用0清除每个字符
	sockAddr.sin_family = PF_INET;						 //协议族
	sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");	 //internet address
	sockAddr.sin_port = htons(1234);				     //port in network byte order
														 //从小尾转化为大尾


	connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));

	char buffer[BUF_SIZE] = { 0 };  //文件缓冲区
	int nCount;
	while ((nCount = recv(sock, buffer, BUF_SIZE, 0)) > 0) {
		fwrite(buffer, nCount, 1, fp);
	}

	fclose(fp);
	
	//输出接收到的数据
	printf("FILE RECV END \n");
	//关闭套接字
	closesocket(sock);

	//终止使用 DLL
	WSACleanup();

	system("pause");
}

int main() {

	FILE *fp;
	fp=fopen(FILE_PATH_SEND, "r");


	//初始化 DLL
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	//创建套接字
	SOCKET servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	//绑定套接字
	struct sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));  //每个字节都用0填充
	
	sockAddr.sin_family = PF_INET;  //使用IPv4地址
	sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");  //具体的IP地址
	sockAddr.sin_port = htons(1234);  //端口
	int d=bind(servSock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));
	//int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
	//绑定描述字
	int f = GetLastError();

	//进入监听状态
	listen(servSock, 20);

	//接收客户端请求
	SOCKADDR clntAddr;
	int nSize = sizeof(SOCKADDR);
	SOCKET clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &nSize);
	//A descriptor that identifies a socket that has been placed in a listening state with the listen function. 
	//The connection is actually made with the socket that is returned by accept.
	//If no error occurs, accept returns a value of type SOCKET that is a descriptor for the new socket.
	//This returned value is a handle for the socket on which the actual connection is made.

	//向客户端发送数据
	char str []= "Hello World!";
	//int a=send(clntSock, str, strlen(str) + sizeof(char), NULL);
	int a=send(clntSock, str, strlen(str) + sizeof(char), NULL);
	char buffer[BUF_SIZE] = { 0 };
	int count;
	while ((count = fread(buffer, 1, BUF_SIZE, fp)) > 0) {
		send(clntSock, buffer, count, 0);
	}
	shutdown(clntSock, SD_SEND);  //文件读取完毕，断开输出流，向客户端发送FIN包
	recv(clntSock, buffer, BUF_SIZE, 0);  //阻塞，等待客户端接收完毕
	fclose(fp);

	//关闭套接字
	closesocket(clntSock);
	closesocket(servSock);
	
	//终止 DLL 的使用
	WSACleanup();
	printf("server is over\n");
	system("pause");

	client();
	return 0;
}