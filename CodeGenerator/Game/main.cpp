#include "Utility.h"
#include "CodeCmd.h"
#include "CodeSkill.h"
#include "CodeState.h"
#include "CodeMySQL.h"
#include "CodeSQLite.h"
#include "CodeNetPacket.h"
#include "CodeAchivement.h"
#include "CodeEvent.h"

void main()
{
	string cppGamePath = "E:/Github/MicroLegend_Server/MicroLegend_Server/Game/";
	string csGamePath = "E:/Github/MicroLegend/Assets/Scripts/Game/";
	string cppStringDefinePath = cppGamePath + "StringDefine";
	cout << "1.网络通信协议代码" << endl;
	cout << "2.SQLite数据代码" << endl;
	cout << "3.MySQL数据代码" << endl;
	cout << "4.命令代码" << endl;
	cout << "5.技能代码" << endl;
	cout << "6.状态代码" << endl;
	cout << "7.成就代码" << endl;
	cout << "8.事件代码" << endl;
	int input;
	cin >> input;
	if (input == 1)
	{
		string cppCSDeclarePath = cppGamePath + "Socket/PacketDeclareCS/";
		string cppSCDeclarePath = cppGamePath + "Socket/PacketDeclareSC/";
		string cppCSPacketPath = cppGamePath + "Socket/ClientServer/";
		string cppSCPacketPath = cppGamePath + "Socket/ServerClient/";
		string cppPacketDefinePath = cppGamePath + "Socket/";
		string csharpCSHeaderPath = csGamePath + "Socket/PacketHeaderCS/";
		string csharpSCHeaderPath = csGamePath + "Socket/PacketHeaderSC/";
		string csharpSCPacketPath = csGamePath + "Socket/ServerClient/";
		string csharpPacketDefinePath = csGamePath + "Socket/";
		CodeNetPacket::generatePacketCode(cppCSDeclarePath, cppSCDeclarePath, csharpCSHeaderPath, csharpSCHeaderPath, cppPacketDefinePath, csharpPacketDefinePath, cppStringDefinePath, cppCSPacketPath, cppSCPacketPath, csharpSCPacketPath);
	}
	else if (input == 2)
	{
		string cppDataPath = cppGamePath + "DataBase/SQLite/Data/";
		string cppTablePath = cppGamePath + "DataBase/SQLite/Table/";
		string csDataPath = csGamePath + "DataBase/SQLite/Data/";
		string csTablePath = csGamePath + "DataBase/SQLite/Table/";
		CodeSQLite::generateSQLiteCode(cppDataPath, cppTablePath, csDataPath, csTablePath);
	}
	else if (input == 3)
	{
		string cppDataPath = cppGamePath + "DataBase/MySQL/Data/";
		string cppTablePath = cppGamePath + "DataBase/MySQL/Table/";
		CodeMySQL::generateMySQLCode(cppDataPath, cppTablePath, cppStringDefinePath);
	}
	else if (input == 4)
	{
		string cppHeaderPath = cppGamePath + "CommandSystem/";
		CodeCmd::generateCmdCode(cppStringDefinePath, cppHeaderPath);
	}
	else if (input == 5)
	{
		string cppHeaderPath = cppGamePath + "Character/Component/Skill/";
		CodeSkill::generateSkillCode(cppStringDefinePath, cppHeaderPath);
	}
	else if (input == 6)
	{
		string cppHeaderPath = cppGamePath + "Character/Component/StateMachine/";
		CodeState::generateStateCode(cppStringDefinePath, cppHeaderPath);
	}
	else if (input == 7)
	{
		string cppHeaderPath = cppGamePath + "Achivement/";
		string cppAchivementFilePath = cppHeaderPath + "Achivement/";
		CodeAchivement::generateAchivementCode(cppHeaderPath, cppAchivementFilePath);
	}
	else if (input == 8)
	{
		string cppHeaderPath = cppGamePath + "EventSystem/";
		string cppEventFilePath = cppHeaderPath + "Event/";
		CodeEvent::generateEventCode(cppStringDefinePath, cppHeaderPath, cppEventFilePath);
	}
	system("pause");
}