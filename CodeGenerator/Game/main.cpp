#include "Utility.h"
#include "CodeGenerator.h"

void main()
{
	cout << "1.生成网络通信协议代码" << endl;
	cout << "2.生成SQLite数据结构代码" << endl;
	cout << "3.生成MySQL数据结构代码" << endl;
	int input;
	cin >> input;
	if (input == 1)
	{
		string cppHeaderPath = "E:/Github/MicroLegend_Server/MicroLegend_Server/Game/Socket/PacketHeader";
		string cppPacketDefinePath = "E:/Github/MicroLegend_Server/MicroLegend_Server/Game/Socket";
		string csHeaderPath = "E:/Github/MicroLegend/Assets/Scripts/Game/Socket/PacketHeader";
		string csPacketDefinePath = "E:/Github/MicroLegend/Assets/Scripts/Game/Socket";
		CodeGenerator::generatePacketCode(cppHeaderPath, csHeaderPath, cppPacketDefinePath, csPacketDefinePath);
	}
	else if (input == 2)
	{
		string cppDataPath = "E:/Github/MicroLegend_Server/MicroLegend_Server/Game/DataBase/SQLite/Data";
		string csDataPath = "E:/Github/MicroLegend/Assets/Scripts/Game/DataBase/SQLite/SQLiteData";
		CodeGenerator::generateSQLiteCode(cppDataPath, csDataPath);
	}
	else if (input == 3)
	{
		string cppDataPath = "E:/Github/MicroLegend_Server/MicroLegend_Server/Game/DataBase/MySQL/Data";
		CodeGenerator::generateMySQLCode(cppDataPath);
	}
	system("pause");
}