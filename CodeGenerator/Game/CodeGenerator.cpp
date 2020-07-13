#include "CodeGenerator.h"

void CodeGenerator::generatePacketCode(string cppHeaderFilePath, string csFilePath, string cppPacketDefineFilePath, string csPacketDefineFilePath)
{
	// 解析模板文件
	string fileContent;
	openTxtFile("PacketProtocal.txt", fileContent);
	if (fileContent.length() == 0)
	{
		ERROR("未找到协议文件PacketProtocal.txt");
		return;
	}
	myVector<string> lines;
	split(fileContent.c_str(), "\r\n", lines);
	bool packetStart = false;
	myVector<PacketInfo> packetInfoList;
	myVector<MemberInfo> tempMemberList;
	string tempPacketName;
	FOR_VECTOR_CONST(lines)
	{
		// 忽略注释
		if (startWith(lines[i], "//"))
		{
			continue;
		}
		// 没有成员变量的消息包
		if (lines[i] == "{}")
		{
			PacketInfo info;
			info.mPacketName = lines[i - 1];
			packetInfoList.push_back(info);
			continue;
		}
		// 成员变量列表起始
		if (lines[i] == "{")
		{
			packetStart = true;
			tempPacketName = lines[i - 1];
			tempMemberList.clear();
			continue;
		}
		// 成员变量列表结束
		if (lines[i] == "}")
		{
			PacketInfo info;
			info.mPacketName = tempPacketName;
			info.mMemberList = tempMemberList;
			packetInfoList.push_back(info);
			packetStart = false;
			tempMemberList.clear();
			tempPacketName = "";
			continue;
		}
		if (packetStart)
		{
			tempMemberList.push_back(parseMemberLine(lines[i]));
		}
	}
	deleteFolder(cppHeaderFilePath);
	deleteFolder(csFilePath);
	FOR_VECTOR_CONST(packetInfoList)
	{
		// 生成代码文件
		// .h代码
		generateCppHeaderFile(packetInfoList[i].mMemberList, packetInfoList[i].mPacketName, cppHeaderFilePath);
		// .cs代码
		generateCSharpFile(packetInfoList[i].mMemberList, packetInfoList[i].mPacketName, csFilePath);
	}
	// c++
	generateCppPacketDefineFile(packetInfoList, cppPacketDefineFilePath);
	generateCppPacketRegisteFile(packetInfoList, cppPacketDefineFilePath);
	generateCppPacketHeaderFile(packetInfoList, cppPacketDefineFilePath);
	// c#
	generateCSharpPacketDefineFile(packetInfoList, csPacketDefineFilePath);
	generateCSharpPacketRegisteFile(packetInfoList, csPacketDefineFilePath);
}

// PacketHeader.h和PacketDeclareHeader.h文件
void CodeGenerator::generateCppPacketHeaderFile(const myVector<PacketInfo>& packetList, string filePath)
{
	// PacketHeader.h
	string str0;
	str0 += "#ifndef _PACKET_HEADER_H_\r\n";
	str0 += "#define _PACKET_HEADER_H_\r\n";
	str0 += "\r\n";
	uint packetCount = packetList.size();
	FOR_I(packetCount)
	{
		if (startWith(packetList[i].mPacketName, "CS"))
		{
			str0 += "#include \"" + packetList[i].mPacketName + ".h\"\r\n";
		}
	}
	str0 += "\r\n";
	FOR_I(packetCount)
	{
		if (startWith(packetList[i].mPacketName, "SC"))
		{
			str0 += "#include \"" + packetList[i].mPacketName + ".h\"\r\n";
		}
	}
	str0 += "#include \"StringDefine.h\"\r\n";
	str0 += "\r\n";
	str0 += "#endif";
	validPath(filePath);
	str0 = ANSIToUTF8(str0.c_str(), true);
	writeFile(filePath + "PacketHeader.h", str0);

	// PacketDeclareHeader.h
	string str1;
	str1 += "#ifndef _PACKET_DECLARE_HEADER_H_\r\n";
	str1 += "#define _PACKET_DECLARE_HEADER_H_\r\n";
	str1 += "\r\n";
	FOR_I(packetCount)
	{
		if (startWith(packetList[i].mPacketName, "CS"))
		{
			str1 += "#include \"" + packetList[i].mPacketName + "_Declare.h\"\r\n";
		}
	}
	str1 += "\r\n";
	FOR_I(packetCount)
	{
		if (startWith(packetList[i].mPacketName, "SC"))
		{
			str1 += "#include \"" + packetList[i].mPacketName + "_Declare.h\"\r\n";
		}
	}
	str1 += "\r\n";
	str1 += "#endif";
	validPath(filePath);
	str1 = ANSIToUTF8(str1.c_str(), true);
	writeFile(filePath + "PacketDeclareHeader.h", str1);
}

// PacketDefine.h文件
void CodeGenerator::generateCppPacketDefineFile(const myVector<PacketInfo>& packetList, string filePath)
{
	string str;
	str += "#ifndef _PACKET_DEFINE_H_\r\n";
	str += "#define _PACKET_DEFINE_H_\r\n";
	str += "\r\n";
	str += "#include \"ServerDefine.h\"\r\n";
	str += "\r\n";
	str += "enum class PACKET_TYPE : ushort\r\n";
	str += "{\r\n";
	str += "\tMIN,\r\n";
	str += "\r\n";
	str += "\tCS_MIN = 10000,\r\n";
	uint packetCount = packetList.size();
	FOR_I(packetCount)
	{
		if (startWith(packetList[i].mPacketName, "CS"))
		{
			str += "\t" + packetNameToUpper(packetList[i].mPacketName) + ",\r\n";
		}
	}
	str += "\tCS_MAX,\r\n";

	str += "\r\n";
	str += "\tSC_MIN = 20000,\r\n";
	FOR_I(packetCount)
	{
		if (startWith(packetList[i].mPacketName, "SC"))
		{
			str += "\t" + packetNameToUpper(packetList[i].mPacketName) + ",\r\n";
		}
	}
	str += "\tSC_MAX,\r\n";
	str += "};\r\n";
	str += "\r\n";
	str += "#endif";
	validPath(filePath);
	str = ANSIToUTF8(str.c_str(), true);
	writeFile(filePath + "PacketDefine.h", str);
}

// PacketRegister.cpp文件
void CodeGenerator::generateCppPacketRegisteFile(const myVector<PacketInfo>& packetList, string filePath)
{
	string str;
	str += "#include \"PacketHeader.h\"\r\n";
	str += "#include \"GameLog.h\"\r\n";
	str += "#include \"NetServer.h\"\r\n";
	str += "#include \"PacketFactoryManager.h\"\r\n";
	str += "#include \"PacketRegister.h\"\r\n";
	str += "\r\n";
	str += "#define PACKET_FACTORY(packet, type) mPacketFactoryManager->addFactory<packet>(PACKET_TYPE::type, NAME(packet));\r\n";
	str += "\r\n";
	str += "void PacketRegister::registeAllPacket()\r\n";
	str += "{\r\n";
	str += "\tuint preCount = mPacketFactoryManager->getFactoryCount();\r\n";
	uint packetCount = packetList.size();
	FOR_I(packetCount)
	{
		if (startWith(packetList[i].mPacketName, "CS"))
		{
			str += "\tPACKET_FACTORY(" + packetList[i].mPacketName + ", " + packetNameToUpper(packetList[i].mPacketName) + ");\r\n";
		}
	}
	str += "\tmPacketFactoryManager->checkRegisteCount(preCount, (int)PACKET_TYPE::CS_MAX - (int)PACKET_TYPE::CS_MIN - 1, \"CS\");\r\n";
	str += "\r\n";
	str += "\tpreCount = mPacketFactoryManager->getFactoryCount();\r\n";
	FOR_I(packetCount)
	{
		if (startWith(packetList[i].mPacketName, "SC"))
		{
			str += "\tPACKET_FACTORY(" + packetList[i].mPacketName + ", " + packetNameToUpper(packetList[i].mPacketName) + ");\r\n";
		}
	}
	str += "\tmPacketFactoryManager->checkRegisteCount(preCount, (int)PACKET_TYPE::SC_MAX - (int)PACKET_TYPE::SC_MIN - 1, \"SC\");\r\n";
	str += "};\r\n";
	validPath(filePath);
	str = ANSIToUTF8(str.c_str(), true);
	writeFile(filePath + "PacketRegister.cpp", str);
}

// _Declare.h文件
void CodeGenerator::generateCppHeaderFile(const myVector<MemberInfo>& memberInfoList, const string& packetName, string filePath)
{
	string headerMacro = "_" + packetNameToUpper(packetName) + "_DECLARE_H_";
	string fileString;
	fileString += "#ifndef " + headerMacro + "\r\n";
	fileString += "#define " + headerMacro + "\r\n";
	fileString += "\r\n";
	fileString += "#define " + packetName + "_Declare \\\r\n";
	fileString += "public:\\\r\n";
	// 注册成员变量
	uint memberCount = memberInfoList.size();
	if (memberCount != 0)
	{
		fileString += "\tvirtual void fillParams()\\\r\n";
		fileString += "\t{\\\r\n";
		FOR_I(memberCount)
		{
			fileString += cppPushParamString(memberInfoList[i]);
		}
		fileString += "\t}\\\r\n";
	}
	else
	{
		fileString += "\tvirtual void fillParams(){}\\\r\n";
	}
	// 成员变量的声明
	fileString += "public:\\\r\n";
	FOR_I(memberCount)
	{
		fileString += cppMemberDeclareString(memberInfoList[i]);
	}
	removeLast(fileString, '\\');
	fileString += "\r\n";
	fileString += "#endif";
	validPath(filePath);
	fileString = ANSIToUTF8(fileString.c_str(), true);
	writeFile(filePath + packetName + "_Declare.h", fileString);
}

// PacketDefine.cs文件
void CodeGenerator::generateCSharpPacketDefineFile(const myVector<PacketInfo>& packetList, string filePath)
{
	string str;
	str += "using System;";
	str += "using System.Collections.Generic;";
	str += "\r\n";
	str += "public enum PACKET_TYPE : ushort\r\n";
	str += "{\r\n";
	str += "\tMIN,\r\n";
	str += "\r\n";
	str += "\tCS_MIN = 10000,\r\n";
	uint packetCount = packetList.size();
	FOR_I(packetCount)
	{
		if (startWith(packetList[i].mPacketName, "CS"))
		{
			str += "\t" + packetNameToUpper(packetList[i].mPacketName) + ",\r\n";
		}
	}
	str += "\tCS_MAX,\r\n";

	str += "\r\n";
	str += "\tSC_MIN = 20000,\r\n";
	FOR_I(packetCount)
	{
		if (startWith(packetList[i].mPacketName, "SC"))
		{
			str += "\t" + packetNameToUpper(packetList[i].mPacketName) + ",\r\n";
		}
	}
	str += "\tSC_MAX,\r\n";
	str += "};";
	validPath(filePath);
	str = ANSIToUTF8(str.c_str(), true);
	writeFile(filePath + "PacketDefine.cs", str);
}

// PacketRegister.cs文件
void CodeGenerator::generateCSharpPacketRegisteFile(const myVector<PacketInfo>& packetList, string filePath)
{
	string str;
	str += "using System;\r\n";
	str += "using System.Collections;\r\n";
	str += "using System.Collections.Generic;\r\n";
	str += "\r\n";
	str += "public class PacketRegister : GameBase\r\n";
	str += "{\r\n";
	str += "\tpublic static void registeAllPacket()\r\n";
	str += "\t{\r\n";
	str += "\t\tint preCount = mSocketFactory.getPacketTypeCount();\r\n";
	uint packetCount = packetList.size();
	FOR_I(packetCount)
	{
		if (startWith(packetList[i].mPacketName, "CS"))
		{
			str += "\t\tregistePacket<" + packetList[i].mPacketName + ">(PACKET_TYPE." + packetNameToUpper(packetList[i].mPacketName) + ");\r\n";
		}
	}
	str += "\t\tmSocketFactory.checkRegisteCount(PACKET_TYPE.CS_MAX - PACKET_TYPE.CS_MIN - 1, preCount, \"CS\");\r\n";
	str += "\r\n";
	str += "\t\tpreCount = mSocketFactory.getPacketTypeCount();\r\n";
	FOR_I(packetCount)
	{
		if (startWith(packetList[i].mPacketName, "SC"))
		{
			str += "\t\tregistePacket<" + packetList[i].mPacketName + ">(PACKET_TYPE." + packetNameToUpper(packetList[i].mPacketName) + ");\r\n";
		}
	}
	str += "\t\tmSocketFactory.checkRegisteCount(PACKET_TYPE.SC_MAX - PACKET_TYPE.SC_MIN - 1, preCount, \"SC\");\r\n";
	str += "\t}\r\n";
	str += "\tprotected static void registePacket<T>(PACKET_TYPE type) where T : SocketPacket, new()\r\n";
	str += "\t{\r\n";
	str += "\t\tmSocketFactory.registePacket<T>(type);\r\n";
	str += "\t}\r\n";
	str += "}\r\n";
	validPath(filePath);
	str = ANSIToUTF8(str.c_str(), true);
	writeFile(filePath + "PacketRegister.cs", str);
}

// _Declare.cs文件
void CodeGenerator::generateCSharpFile(const myVector<MemberInfo>& memberInfoList, const string& packetName, string filePath)
{
	const int prefixLength = 2;
	if (packetName.substr(0, prefixLength) != "CS" && packetName.substr(0, prefixLength) != "SC")
	{
		ERROR("包名前缀错误");
		return;
	}
	string fileString = "using System;\r\n";
	fileString += "using System.Collections;\r\n";
	fileString += "using System.Collections.Generic;\r\n";
	fileString += "\r\n";
	fileString += "public partial class " + packetName + " : SocketPacket\r\n";
	fileString += "{\r\n";
	uint memberCount = memberInfoList.size();
	FOR_I(memberCount)
	{
		fileString += cSharpMemberDeclareString(memberInfoList[i]);
	}
	if (memberCount > 0)
	{
		fileString += "\tprotected override void fillParams()\r\n";
		fileString += "\t{\r\n";
		FOR_I(memberCount)
		{
			fileString += cSharpPushParamString(memberInfoList[i]);
		}
		fileString += "\t}\r\n";
	}
	fileString += "}";
	validPath(filePath);
	fileString = ANSIToUTF8(fileString.c_str(), true);
	writeFile(filePath + packetName + "_Declare.cs", fileString);
}

MemberInfo CodeGenerator::parseMemberLine(const string& line)
{
	MemberInfo memberInfo;
	// 数组成员变量
	if (line.find_first_of('[') != -1)
	{
		// 如果是数组,则优先处理[]内部的常量
		int lengthMarcoStart = line.find_first_of('[');
		int lengthMarcoEnd = line.find_first_of(']');
		string lengthMacro = line.substr(lengthMarcoStart + 1, lengthMarcoEnd - lengthMarcoStart - 1);
		strReplaceAll(lengthMacro, " ", "");
		split(lengthMacro.c_str(), "*", memberInfo.mArrayLengthMacro);
		// 常量处理完了,再判断变量类型和变量名
		string memberLine = line.substr(0, lengthMarcoStart);
		myVector<string> memberStrList;
		split(memberLine.c_str(), " ", memberStrList);
		memberInfo.mTypeName = memberStrList[0];
		strReplaceAll(memberInfo.mTypeName, "\t", "");
		memberInfo.mIsArray = true;
		memberInfo.mMemberName = memberStrList[1];
		// 只有数组成员变量才会有第三个参数
		if (memberStrList.size() == 3)
		{
			memberInfo.mVariableLength = stringToBool(memberStrList[2]);
		}
		else
		{
			memberInfo.mVariableLength = true;
		}
	}
	// 普通成员变量
	else
	{
		myVector<string> memberStrList;
		split(line.c_str(), " ", memberStrList);
		if (memberStrList.size() != 2 && memberStrList.size() != 3)
		{
			ERROR("成员变量行错误:" + line);
			return MemberInfo();
		}
		memberInfo.mTypeName = memberStrList[0];
		strReplaceAll(memberInfo.mTypeName, "\t", "");
		memberInfo.mIsArray = false;
		memberInfo.mMemberName = memberStrList[1];
	}
	return memberInfo;
}

string CodeGenerator::packetNameToUpper(const string& packetName)
{
	// 根据大写字母拆分
	myVector<string> macroList;
	int length = packetName.length();
	const int prefixLength = 2;
	if (packetName.substr(0, prefixLength) != "CS" && packetName.substr(0, prefixLength) != "SC")
	{
		ERROR("包名前缀错误");
		return "";
	}
	int lastIndex = prefixLength;
	// 从3开始,因为第0,1个是前缀,第2个始终都是大写,会截取出空字符串
	for (int i = prefixLength + 1; i < length; ++i)
	{
		// 已大写字母为分隔符
		if (packetName[i] >= 'A' && packetName[i] <= 'Z')
		{
			macroList.push_back(packetName.substr(lastIndex, i - lastIndex));
			lastIndex = i;
		}
	}
	macroList.push_back(packetName.substr(lastIndex, length - lastIndex));
	string headerMacro = packetName.substr(0, prefixLength);
	FOR_VECTOR_CONST(macroList)
	{
		headerMacro += "_" + toUpper(macroList[i]);
	}
	return headerMacro;
}

string CodeGenerator::cppPushParamString(const MemberInfo& memberInfo)
{
	string str;
	if (memberInfo.mIsArray)
	{
		string lengthMacro;
		uint macroCount = memberInfo.mArrayLengthMacro.size();
		FOR_I(macroCount)
		{
			lengthMacro += "GameDefine::" + memberInfo.mArrayLengthMacro[i];
			if (i != macroCount - 1)
			{
				lengthMacro += " * ";
			}
		}
		str = "\t\tpushParam(" + memberInfo.mMemberName + ", " + lengthMacro +
			", " + boolToString(memberInfo.mVariableLength) + ");\\\r\n";
	}
	else
	{
		str = "\t\tpushParam(" + memberInfo.mMemberName + ");\\\r\n";
	}
	return str;
}

string CodeGenerator::cppMemberDeclareString(const MemberInfo& memberInfo)
{
	string str;
	if (memberInfo.mIsArray)
	{
		string lengthMacro;
		uint macroCount = memberInfo.mArrayLengthMacro.size();
		FOR_I(macroCount)
		{
			lengthMacro += "GameDefine::" + memberInfo.mArrayLengthMacro[i];
			if (i != macroCount - 1)
			{
				lengthMacro += " * ";
			}
		}
		str = "\t" + memberInfo.mTypeName + " " + memberInfo.mMemberName + "[" + lengthMacro + "];\\\r\n";
	}
	else
	{
		str = "\t" + memberInfo.mTypeName + " " + memberInfo.mMemberName + ";\\\r\n";
	}
	return str;
}

string CodeGenerator::cSharpPushParamString(const MemberInfo& memberInfo)
{
	string str;
	if (memberInfo.mIsArray)
	{
		str = "\t\tpushParam(" + memberInfo.mMemberName + ", " + boolToString(memberInfo.mVariableLength) + ");\r\n";
	}
	else
	{
		str = "\t\tpushParam(" + memberInfo.mMemberName + ");\r\n";
	}
	return str;
}

string CodeGenerator::cSharpMemberDeclareString(const MemberInfo& memberInfo)
{
	// c#里面不用char,使用byte,也没有ullong,使用long
	string typeName = memberInfo.mTypeName;
	if (typeName == "char")
	{
		typeName = "byte";
	}
	else if (typeName == "ullong")
	{
		typeName = "long";
	}
	string str;
	if (memberInfo.mIsArray)
	{
		typeName = toUpper(typeName) + "S";
		string lengthMacro;
		uint macroCount = memberInfo.mArrayLengthMacro.size();
		FOR_I(macroCount)
		{
			lengthMacro += "GameDefine." + memberInfo.mArrayLengthMacro[i];
			if (i != macroCount - 1)
			{
				lengthMacro += " * ";
			}
		}
		str = "\tpublic " + typeName + " " + memberInfo.mMemberName + " = new " + typeName + "(" + lengthMacro + ");\r\n";
	}
	else
	{
		typeName = toUpper(typeName);
		str = "\tpublic " + typeName + " " + memberInfo.mMemberName + " = new " + typeName + "();\r\n";
	}
	return str;
}