#include<Windows.h>
#include<iostream>


//heap.cpp ���Խ��Ӧ������ʾ
//*��ַÿ�����+
/*
��1���Ѷ�δݻ�,���ַΪ:0x025B0000�����ܹ��ͷ���2��
��2����δ���ݻ�,���ַΪ:0x02780000
��1������ռ����ͷ�,���ַΪ:0x025B0498�����ܹ��ͷ���3��
��2������ռ�δ���ͷ�,���ַΪ:0x025B05A0
*/


int main() {
	LPSYSTEM_INFO lpSystemInfo=new SYSTEM_INFO;
	GetSystemInfo(lpSystemInfo);//��ȡ��ǰ����page��С
	std::cout<<"Pagesize:0x"<< std::hex<<lpSystemInfo->dwPageSize<< std::endl;
	//x86CPU�ϣ����ֵΪ0x1000����Alpha CPU�ϣ����ֵΪ0x2000����IA-64�ϣ����ֵΪ0x2000
	
	HANDLE hHeap, hHeap1, hHeap2;//*hProcessHeap=new HANDLE[2];
	hHeap=HeapCreate(HEAP_CREATE_ENABLE_EXECUTE | HEAP_GENERATE_EXCEPTIONS|HEAP_NO_SERIALIZE, 0, 0x2000); //����ͷ�
	hHeap1=HeapCreate(HEAP_CREATE_ENABLE_EXECUTE | HEAP_GENERATE_EXCEPTIONS| HEAP_NO_SERIALIZE, 0, 0x2000);//δ�ͷ�
	hHeap2=HeapCreate(HEAP_CREATE_ENABLE_EXECUTE | HEAP_GENERATE_EXCEPTIONS| HEAP_NO_SERIALIZE, 0, 0x2000);//�����ͷ�

	std::cout << "HeapCreate:0x:" << hHeap << std::endl;
	/*
	GetProcessHeaps(2,hProcessHeap);

	
	std::cout<<"hProcessHeap[0]	0x:"<<hProcessHeap[0]<< std::endl;
	std::cout<<"hProcessHeap[1]	0x:"<<hProcessHeap[1]<< std::endl;
	*/


	//��private hHeap�������ڴ�,����0
	LPVOID hAlloc, hAlloc1, hAlloc2;
	hAlloc=HeapAlloc(hHeap, HEAP_ZERO_MEMORY | HEAP_NO_SERIALIZE, 0x100); //����ͷ�
	hAlloc1=HeapAlloc(hHeap, HEAP_ZERO_MEMORY | HEAP_NO_SERIALIZE, 0x100);//δ�ͷ�
	hAlloc2=HeapAlloc(hHeap, HEAP_ZERO_MEMORY | HEAP_NO_SERIALIZE, 0x100);//�����ͷ�

	//�ͷ��ڴ�
	HeapFree(hHeap, NULL, hAlloc);
	HeapFree(hHeap, NULL, hAlloc);
	HeapFree(hHeap, NULL, hAlloc);
	HeapFree(hHeap, NULL, hAlloc2);

	//�ݻٶ�
	HeapDestroy(hHeap);
	HeapDestroy(hHeap);
	HeapDestroy(hHeap2);
	
	//void* __cdecl malloc( size_t _Size);
	int *a = (int *)malloc(5*sizeof(int));//�ظ��ͷ�
	a[4] = 3;
	a=(int*)realloc(a, 6 * sizeof(int));
	a[5] = 4;
	free(a);
	//free(a);
	//free(a);


	return 0;
}