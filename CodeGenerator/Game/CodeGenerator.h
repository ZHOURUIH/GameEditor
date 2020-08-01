#ifndef _CODE_GENERATOR_H_
#define _CODE_GENERATOR_H_

#include "Utility.h"

struct PacketMember
{
	bool mIsArray;
	myVector<string> mArrayLengthMacro;	// 多个常量只能相乘
	string mTypeName;
	string mMemberName;
	bool mVariableLength;
};

struct PacketInfo
{
	myVector<PacketMember> mMemberList;
	string mPacketName;
};

enum class SQLITE_OWNER
{
	BOTH,
	CLIENT_ONLY,
	SERVER_ONLY,
};

struct SQLiteMember
{
	SQLITE_OWNER mOwner;
	string mTypeName;
	string mMemberName;
	string mLinkTable;
};

struct SQLiteInfo
{
	myVector<SQLiteMember> mMemberList;
	SQLITE_OWNER mOwner;
	string mSQLiteName;
};

struct MySQLMember
{
	string mTypeName;
	string mMemberName;
	bool mUTF8;
};

struct MySQLInfo
{
	myVector<MySQLMember> mMemberList;
	string mMySQLName;
};

class CodeGenerator : public SystemUtility
{
public:
	static void generatePacketCode(string cppDeclareFilePath, string csFilePath, string cppPacketDefineFilePath, string csPacketDefineFilePath, string cppStringDefinePath);
	static void generateSQLiteCode(string cppDataPath, string cppTablePath, string csDataPath, string csTablePath);
	static void generateMySQLCode(string cppDataPath, string cppStringDefinePath);
	static void generateCmdCode(string filePath, string headerPath);
	static void generateStateCode(string filePath, string headerPath);
	static void generateSkillCode(string filePath, string headerPath);
protected:
	//c++
	static void generateCppMySQLDataFile(const MySQLInfo& sqliteInfo, string filePath);
	static void generateCppSQLiteDataFile(const SQLiteInfo& sqliteInfo, string dataFilePath, string tableFilePath);
	static void generateCppSQLiteTotalHeaderFile(const myVector<SQLiteInfo>& sqliteList, string filePath);
	static void generateCppSQLiteRegisteFile(const myVector<SQLiteInfo>& sqliteList, string filePath);
	static void generateCppMySQLTotalHeaderFile(const myVector<MySQLInfo>& mysqlList, string filePath);
	static void generateCppMySQLRegisteFile(const myVector<MySQLInfo>& mysqlList, string filePath);
	static void generateCppCmdTotalHeaderFile(const myVector<string>& cmdList, string filePath);
	static void generateCppStateTotalHeaderFile(const myVector<string>& stateList, string filePath);
	static void generateCppSkillTotalHeaderFile(const myVector<string>& skillList, string filePath);
	static void generateCppPacketTotalHeaderFile(const myVector<PacketInfo>& packetList, string filePath);
	static void generateCppPacketDefineFile(const myVector<PacketInfo>& packetList, string filePath);
	static void generateCppPacketRegisteFile(const myVector<PacketInfo>& packetList, string filePath);
	static void generateCppPacketDeclareFile(const PacketInfo& packetName, string filePath);
	static void generateStringDefineCmd(const myVector<string>& cmdList, string filePath);
	static void generateStringDefineSkill(const myVector<string>& skillList, string filePath);
	static void generateStringDefineMySQL(const myVector<string>& mysqlList, string filePath);
	static void generateStringDefineState(const myVector<string>& stateList, string filePath);
	static void generateStringDefinePacket(const myVector<string>& packetList, string filePath);
	//c#
	static void generateCSharpSQLiteDataFile(const SQLiteInfo& sqliteInfo, string dataFilePath, string tableFilePath);
	static void generateCSharpSQLiteRegisteFileFile(const myVector<SQLiteInfo>& sqliteInfo, string filePath);
	static void generateCSharpPacketDefineFile(const myVector<PacketInfo>& packetList, string filePath);
	static void generateCSharpPacketRegisteFile(const myVector<PacketInfo>& packetList, string filePath);
	static void generateCSharpFile(const PacketInfo& packetName, string filePath);
	static MySQLMember parseMySQLMemberLine(string line);
	static SQLiteMember parseSQLiteMemberLine(string line);
	static PacketMember parseMemberLine(const string& line);
	static string packetNameToUpper(const string& packetName);
	static string nameToUpper(const string& sqliteName);
	static string cppPushParamString(const PacketMember& memberInfo);
	static string cppMemberDeclareString(const PacketMember& memberInfo);
	static string cSharpPushParamString(const PacketMember& memberInfo);
	static string cSharpMemberDeclareString(const PacketMember& memberInfo);
	static void line(string& str, const string& line, bool returnLine = true) 
	{
		str += line;
		if (returnLine)
		{
			str += "\r\n";
		}
	}
};

#endif