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

class CodeGenerator : public SystemUtility
{
public:
	static void generatePacketCode(string cppHeaderFilePath, string csFilePath, string cppPacketDefineFilePath, string csPacketDefineFilePath);
protected:
	//c++
	static void generateCppPacketHeaderFile(const myVector<PacketInfo>& packetList, string filePath);
	static void generateCppPacketDefineFile(const myVector<PacketInfo>& packetList, string filePath);
	static void generateCppPacketRegisteFile(const myVector<PacketInfo>& packetList, string filePath);
	static void generateCppHeaderFile(const myVector<MemberInfo>& memberInfoList, const string& packetName, string filePath);
	//c#
	static void generateCSharpPacketDefineFile(const myVector<PacketInfo>& packetList, string filePath);
	static void generateCSharpPacketRegisteFile(const myVector<PacketInfo>& packetList, string filePath);
	static void generateCSharpFile(const myVector<MemberInfo>& memberInfoList, const string& packetName, string filePath);
	static MemberInfo parseMemberLine(const string& line);
	static string packetNameToUpper(const string& packetName);
	static string cppPushParamString(const MemberInfo& memberInfo);
	static string cppMemberDeclareString(const MemberInfo& memberInfo);
	static string cSharpPushParamString(const MemberInfo& memberInfo);
	static string cSharpMemberDeclareString(const MemberInfo& memberInfo);
};

#endif