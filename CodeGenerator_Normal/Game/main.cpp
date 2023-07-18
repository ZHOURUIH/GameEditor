#include "Utility.h"
#include "CodeNetPacket.h"
#include "CodeUnityBuild.h"
#include "CodeClassDeclareAndHeader.h"

void main()
{
	if (!CodeUtility::initPath())
	{
		system("pause");
		return;
	}

	cout << "0.全部生成(除了网络通信协议代码)" << endl;
	cout << "1.全部生成(包括网络通信协议代码)" << endl;
	cout << "2.生成网络消息代码" << endl;
	cout << "3.生成UnityBuild代码" << endl;
	cout << "4.生成类声明和头文件代码" << endl;
	int input;
	cin >> input;
	if (input == 0)
	{
		CodeUnityBuild::generate();
		CodeClassDeclareAndHeader::generate();
	}
	else if (input == 1)
	{
		CodeNetPacket::generate();
		CodeUnityBuild::generate();
		CodeClassDeclareAndHeader::generate();
	}
	else if (input == 2)
	{
		CodeNetPacket::generate();
	}
	else if (input == 3)
	{
		CodeUnityBuild::generate();
	}
	else if (input == 4)
	{
		CodeClassDeclareAndHeader::generate();
	}
	system("pause");
}