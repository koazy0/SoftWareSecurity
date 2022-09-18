#pragma once

#include "pch.h"
#include<vector>
#include"framework.h"
#include"detours.h"
#include <fstream>
#include"stdio.h"
#include<iostream>
#include"record.h"
#pragma comment (lib, "ws2_32.lib")

struct hHeaps
{
	HANDLE  heap;
	int flag;
};

struct Hallocs {
	LPVOID halloc;
	int flag;
};

//定义和引入需要Hook的函数，和替换的函数
//msgbox
static int(WINAPI *OldMessageBox)(_In_opt_ HWND hWnd, _In_opt_ LPCTSTR lpText, _In_opt_ LPCTSTR lpCaption, _In_ UINT uType) = MessageBox;

//堆操作
static HANDLE(WINAPI *OldHeapCreate)(DWORD fIOoptions, SIZE_T dwInitialSize, SIZE_T dwMaximumSize) = HeapCreate;
static BOOL(WINAPI*OldHeapDestroy)(HANDLE) = HeapDestroy;
static LPVOID(WINAPI *	OldHeapAlloc)(_In_ HANDLE hHeap, _In_ DWORD dwFlags, _In_ SIZE_T dwBytes) = HeapAlloc;
static BOOL(WINAPI*	OldHeapFree)(HANDLE hHeap, DWORD dwFlags, LPVOID lpMem) = HeapFree;

//socket操作
static SOCKET(WSAAPI * Oldaccept)(SOCKET s, sockaddr * addr, int *addrlen) = accept;
static int(WSAAPI * Oldsend)(SOCKET s, const char  * buf, int len, int flags) = send;
static int(WSAAPI*Oldrecv)	(SOCKET s, char FAR * buf, int len, int flags) = recv;
static int(WSAAPI *Oldclosesocket)(SOCKET s) = closesocket;
static int(WSAAPI *Oldconnect)(SOCKET s, const sockaddr *name, int namelen) = connect;

BOOL Traced(HANDLE handle);
void Enable();
void Disable();