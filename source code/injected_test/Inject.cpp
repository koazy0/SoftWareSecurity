#include<iostream>
#include<cstdio>
#include<windows.h>
#include<detours.h>

#define INJECTED L"\\injected"
#define DLL L"\\dll"

#define DLLFILEW L"\\DLLmainW.dll"
#define DLLFILEA L"\\DLLmainA.dll"
#define HEAPFILE L"\\heap.exe"
#define	SOCKETFILE L"\\socket_server_injected.exe"
#define	REGFILE L"\\reg.exe"
#define	MBFILE L"\\app-MB.exe"


#pragma comment(lib,"detours.lib")


int main(int argc, char* argv[]) 	{

	wchar_t Workpath[MAX_PATH], DirPath[MAX_PATH],InjectPath[MAX_PATH];	//目录
	wchar_t Exe[MAX_PATH],Dll[MAX_PATH];								//具体到文件
	char Dll_char[MAX_PATH];

	
	_wgetcwd(Workpath, MAX_PATH);	//获取工作目录C:\Users\koazy-0\source\repos\test\test
	wcscpy_s(DirPath, Workpath);
	wcscpy_s(InjectPath, Workpath);
	lstrcatW(DirPath,DLL);			//dll文件目录
	lstrcatW(InjectPath,INJECTED);	//测试文件目录

	wcscpy_s(Exe,InjectPath);
	wcscpy_s(Dll,DirPath);
	lstrcatW(Exe, HEAPFILE);	//测试文件
	lstrcatW(Dll, DLLFILEW);	//dll文件
	
	
	int size= WideCharToMultiByte(CP_ACP, 0, Dll, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, Dll, -1, Dll_char, size, NULL, NULL);
	

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));

	si.cb = sizeof(STARTUPINFO);


	//DLL的文件夹

	if (DetourCreateProcessWithDllEx(Exe, NULL, NULL, NULL, TRUE,
		CREATE_DEFAULT_ERROR_MODE | CREATE_SUSPENDED,
		NULL, DirPath, &si, &pi, Dll_char, NULL))
	{
		//MessageBoxA(NULL,"inject","inject",NULL);
		ResumeThread(pi.hThread);
		WaitForSingleObject(pi.hProcess, INFINITE);
	}

	else
	{
		char error[100];
		sprintf_s(error, "%d", GetLastError());//MessageboxA(NULL,error,NULL,NULL);
	}

	MessageBox(NULL, L"Work Over!!", L"test",MB_OK);
	system("pause");
	return 0;
}
