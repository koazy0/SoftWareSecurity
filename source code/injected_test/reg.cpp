#include<Windows.h>
#include<iostream>


using namespace std;
int main()
{
	system("echo test_reg.exe");
	system("pause");

	HKEY hkey = NULL;
	//�򿪻��ߴ���һ��ע���key
	
	LONG error = RegCreateKey(HKEY_CURRENT_USER, L"1st_level\\2nd_level\\3rd_level\\test", &hkey);

	if (error != ERROR_SUCCESS)
	{
		cout << "����ʧ��" << endl;
		return EXIT_FAILURE;
	}

	char ou[] = "hello my value";
	//��򿪵�ע���key���д��һ��helloΪ��  hello my value Ϊֵ��item
	//REG_SZ��ʶд�������,RegSetValue��ֻ��ΪREG_SZ
	//int reult = RegSetValueEx(openkey, "hello", 0, REG_SZ, (CONST BYTE*)ou, sizeof(ou));
	error = RegSetValue(hkey, L"hello", REG_SZ, (LPCTSTR)ou, sizeof(ou));

	if (error != ERROR_SUCCESS)
	{
		cout << "д��ʧ��" << endl;
		return EXIT_FAILURE;
	}

	//error = RegDeleteTree(HKEY_CURRENT_USER, "1st_level");  //ֻ��ɾ����ײ�ļ�
	error=RegDeleteTree(HKEY_CURRENT_USER, L"1st_level");
	
	if (error != ERROR_SUCCESS)
	{
		cout << "ɾ��ʧ��:"<<GetLastError() << endl;
		return EXIT_FAILURE;
	}

	RegCloseKey(hkey);
	return 0;
}