//dllmain.cpp:定义DLL应用程序的入口点
#include"pch.h"
#include"dllmain.h"
#pragma comment(lib,"detours.lib")

SYSTEMTIME st;

std::vector<hHeaps> phHeap;
std::vector<Hallocs> pHalloc;
std::vector<event> pevent_heap, pevent_file, pevent_reg, pevent_socket;

//detour函数实现
extern "C" __declspec(dllexport) int WINAPI NewMessageBox(_In_opt_ HWND hWnd, _In_opt_ LPCTSTR lpText, _In_opt_ LPCTSTR lpCaption, _In_ UINT uType) {
	

	printf("\n\n******************************************\n");
	printf("MessageBoxW Hooked\n");
	std::cout << "hWnd: 0x"<<hWnd <<std::endl;
	std::cout << "uType:0x"<<uType <<std::endl;


	int num = WideCharToMultiByte(CP_OEMCP, NULL, lpCaption, -1, NULL, 0, NULL, FALSE);
	LPSTR pchar = new char[num];
	WideCharToMultiByte(CP_OEMCP, NULL, lpCaption, -1, pchar, num, NULL, FALSE);
	std::cout << "lpCaption:"<< pchar <<std::endl;

	num = WideCharToMultiByte(CP_OEMCP, NULL, lpText, -1, NULL, 0, NULL, FALSE);
	pchar = new char[num];
	WideCharToMultiByte(CP_OEMCP, NULL, lpText, -1, pchar, num, NULL, FALSE);
	std::cout << "lpText:"<<pchar <<std::endl;
	GetLocalTime(&st);
	printf("DLL日志输出:%d-%d-%d %02d: %02d: %02d: %03d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	printf("******************************************\n\n");
	return OldMessageBox(NULL, L"Hooked MessageboxW", L"Hooked", MB_OK);
}

//文件操作 OpenFile CreateFile


//堆操作 HeapCreate HeapDestroy HeapAlloc HeapFree
//HeapCreate

extern "C" __declspec(dllexport) HANDLE WINAPI NewHeapCreate(DWORD fIOoptions, SIZE_T dwInitialSize, SIZE_T dwMaximumSize)
{
	hHeaps tmp;
	HANDLE handle;
	handle = OldHeapCreate(fIOoptions, dwInitialSize, dwMaximumSize);
	tmp.heap = handle;
	tmp.flag = 1;
	phHeap.push_back(tmp);	//记录新堆句柄

	printf("\n\n******************************************\n");
	printf("HeapCreate Hooked\n");
	GetLocalTime(&st);
	printf("DLL日志输出:%d-%d-%d %02d: %02d: %02d: %03d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	std::cout << "fIOoptions:0x" << std::hex << fIOoptions << std::endl;
	std::cout << "dwInitialSize:0x" << std::hex << dwInitialSize << std::endl;
	std::cout << "dwMaximumSize:0x" << std::hex << dwMaximumSize << std::endl;
	std::cout << "hHeap:0x" << std::hex << tmp.heap << std::endl;


	if ((fIOoptions & HEAP_CREATE_ENABLE_EXECUTE) != 0) {
		std::cout << "Heap is executable!!!!" << std::endl;	//检测堆是否可执行
	}
	printf("******************************************\n\n");
	Record_event(HEAPCREATE,st, fIOoptions, dwInitialSize, dwMaximumSize, handle);
	return handle;
}
//HeapDestroy

extern "C" __declspec(dllexport) BOOL WINAPI NewHeapDestroy(HANDLE hHeap) {
	printf("\n\n******************************************\n");
	printf("HeapDestroy Hooked\n");
	GetLocalTime(&st);
	printf("DLL日志输出:%d-%d-%d %02d: %02d: %02d: %03d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	std::cout << "hHeap:" << hHeap << std::endl;
	printf("******************************************\n\n");

	for (unsigned int i = 0; i < phHeap.size(); i++)
	{
		if (phHeap[i].heap == hHeap) {
			phHeap[i].flag--;
			if (phHeap[i].flag < 0) {
				//Enable();
				return FALSE;
			}
			break;
		}
	}
	//Enable();
	BOOL result=OldHeapDestroy(hHeap);
	Record_event(HEAPDESTROY,st, hHeap, result);
	return result;
}

//HeapAlloc
extern "C" __declspec(dllexport) LPVOID WINAPI NewHeapAlloc(HANDLE hHeap, DWORD dwFlags, SIZE_T dwBytes) {

	//从记录过的堆找分配函数，排除其他项干扰
	if (!Traced(hHeap)) {		//未记录过的句柄
		return OldHeapAlloc(hHeap, dwFlags, dwBytes);
	}
	printf("\n\n******************************************\n");
	printf("HeapAlloc Hooked\n");
	GetLocalTime(&st);
	printf("DLL日志输出:%d-%d-%d %02d: %02d: %02d: %03d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	std::cout << "hHeap:  0x" << std::hex << hHeap << std::endl;
	std::cout << "dwFlags:0x" << std::hex << dwFlags << std::endl;
	std::cout << "dwBytes:0x" << std::hex << dwBytes << std::endl;
	Hallocs tmp;
	tmp.flag = 1;
	tmp.halloc = OldHeapAlloc(hHeap, dwFlags, dwBytes);
	pHalloc.push_back(tmp);

	//std::cout << "halloc: 0x" <<std::hex<< tmp.halloc << std::endl;
	printf("******************************************\n\n");
	Record_event(HEAPALLOC,st, hHeap, dwFlags, dwBytes, tmp.halloc);
	return tmp.halloc;
}

//HeapFree
extern "C" __declspec(dllexport) BOOL WINAPI NewHeapFree(HANDLE hHeap, DWORD dwFlags, LPVOID lpMem) {
	if (!Traced(hHeap)) {		//未记录过的句柄
		return OldHeapFree(hHeap, dwFlags, lpMem);
	}
	printf("\n\n******************************************\n");
	printf("HeapFree Hooked\n");
	GetLocalTime(&st);
	printf("DLL日志输出:%d-%d-%d %02d: %02d: %02d: %03d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	std::cout << "hHeap:  0x" << std::hex << hHeap << std::endl;
	std::cout << "dwFlags:0x" << std::hex << dwFlags << std::endl;
	std::cout << "dwBytes:0x" << std::hex << hHeap << std::endl;
	printf("******************************************\n\n");

	for (unsigned int i = 0; i < pHalloc.size(); i++)
	{
		if (pHalloc[i].halloc == lpMem) {
			pHalloc[i].flag--;
			if (pHalloc[i].flag < 0) {
				return FALSE;
			}
			break;
		}
	}
	BOOL result= OldHeapFree(hHeap, dwFlags, lpMem);
	Record_event(HEAPFREE,st, hHeap, dwFlags, lpMem, result);
	return result;
}

//socket操作 s
//Server端:accept send Client端:recv conncet  
//其他:closesocket
extern "C" __declspec(dllexport) int WSAAPI Newaccept(SOCKET s, sockaddr * addr, int *addrlen)
{

	printf("\n\n******************************************\n");
	printf("Accept Hooked\n");
	GetLocalTime(&st);
	printf("DLL日志输出:%d-%d-%d %02d: %02d: %02d: %03d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	std::cout << "Socket:0x" << std::hex << s << std::endl;
	std::cout << "socketaddr.sa_family:" << addr->sa_family << std::endl;
	std::cout << "socketaddr.sa_data:" << addr->sa_data << std::endl;
	std::cout << "addrlen:" << *addrlen << std::endl;
	printf("******************************************\n\n");
	return Oldaccept(s, addr, addrlen);
}

//send
extern "C" __declspec(dllexport) int WSAAPI Newsend(SOCKET s, const char  * buf, int len, int flags)
{

	printf("\n\n******************************************\n");
	printf("Send Hooked\n");
	GetLocalTime(&st);
	printf("DLL日志输出:%d-%d-%d %02d: %02d: %02d: %03d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	std::cout << "Socket:0x" << std::hex << s << std::endl;
	std::cout << "buf:" << buf << std::endl;
	std::cout << "len of buf:" << len << std::endl;
	std::cout << "flags:" << flags << std::endl;
	printf("******************************************\n\n");

	return Oldsend(s, buf, len, flags);
}

//closesocket
extern "C" __declspec(dllexport) int WSAAPI Newclosesocket(SOCKET s)
{

	printf("\n\n******************************************\n");
	printf("Closesocket Hooked\n");
	GetLocalTime(&st);
	printf("DLL日志输出:%d-%d-%d %02d: %02d: %02d: %03d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	printf("******************************************\n\n");

	return Oldclosesocket(s);
}
//newrecv
extern "C" __declspec(dllexport) int WSAAPI Newrecv(SOCKET s, char * buf, int len, int flags) {

	printf("\n\n******************************************\n");
	printf("Recv Hooked\n");
	GetLocalTime(&st);
	printf("DLL日志输出:%d-%d-%d %02d: %02d: %02d: %03d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	printf("******************************************\n\n");
	return Oldrecv(s, buf, len, flags);;
}
//connect
extern "C" __declspec(dllexport) int WSAAPI Newconnect(SOCKET s, sockaddr *name, int namelen) {
	printf("\n\nconnect hooked\n\n");
	return Oldconnect(s, name, namelen);
}


void Summary() {
	std::ofstream fout("store.json", std::ios::out);
	for (int i = 0; i < pevent_heap.size(); i++) {
		
		fout << pevent_heap[i].parameter<<"\n";
	}
	fout.close();
}


void Enable() {
	DetourAttach(&(PVOID&)OldMessageBox, NewMessageBox);
	DetourAttach(&(PVOID&)OldHeapCreate, NewHeapCreate);
	DetourAttach(&(PVOID&)OldHeapDestroy, NewHeapDestroy);
	DetourAttach(&(PVOID&)OldHeapAlloc, NewHeapAlloc);
	DetourAttach(&(PVOID&)OldHeapFree, NewHeapFree);
	DetourAttach(&(PVOID&)Oldsend, Newsend);
	DetourAttach(&(PVOID&)Oldaccept, Newaccept);
	DetourAttach(&(PVOID&)Oldclosesocket, Newclosesocket);
	DetourAttach(&(PVOID&)Oldrecv, Newrecv);
	DetourAttach(&(PVOID&)Oldconnect, Newconnect);
}

void Disable() {

	DetourDetach(&(PVOID&)OldMessageBox, NewMessageBox);
	DetourDetach(&(PVOID&)OldHeapCreate, NewHeapCreate);
	DetourDetach(&(PVOID&)OldHeapDestroy, NewHeapDestroy);
	DetourDetach(&(PVOID&)OldHeapAlloc, NewHeapAlloc);
	DetourDetach(&(PVOID&)OldHeapFree, NewHeapFree);
	DetourDetach(&(PVOID&)Oldsend, Newsend);
	DetourDetach(&(PVOID&)Oldaccept, Newaccept);
	DetourDetach(&(PVOID&)Oldclosesocket, Newclosesocket);
	DetourDetach(&(PVOID&)Oldrecv, Newrecv);
	DetourDetach(&(PVOID&)Oldconnect, Newconnect);
}

BOOL Traced(HANDLE handle) {
	for (unsigned int i = 0; i < phHeap.size(); i++) {
		if (phHeap[i].heap == handle) return TRUE;
	}
	return FALSE;
}


extern "C" BOOL WINAPI DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
	
	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH: {


		DisableThreadLibraryCalls(hModule);
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		Enable();
		DetourTransactionCommit();
		break;
	}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH: {

		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		Disable();
		DetourTransactionCommit();
		Summary();
		break;
	}
	}
	return TRUE;
}