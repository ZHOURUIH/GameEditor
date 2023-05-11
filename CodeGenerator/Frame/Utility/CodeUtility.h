#ifndef _CODE_UTILITY_H_
#define _CODE_UTILITY_H_

#include "SystemUtility.h"
#include "GameDefine.h"

class CodeUtility : public SystemUtility
{
protected:
	static string ServerProjectPath;
	static string ClientProjectPath;
	static string cppGameProjectPath;
	static string cppFrameProjectPath;
	static string cppGamePath;
	static string cppFramePath;
	static string cppStringDefinePath;
	static string csGamePath;
	static string csHotfixGamePath;
	static string START_FALG;
public:
	static bool initPath();
	static MySQLMember parseMySQLMemberLine(string line);
	// ignoreClientServer表示是否忽略客户端服务器标签,true则表示会忽略,将全部字段定义都导出
	static SQLiteMember parseSQLiteMemberLine(string line, bool ignoreClientServer);
	static PacketMember parseMemberLine(const string& line);
	static string packetNameToUpper(const string& packetName);
	static string nameToUpper(const string& sqliteName, bool preUnderLine = true);
	static string stringDeclare(const string& name) { return "static constexpr const char* NAME_DEF(" + name + ") = STR(" + name + ");"; }
	static string cppPushParamString(const PacketMember& memberInfo);
	static string cppMemberDeclareString(const PacketMember& memberInfo);
	static string cSharpPushParamString(const PacketMember& memberInfo);
	static string cppTypeToCSharpType(const string& cppType);
	static string cSharpMemberDeclareString(const PacketMember& memberInfo, bool isHotFix);
	static void parsePacketName(const string& line, PacketInfo& packetInfo);
	static string convertToCSharpType(const string& cppType);
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
