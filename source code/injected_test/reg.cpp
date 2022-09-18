#include<Windows.h>
#include<iostream>


using namespace std;
int main()
{
	system("echo test_reg.exe");
	system("pause");

	HKEY hkey = NULL;
	//打开或者创建一个注册表key
	
	LONG error = RegCreateKey(HKEY_CURRENT_USER, L"1st_level\\2nd_level\\3rd_level\\test", &hkey);

	if (error != ERROR_SUCCESS)
	{
		cout << "创建失败" << endl;
		return EXIT_FAILURE;
	}

	char ou[] = "hello my value";
	//像打开的注册表key句柄写入一个hello为键  hello my value 为值的item
	//REG_SZ标识写入的类型,RegSetValue里只能为REG_SZ
	//int reult = RegSetValueEx(openkey, "hello", 0, REG_SZ, (CONST BYTE*)ou, sizeof(ou));
	error = RegSetValue(hkey, L"hello", REG_SZ, (LPCTSTR)ou, sizeof(ou));

	if (error != ERROR_SUCCESS)
	{
		cout << "写入失败" << endl;
		return EXIT_FAILURE;
	}

	//error = RegDeleteTree(HKEY_CURRENT_USER, "1st_level");  //只能删除最底层的键
	error=RegDeleteTree(HKEY_CURRENT_USER, L"1st_level");
	
	if (error != ERROR_SUCCESS)
	{
		cout << "删除失败:"<<GetLastError() << endl;
		return EXIT_FAILURE;
	}

	RegCloseKey(hkey);
	return 0;
}