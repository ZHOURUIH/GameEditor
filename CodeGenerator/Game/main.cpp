#include "Utility.h"
#include "CodeGenerator.h"

void main()
{
	cout << "1.生成网络通信协议代码" << endl;
	cout << "2.生成SQLite数据结构代码" << endl;
	int input;
	cin >> input;
	if (input == 1)
	{
		// 网络消息代码
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
		// SQLite数据结构代码
		CodeGenerator::generateSQLiteCode(cppDataPath, csDataPath);
	}
	system("pause");
}