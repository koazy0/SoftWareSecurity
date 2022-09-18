//client.cpp
//��ִ��
#define _WINSOCK_DEPRECATED_NO_WARNINGS 1
#define  _CRT_SECURE_NO_WARNINGS 1
#define BUF_SIZE 1024
#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")  //���� ws2_32.dll

#define FILE_PATH_SEND "1.c"
#define FILE_PATH_RECV "2.1.c"

void client() {
	FILE *fp;
	fp = fopen(FILE_PATH_RECV, "w");
	//��ʼ��DLL
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	//�����׽���
	SOCKET sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	SOCKET sock1 = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	//���������������
	struct sockaddr_in sockAddr;		//������ipv4��Ӧ�Ľṹ
	memset(&sockAddr, 0, sizeof(sockAddr));				 //��0���ÿ���ַ�
	sockAddr.sin_family = PF_INET;						 //Э����
	sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");	 //internet address
	sockAddr.sin_port = htons(1234);				     //port in network byte order
														 //��Сβת��Ϊ��β


	connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));
	/*
	�ͻ��˵���connect()���������������������listen()��Ӧ
	*/

	//���շ��������ص�����
	char szBuffer[MAXBYTE] = { 0 };
	int a = recv(sock, szBuffer, MAXBYTE, NULL);
	int b = WSAGetLastError();
	//������յ�������
	printf("Message form server: %s\n", szBuffer);

	char buffer[BUF_SIZE] = { 0 };  //�ļ�������
	int nCount;
	while ((nCount = recv(sock, buffer, BUF_SIZE, 0)) > 0) {
		fwrite(buffer, nCount, 1, fp);
	}

	fclose(fp);

	//�ر��׽���
	closesocket(sock);

	//��ֹʹ�� DLL
	WSACleanup();
	
	system("pause");
}
 
void server() {
	FILE *fp;
	fp = fopen(FILE_PATH_SEND, "r");

	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	//�����׽���
	SOCKET servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	//���׽���
	struct sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));  //ÿ���ֽڶ���0���

	sockAddr.sin_family = PF_INET;  //ʹ��IPv4��ַ
	sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");  //�����IP��ַ
	sockAddr.sin_port = htons(1234);  //�˿�
	bind(servSock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));
	//int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
	//��������


	//�������״̬
	listen(servSock, 20);

	//���տͻ�������
	SOCKADDR clntAddr;
	int nSize = sizeof(SOCKADDR);
	SOCKET clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &nSize);
	//A descriptor that identifies a socket that has been placed in a listening state with the listen function. 
	//The connection is actually made with the socket that is returned by accept.
	//If no error occurs, accept returns a value of type SOCKET that is a descriptor for the new socket.
	//This returned value is a handle for the socket on which the actual connection is made.

	//��ͻ��˷�������
	//char str[] = "Hello World!";
	////int a=send(clntSock, str, strlen(str) + sizeof(char), NULL);
	//int a = send(clntSock, str, strlen(str) + sizeof(char), NULL);
	//int b = WSAGetLastError();

	char buffer[BUF_SIZE] = { 0 };
	int count;
	while ((count = fread(buffer, 1, BUF_SIZE, fp)) > 0) {
		send(clntSock, buffer, count, 0);
	}
	shutdown(clntSock, SD_SEND);  //�ļ���ȡ��ϣ��Ͽ����������ͻ��˷���FIN��
	recv(clntSock, buffer, BUF_SIZE, 0);  //�������ȴ��ͻ��˽������
	fclose(fp);

	//�ر��׽���
	closesocket(clntSock);
	closesocket(servSock);

	//��ֹ DLL ��ʹ��
	WSACleanup();
}

int main() {
	
	client();
	server();
	return 0;
}