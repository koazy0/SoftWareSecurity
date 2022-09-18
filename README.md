# SoftWareSecurity
（ '▿ ' ）💧
## 1.文件说明



文件结构如下

![image-20220918163359952](pic\image-20220918163359952.png)

- dll：存放dll的文件夹
- injected：包含样本程序
- pic：本文档的图片
- source code:包含注入dll和其他程序的源码
- example:任务书上的源代码
- injected_test:注射器及样本程序的源码





![image-20220918162329037](pic\image-20220918162329037.png)

## 2.代码

## 3.其他代码解释

### 3.1 detour库

~~~c++
LONG WINAPI DetourAttach(_Inout_ PVOID *ppPointer,_In_ PVOID pDetour);

/*
DetourAttach(&(PVOID&)OldCreateFileW, NewCreateFile);
ppPointer : Pointer to the target pointer to which the detour will be attached. See the remarks section below for additional considerations.

pDetour : Pointer to the detour function.
*/

LONG DetourTransactionBegin(VOID);

/*
Returns NO_ERROR if successful; otherwise returns ERROR_INVALID_OPERATION.
ERROR_INVALID_OPERATION : A pending transaction already exists. //挂起事务已存在
*/

LONG DetourTransactionCommit(VOID);

/*
Returns NO_ERROR if successful; otherwise, returns an error code.
ERROR_INVALID_DATA : Target function was changed by third party between steps of the transaction.

ERROR_INVALID_OPERATION : No pending transaction exists.

Other : Error code returned by API within DetourAttach, DetourAttachEx, or DetourDetach that caused transaction to fail.
*/
~~~



### 3.2 注册表相关

~~~c++
LSTATUS RegCreateKeyA(
  [in]           HKEY   hKey,
  [in, optional] LPCSTR lpSubKey,
  [out]          PHKEY  phkResult
);
/*
If the function succeeds, the return value is ERROR_SUCCESS.

*/
LSTATUS RegSetValueA(
  [in]           HKEY   hKey,
  [in, optional] LPCSTR lpSubKey,
  [in]           DWORD  dwType,
  [in]           LPCSTR lpData,
  [in]           DWORD  cbData
);
/*
If the function succeeds, the return value is ERROR_SUCCESS.
[in, optional] lpSubKey:Key names are not case sensitive.
[in] dwType:This parameter must be the REG_SZ type
[in] lpData:The data to be stored. This parameter cannot be NULL.
[in] cbData:lpData的大小。
*/
~~~

### 3.3 堆相关

~~~c++
//堆创建
HANDLE HeapCreate( [in] DWORD  flOptions,  [in] SIZE_T dwInitialSize,  [in] SIZE_T dwMaximumSize);
/*
flOptions:标志堆属性:
		HEAP_NO_SERIALIZE：对堆的访问是非独占的，如果一个线程没有完成对堆的操作，其它线程也可以进程堆操作，这个开关是非常危险的，应尽量避免使用。
		HEAP_GENERATE_EXCEPTIONS：当堆分配内存失败时，会抛出异常。如果不设置，则返回NULL
		HEAP_CREATE_ENALBE_EXECUTE：堆中存放的内容是可以执行的代码。如果不设置，意味着堆中存放的是不可执行的数据(堆喷射)
dwInitialSize:
*/
~~~

### 3.4 socket编程

~~~c++
//相关结构体
struct SOCKADDR {
unsigned short sa_family; /* address family, AF_xxx */
char sa_data[14]; 		  /* 14 bytes of protocol address */
};

/*
	sa_family:地址族AF_INET，AF_INET6和AF_UNSPEC
	sa_data:返回协议的地址
*/


//相关函数

SOCKET clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &nSize);
~~~

![sockaddr示意](pic\image-20220915132514235.png)

### 3.5 相关结构体

~~~c
struct event_file{
    
}

struct event_heap{
    
}
struct event_socket{
    
}
~~~



## 4.相关知识补充

### 4.1 _T宏

​	`_T("")` 是一个宏，定义于 `tchar.h`文件中, 作用是将字符串转化为Unicode编码。在C中默认字符串使用ANSI进行处理，如

`char *str="ABC";`为了保存中文或者其他文件，则需要转化为Unicode，可以使用`_T"ABC"` 或者`L"ABC"`的方式来表示Unicode编码, 但是，它们也存在区别。

- L: 一定使用UNICODE进行编码

- T():根据编译条件的设置来选择编码，vs选项在     工程属性-常规-项目默认值-字符集     里面设置。

  ![image-20220907090628176](pic\image-20220907090628176.png)

### 4.2 ANSI和Unicode

Unicode 是 ANSI的超集。

### 4.3 LPCTSTR,LPCSTR,LPCWSTR等

在将题目的类型之前，我们需要了解char,tchar,wchar_t的区别。具体如下：https://blog.csdn.net/h_wlyfw/article/details/20443531

~~~c++
//三种char类型的区别
char str1[5]="陈伟";			//const char[5]
TCHAR str2[3]=L"陈伟";		//const wchar_t[3]
wchar_t str3[3] = L"陈伟";	//const wchar_t[3]
~~~

- LPCTSTR

~~~c
    //L表示long指针 这是为了兼容Windows 3.1等16位操作系统遗留下来的。没有实际意义。
    //P表示这是一个指针
    //C表示是一个常量
    //T表示在Win32环境中， 有一个_T宏
    //STR表示这个变量是一个字符串
	//这个宏用来表示你的字符是否使用UNICODE, 如果你的程序定义了UNICODE或者其他相关的宏，那么这个字符或者字符串将被作为UNICODE字符串，否则就是标准的ANSI字符串。所以LPCTSTR就表示一个指向常固定地址的可以根据一些宏定义改变语义的字符串。
~~~

- LPCWSTR  LPCSTR 与上方对应

### 4.4 注入调试

### 总结

​		所以源码中的messageboxA，W可以有如下形式

​		如果后缀有W(如MessageBoxW), 则用LPCWSTR

~~~assembly
static int(WINAPI *OldMessageBoxW)(_In_opt_ HWND hWnd, _In_opt_ LPCWSTR lpText, _In_opt_ LPCWSTR lpCaption, _In_ UINT uType) = MessageBoxW;
static int(WINAPI *OldMessageBoxA)(_In_opt_ HWND hWnd, _In_opt_ LPCSTR lpText, _In_opt_ LPCSTR lpCaption, _In_ UINT uType) = MessageBoxA;
static int (WINAPI *OldMessageBox) (_In_opt_ HWND hWnd, _In_opt_ LPCTSTR lpText, _In_opt_ LPCTSTR lpCaption, _In_ UINT uType)= MessageBox;
~~~

### 4.5 json

~~~json
var event='{"event":"File","func":"CreateFile",parameter":{
		"lpfilename":"test_filename",
		"dwDesiredAccess":"",
		"dwShareMode":,
		"lpSecurityAttributes":,
		"dwCreationDisposition":,
		"dwFlagsAndAttributes":,
		"hTemplate":""
	}


}';
~~~





## * 遇到的其他错误及问题

### 1.dll的入口函数

​		是`BOOL WINAPI DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved);`

DllMain的大小写容易混淆。

### 2.关于编译是A还是W的问题

（以MessageBox为例）

​	detour函数在dll里面形成，如果是

~~~c++
static int(WINAPI *OldMessageBox)
    (_In_opt_ HWND hWnd, 
     _In_opt_ LPCTSTR lpText, 
     _In_opt_ LPCTSTR lpCaption, 
     _In_ UINT uType) = MessageBox;
...
    
return OldMessageBox(NULL,L"Hooked Messagebox", L"Hooked", MB_OK);
~~~

的形式，在项目属性设置成Unicode，则最后hook的是MessageBoxW;如果项目属性的语言未设置，且为

`OldMessageBox(NULL,"Hooked Messagebox", "Hooked", MB_OK);` 则编译为MessageBoxA(因为是 const char)

~~~c

~~~

### 3.关于 hook heapalloc

出现问题的原因

​		如题，注入时会报出如下问题：

![image-20220910170105001](pic\image-20220910170105001.png)

### 4.error lnk2005

~~~c
 error LNK2005: "int __cdecl Record_event(int,void *,unsigned long,unsigned long,void *)" (?Record_event@@YAHHPAXKK0@Z) 已经在 dllmain.obj 中定义
~~~

