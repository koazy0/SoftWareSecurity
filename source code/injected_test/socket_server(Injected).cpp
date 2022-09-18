//server.cpp
//���Գ���injectע������ע��˳���
//Ȼ����ִ��socket_client.cpp������exe

#define _WINSOCK_DEPRECATED_NO_WARNINGS 1
#define  _CRT_SECURE_NO_WARNINGS 1
//error C4996: 'inet_addr': Use inet_pton() or InetPton() instead 
//or define _WINSOCK_DEPRECATED_NO_WARNINGS to disable deprecated API warnings

#include <stdio.h>
#include <winsock2.h>
#pragma comment (lib, "ws2_32.lib")  //���� ws2_32.dll

#define FILE_PATH_SEND "1.c"
#define FILE_PATH_RECV "2.1_test.c"

#define BUF_SIZE 1024


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

	char buffer[BUF_SIZE] = { 0 };  //�ļ�������
	int nCount;
	while ((nCount = recv(sock, buffer, BUF_SIZE, 0)) > 0) {
		fwrite(buffer, nCount, 1, fp);
	}

	fclose(fp);
	
	//������յ�������
	printf("FILE RECV END \n");
	//�ر��׽���
	closesocket(sock);

	//��ֹʹ�� DLL
	WSACleanup();

	system("pause");
}

int main() {

	FILE *fp;
	fp=fopen(FILE_PATH_SEND, "r");


	//��ʼ�� DLL
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
	int d=bind(servSock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));
	//int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
	//��������
	int f = GetLastError();

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
	char str []= "Hello World!";
	//int a=send(clntSock, str, strlen(str) + sizeof(char), NULL);
	int a=send(clntSock, str, strlen(str) + sizeof(char), NULL);
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
	printf("server is over\n");
	system("pause");

	client();
	return 0;
}