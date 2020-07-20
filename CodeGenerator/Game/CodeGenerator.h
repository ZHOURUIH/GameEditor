#ifndef _CODE_GENERATOR_H_
#define _CODE_GENERATOR_H_

#include "Utility.h"

struct MemberInfo
{
	bool mIsArray;
	myVector<string> mArrayLengthMacro;	// 多个常量只能相乘
	string mTypeName;
	string mMemberName;
	bool mVariableLength;
};

struct PacketInfo
{
	myVector<MemberInfo> mMemberList;
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
	string mSQLiteName;
};

class CodeGenerator : public SystemUtility
{
public:
	static void generatePacketCode(string cppHeaderFilePath, string csFilePath, string cppPacketDefineFilePath, string csPacketDefineFilePath);
	static void generateSQLiteCode(string cppDataPath, string csDataPath);
	static void generateMySQLCode(string cppDataPath);
protected:
	//c++
	static void generateCppMySQLDataFile(const MySQLInfo& sqliteInfo, string filePath);
	static void generateCppSQLiteDataFile(const SQLiteInfo& sqliteInfo, string filePath);
	static void generateCppPacketHeaderFile(const myVector<PacketInfo>& packetList, string filePath);
	static void generateCppPacketDefineFile(const myVector<PacketInfo>& packetList, string filePath);
	static void generateCppPacketRegisteFile(const myVector<PacketInfo>& packetList, string filePath);
	static void generateCppHeaderFile(const myVector<MemberInfo>& memberInfoList, const string& packetName, string filePath);
	//c#
	static void generateCSharpSQLiteDataFile(const SQLiteInfo& sqliteInfo, string filePath);
	static void generateCSharpPacketDefineFile(const myVector<PacketInfo>& packetList, string filePath);
	static void generateCSharpPacketRegisteFile(const myVector<PacketInfo>& packetList, string filePath);
	static void generateCSharpFile(const myVector<MemberInfo>& memberInfoList, const string& packetName, string filePath);
	static MySQLMember parseMySQLMemberLine(string line);
	static SQLiteMember parseSQLiteMemberLine(string line);
	static MemberInfo parseMemberLine(const string& line);
	static string packetNameToUpper(const string& packetName);
	static string nameToUpper(const string& sqliteName);
	static string cppPushParamString(const MemberInfo& memberInfo);
	static string cppMemberDeclareString(const MemberInfo& memberInfo);
	static string cSharpPushParamString(const MemberInfo& memberInfo);
	static string cSharpMemberDeclareString(const MemberInfo& memberInfo);
};

#endif