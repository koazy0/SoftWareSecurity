#pragma once
#define _CRT_SECURE_NO_WARNINGS 1

#include<WinSock2.h>
#include<Windows.h>
#include"nlohmann/json.hpp"
#define DEBUG_S

#ifdef DEBUG
#define HEAPCREATE 0x1
#define HEAPDESTROY 0x2
#define HEAPALLOC 0x4
#define HEAPFREE 0x8


extern BOOL Record_event(int fun, SYSTEMTIME st, DWORD fIOoptions, SIZE_T dwInitialSize, SIZE_T dwMaximumSize, HANDLE returned_Handle);
extern BOOL Record_event(int fun, SYSTEMTIME st, HANDLE handle, BOOL returned_Bool);
extern BOOL Record_event(int fun, SYSTEMTIME st, HANDLE hHeap, DWORD dwFlags, SIZE_T dwBytes, LPVOID returned_Lpvoid);
extern BOOL Record_event(int fun, SYSTEMTIME st, HANDLE hHeap, DWORD dwFlags, LPVOID lpMem, BOOL returned_Bool);

extern BOOL Record_event(int fun, SYSTEMTIME st, SOCKET s, sockaddr * addr, int *addrlen, SOCKET returned_Socket);
extern BOOL Record_event(int fun, SYSTEMTIME st, SOCKET s, const char * buf, int len, int flags, int returned_Count);
extern BOOL Record_event(int fun, SYSTEMTIME st, SOCKET s, char FAR * buf, int len, int flags, int returned_Count);
extern BOOL Record_event(int fun, SYSTEMTIME st, SOCKET s, int returned_Int);
extern BOOL Record_event(int fun, SYSTEMTIME st, SOCKET s, const sockaddr *name, int namelen, int returned_Int);

#else
#define HEAPCREATE "HeapCreate"
#define HEAPDESTROY "heapDestroy"
#define HEAPALLOC "HeapAlloc"
#define HEAPFREE "HeapFree"

extern BOOL Record_event(const char* fun, SYSTEMTIME st, DWORD fIOoptions, SIZE_T dwInitialSize, SIZE_T dwMaximumSize, HANDLE returned_Handle);
extern BOOL Record_event(const char* fun, SYSTEMTIME st, HANDLE handle, BOOL returned_Bool);
extern BOOL Record_event(const char* fun, SYSTEMTIME st, HANDLE hHeap, DWORD dwFlags, SIZE_T dwBytes, LPVOID returned_Lpvoid);
extern BOOL Record_event(const char* fun, SYSTEMTIME st, HANDLE hHeap, DWORD dwFlags, LPVOID lpMem, BOOL returned_Bool);

extern BOOL Record_event(const char* fun, SYSTEMTIME st, SOCKET s, sockaddr * addr, int *addrlen, SOCKET returned_Socket);
extern BOOL Record_event(const char* fun, SYSTEMTIME st, SOCKET s, const char * buf, int len, int flags, int returned_Count);
extern BOOL Record_event(const char* fun, SYSTEMTIME st, SOCKET s, char FAR * buf, int len, int flags, int returned_Count);
extern BOOL Record_event(const char* fun, SYSTEMTIME st, SOCKET s, int returned_Int);
extern BOOL Record_event(const char* fun, SYSTEMTIME st, SOCKET s, const sockaddr *name, int namelen, int returned_Int);


#endif // DEBUG

#define _HEAP 0xf
#define _FILE 0xf0
#define _REG 0xf00
#define _SOCKET 0xf000


using json = nlohmann::json;

struct event
{
	//int fun_i;
	char fun[MAX_PATH];
	json parameter;
	SYSTEMTIME st;
};

