#include<Windows.h>
#include<iostream>


//heap.cpp 测试结果应如下所示
//*地址每次随机+
/*
第1个堆多次摧毁,其地址为:0x025B0000，且总共释放了2次
第2个堆未经摧毁,其地址为:0x02780000
第1个申请空间多次释放,其地址为:0x025B0498，且总共释放了3次
第2个申请空间未经释放,其地址为:0x025B05A0
*/


int main() {
	LPSYSTEM_INFO lpSystemInfo=new SYSTEM_INFO;
	GetSystemInfo(lpSystemInfo);//获取当前主机page大小
	std::cout<<"Pagesize:0x"<< std::hex<<lpSystemInfo->dwPageSize<< std::endl;
	//x86CPU上，这个值为0x1000，在Alpha CPU上，这个值为0x2000，在IA-64上，这个值为0x2000
	
	HANDLE hHeap, hHeap1, hHeap2;//*hProcessHeap=new HANDLE[2];
	hHeap=HeapCreate(HEAP_CREATE_ENABLE_EXECUTE | HEAP_GENERATE_EXCEPTIONS|HEAP_NO_SERIALIZE, 0, 0x2000); //多次释放
	hHeap1=HeapCreate(HEAP_CREATE_ENABLE_EXECUTE | HEAP_GENERATE_EXCEPTIONS| HEAP_NO_SERIALIZE, 0, 0x2000);//未释放
	hHeap2=HeapCreate(HEAP_CREATE_ENABLE_EXECUTE | HEAP_GENERATE_EXCEPTIONS| HEAP_NO_SERIALIZE, 0, 0x2000);//正常释放

	std::cout << "HeapCreate:0x:" << hHeap << std::endl;
	/*
	GetProcessHeaps(2,hProcessHeap);

	
	std::cout<<"hProcessHeap[0]	0x:"<<hProcessHeap[0]<< std::endl;
	std::cout<<"hProcessHeap[1]	0x:"<<hProcessHeap[1]<< std::endl;
	*/


	//从private hHeap中申请内存,并清0
	LPVOID hAlloc, hAlloc1, hAlloc2;
	hAlloc=HeapAlloc(hHeap, HEAP_ZERO_MEMORY | HEAP_NO_SERIALIZE, 0x100); //多次释放
	hAlloc1=HeapAlloc(hHeap, HEAP_ZERO_MEMORY | HEAP_NO_SERIALIZE, 0x100);//未释放
	hAlloc2=HeapAlloc(hHeap, HEAP_ZERO_MEMORY | HEAP_NO_SERIALIZE, 0x100);//正常释放

	//释放内存
	HeapFree(hHeap, NULL, hAlloc);
	HeapFree(hHeap, NULL, hAlloc);
	HeapFree(hHeap, NULL, hAlloc);
	HeapFree(hHeap, NULL, hAlloc2);

	//摧毁堆
	HeapDestroy(hHeap);
	HeapDestroy(hHeap);
	HeapDestroy(hHeap2);
	
	//void* __cdecl malloc( size_t _Size);
	int *a = (int *)malloc(5*sizeof(int));//重复释放
	a[4] = 3;
	a=(int*)realloc(a, 6 * sizeof(int));
	a[5] = 4;
	free(a);
	//free(a);
	//free(a);


	return 0;
}