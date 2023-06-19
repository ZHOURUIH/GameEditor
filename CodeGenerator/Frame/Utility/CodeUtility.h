#ifndef _CODE_UTILITY_H_
#define _CODE_UTILITY_H_

#include "SystemUtility.h"
#include "GameDefine.h"

typedef std::function<bool(const string& line)> LineMatchCallback;

class CodeUtility : public SystemUtility
{
protected:
	static string ServerProjectPath;
	static string ClientProjectPath;
	static myVector<string> ServerExcludeIncludePath;
	static string cppGameProjectPath;
	static string cppGameCoreProjectPath;
	static string cppFrameProjectPath;
	static string cppGamePath;
	static string cppGameCorePath;
	static string cppFramePath;
	static string cppGameStringDefineFile;
	static string cppGameCoreStringDefineFile;
	static string csGamePath;
	static string csHotfixGamePath;
	static string START_FALG;
public:
	static bool initPath();
	static bool isPod(const string& type)
	{
		return type == "bool" ||
			   type == "char" ||
			   type == "sbyte" ||
			   type == "byte" ||
			   type == "short" ||
			   type == "ushort" ||
			   type == "int" ||
			   type == "uint" ||
			   type == "long" ||
			   type == "ulong" ||
			   type == "llong" ||
			   type == "ullong" ||
			   type == "float" ||
			   type == "double";
	}
	static bool isPodInteger(const string& type)
	{
		return type == "bool" ||
			   type == "char" ||
			   type == "byte" ||
			   type == "short" ||
			   type == "ushort" ||
			   type == "int" ||
			   type == "uint" ||
			   type == "long" ||
			   type == "ulong" ||
			   type == "llong" ||
			   type == "ullong";
	}
	static MySQLMember parseMySQLMemberLine(string line);
	// ignoreClientServer表示是否忽略客户端服务器标签,true则表示会忽略,将全部字段定义都导出
	static SQLiteMember parseSQLiteMemberLine(string line, bool ignoreClientServer);
	static PacketMember parseMemberLine(const string& line);
	static string packetNameToUpper(const string& packetName);
	static string nameToUpper(const string& sqliteName, bool preUnderLine = true);
	static string stringDeclare(const string& name) { return "\tstatic constexpr const char* NAME_DEF(" + name + ") = STR(" + name + ");"; }
	static string cSharpPushParamString(const PacketMember& memberInfo);
	static string cppTypeToCSharpType(const string& cppType);
	static string cSharpTypeToWrapType(const string& csharpType);
	static string cSharpMemberDeclareString(const PacketMember& memberInfo);
	static void parseStructName(const string& line, PacketStruct& structInfo);
	static void parsePacketName(const string& line, PacketInfo& packetInfo);
	static string convertToCSharpType(const string& cppType);
	static bool findCustomCode(const string& fullPath, myVector<string>& codeList, int& lineStart, const LineMatchCallback& startLineMatch, const LineMatchCallback& endLineMatch);
	static string codeListToString(const myVector<string>& codeList);
	static myVector<string> findTargetHeaderFile(const string& path, const LineMatchCallback& fileNameMatch, const LineMatchCallback& lineMatch, myMap<string, myVector<string>>* fileContentList = nullptr);
	static string findClassName(const string& line);
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
