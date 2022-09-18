
#include"pch.h"
#include"record.h"



extern std::vector<event> pevent_heap, pevent_file, pevent_reg, pevent_socket;

//Heap
BOOL Record_event(const char* fun, SYSTEMTIME st, DWORD fIOoptions, SIZE_T dwInitialSize, SIZE_T dwMaximumSize, HANDLE returned_Handle) {
	event tmp;
	//strcpy_s(tmp.fun, sizeof(fun), fun);
	//tmp.fun = fun;
	
	tmp.st = st;
	tmp.parameter["fun"] = fun;
	tmp.parameter["fIOoptions"] = fIOoptions;
	tmp.parameter["dwInitialSize"] = dwInitialSize;
	tmp.parameter["dwMaximumSize"] = dwMaximumSize;
	tmp.parameter["returned_Handle"] = (int)returned_Handle;	//“‘int–Œ Ω¥Ê¥¢
	pevent_heap.push_back(tmp);
	return TRUE;
}
BOOL Record_event(const char* fun, SYSTEMTIME st, HANDLE handle, BOOL returned_Bool) {
	event tmp;
	//strcpy_s(tmp.fun, sizeof(fun),fun);
	//tmp.fun = fun;
	
	tmp.st = st;
	tmp.parameter["fun"] = fun;
	tmp.parameter["handle"] = (int)handle;
	tmp.parameter["returned_Bool"] = returned_Bool;
	pevent_heap.push_back(tmp);
	return TRUE;
}
BOOL Record_event(const char* fun, SYSTEMTIME st, HANDLE hHeap, DWORD dwFlags, SIZE_T dwBytes, LPVOID returned_Lpvoid) {
	event tmp;
	//strcpy_s(tmp.fun, sizeof(fun),fun);
	//tmp.fun = fun;
	
	tmp.st = st;
	tmp.parameter["fun"] = fun;
	tmp.parameter["hHeap"] = (int)hHeap;
	tmp.parameter["dwFlags"] = dwFlags;
	tmp.parameter["dwBytes"] = dwBytes;
	tmp.parameter["returned_Lpvoid"] = (int)returned_Lpvoid;
	pevent_heap.push_back(tmp);
	return TRUE;
}
BOOL Record_event(const char* fun, SYSTEMTIME st,HANDLE hHeap, DWORD dwFlags, LPVOID lpMem, BOOL returned_Bool) {
	event tmp;
	//strcpy_s(tmp.fun, sizeof(fun),fun);
	//tmp.fun = fun;
	
	tmp.st = st;
	tmp.parameter["fun"] = fun;
	tmp.parameter["hHeap"] = (int)hHeap;
	tmp.parameter["dwFlags"] = dwFlags;
	tmp.parameter["lpMem"] = (int)lpMem;
	tmp.parameter["returned_Bool"] = returned_Bool;
	pevent_heap.push_back(tmp);
	return TRUE;
}

//Socket
BOOL Record_event(const char* fun, SYSTEMTIME st,SOCKET s, sockaddr * addr, int *addrlen, SOCKET returned_Socket) {
	event tmp;
	//strcpy_s(tmp.fun, sizeof(fun),fun);
	//tmp.fun = fun;
	
	tmp.st = st;
	tmp.parameter["fun"] = fun;
	tmp.parameter["Socket"] = s;
	tmp.parameter["addr"] = (int)addr;
	tmp.parameter["addrlen"] = *addrlen;
	tmp.parameter["returned_Socket"] = returned_Socket;
	pevent_socket.push_back(tmp);
	return TRUE;
}
BOOL Record_event(const char* fun, SYSTEMTIME st,SOCKET s, const char * buf, int len, int flags, int returned_Count) {
	event tmp;
	//strcpy_s(tmp.fun, sizeof(fun),fun);
	//tmp.fun = fun;
	
	tmp.st = st;
	tmp.parameter["fun"] = fun;
	tmp.parameter["buf"] = *buf;
	tmp.parameter["len"] = len;
	tmp.parameter["flags"] = flags;
	tmp.parameter["returned_Count"] = returned_Count;
	pevent_socket.push_back(tmp);
	return TRUE;
}
BOOL Record_event(const char* fun, SYSTEMTIME st,SOCKET s, char  * buf, int len, int flags, int returned_Count) {
	event tmp;
	//strcpy_s(tmp.fun, sizeof(fun),fun);
	//tmp.fun = fun;
	
	tmp.parameter["fun"] = fun;
	tmp.parameter["buf"] = *buf;
	tmp.parameter["len"] = len;
	tmp.parameter["flags"] = flags;
	tmp.parameter["returned_Count"] = returned_Count;
	pevent_socket.push_back(tmp);
	return TRUE;
}
BOOL Record_event(const char* fun, SYSTEMTIME st,SOCKET s, int returned_Int) {
	event tmp;
	//strcpy_s(tmp.fun, sizeof(fun),fun);
	//tmp.fun = fun;
	
	tmp.st = st;
	tmp.parameter["fun"] = fun;
	tmp.parameter["Socket"] = s;
	tmp.parameter["returned_Int"] = returned_Int;
	pevent_socket.push_back(tmp);
	return TRUE;
}
BOOL Record_event(const char* fun, SYSTEMTIME st,SOCKET s, const sockaddr *name, int namelen, int returned_Int) {
	event tmp;
	//strcpy_s(tmp.fun, sizeof(fun),fun);
	//tmp.fun = fun;
	
	tmp.st = st;
	tmp.parameter["fun"] = fun;
	tmp.parameter["name"] = s;
	tmp.parameter["sockaddr*"] = (int)name;
	tmp.parameter["namelen"] = namelen;
	tmp.parameter["returned_Int"] = returned_Int;
	pevent_socket.push_back(tmp);
	return TRUE;
}