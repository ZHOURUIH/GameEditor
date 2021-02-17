#ifndef _CODE_UTILITY_H_
#define _CODE_UTILITY_H_

#include "SystemUtility.h"
#include "GameDefine.h"

class CodeUtility : public SystemUtility
{
protected:
	static string ServerProjectPath;
	static string GameProjectPath;
	static string cppProjectPath;
	static string cppGamePath;
	static string cppFramePath;
	static string cppStringDefinePath;
	static string csGamePath;
public:
	static void initPath();
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
