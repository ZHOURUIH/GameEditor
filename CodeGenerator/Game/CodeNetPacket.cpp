#include "CodeNetPacket.h"

void CodeNetPacket::generatePacketCode(string cppCSDeclareFilePath, string cppSCDeclareFilePath, string csharpCSFilePath, string csharpSCFilePath, string cppPacketDefineFilePath, string csPacketDefineFilePath, string cppStringDefinePath)
{
	// 解析模板文件
	string csFileContent;
	openTxtFile("PacketCS.txt", csFileContent);
	if (csFileContent.length() == 0)
	{
		ERROR("未找到协议文件PacketCS.txt");
		return;
	}
	string scFileContent;
	openTxtFile("PacketSC.txt", scFileContent);
	if (scFileContent.length() == 0)
	{
		ERROR("未找到协议文件PacketSC.txt");
		return;
	}
	myVector<string> csLines;
	myVector<string> scLines;
	split(csFileContent.c_str(), "\r\n", csLines);
	split(scFileContent.c_str(), "\r\n", scLines);
	myVector<string> lines;
	lines.merge(csLines);
	lines.merge(scLines);
	bool packetStart = false;
	myVector<PacketInfo> packetInfoList;
	myVector<PacketMember> tempMemberList;
	string tempPacketName;
	FOR_VECTOR_CONST(lines)
	{
		string line = lines[i];
		// 忽略注释
		if (startWith(line, "//"))
		{
			continue;
		}
		// 如果后面插有注释,则去除
		int pos = -1;
		if (findString(line.c_str(), "//", &pos))
		{
			line = line.substr(0, pos);
		}
		// 去除所有制表符
		strReplaceAll(line, "\t", "");
		// 去除所有的分号
		strReplaceAll(line, ";", "");
		// 没有成员变量的消息包
		if (line == "{}")
		{
			PacketInfo info;
			info.mPacketName = lines[i - 1];
			packetInfoList.push_back(info);
			continue;
		}
		// 成员变量列表起始
		if (line == "{")
		{
			packetStart = true;
			tempPacketName = lines[i - 1];
			tempMemberList.clear();
			continue;
		}
		// 成员变量列表结束
		if (line == "}")
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
			tempMemberList.push_back(parseMemberLine(line));
		}
	}

	// 删除旧文件
	deleteFolder(cppCSDeclareFilePath);
	deleteFolder(cppSCDeclareFilePath);
	// c#的只删除代码文件,不删除meta文件
	myVector<string> csharpCSFileList;
	findFiles(csharpCSFilePath, csharpCSFileList, ".cs");
	FOR_VECTOR_CONST(csharpCSFileList)
	{
		deleteFile(csharpCSFileList[i]);
	}
	myVector<string> csharpSCFileList;
	findFiles(csharpSCFilePath, csharpSCFileList, ".cs");
	FOR_VECTOR_CONST(csharpSCFileList)
	{
		deleteFile(csharpSCFileList[i]);
	}

	myVector<string> packetList;
	FOR_VECTOR_CONST(packetInfoList)
	{
		// 生成代码文件
		// .h代码
		generateCppPacketDeclareFile(packetInfoList[i], cppCSDeclareFilePath, cppSCDeclareFilePath);
		// .cs代码
		generateCSharpFile(packetInfoList[i], csharpCSFilePath, csharpSCFilePath);
		packetList.push_back(packetInfoList[i].mPacketName);
	}
	// c++
	generateCppPacketDefineFile(packetInfoList, cppPacketDefineFilePath);
	generateCppPacketRegisteFile(packetInfoList, cppPacketDefineFilePath);
	generateCppPacketTotalHeaderFile(packetInfoList, cppPacketDefineFilePath);
	generateStringDefinePacket(packetList, cppStringDefinePath);
	// c#
	generateCSharpPacketDefineFile(packetInfoList, csPacketDefineFilePath);
	generateCSharpPacketRegisteFile(packetInfoList, csPacketDefineFilePath);
}

// PacketHeader.h和PacketDeclareHeader.h文件
void CodeNetPacket::generateCppPacketTotalHeaderFile(const myVector<PacketInfo>& packetList, string filePath)
{
	// PacketHeader.h
	string str0;
	line(str0, "#ifndef _PACKET_HEADER_H_");
	line(str0, "#define _PACKET_HEADER_H_");
	line(str0, "");
	uint packetCount = packetList.size();
	FOR_I(packetCount)
	{
		if (startWith(packetList[i].mPacketName, "CS"))
		{
			line(str0, "#include \"" + packetList[i].mPacketName + ".h\"");
		}
	}
	line(str0, "");
	FOR_I(packetCount)
	{
		if (startWith(packetList[i].mPacketName, "SC"))
		{
			line(str0, "#include \"" + packetList[i].mPacketName + ".h\"");
		}
	}
	line(str0, "#include \"StringDefine.h\"");
	line(str0, "");
	line(str0, "#endif", false);
	validPath(filePath);
	str0 = ANSIToUTF8(str0.c_str(), true);
	writeFile(filePath + "PacketHeader.h", str0);

	// PacketDeclareHeader.h
	string str1;
	line(str1, "#ifndef _PACKET_DECLARE_HEADER_H_");
	line(str1, "#define _PACKET_DECLARE_HEADER_H_");
	line(str1, "");
	FOR_I(packetCount)
	{
		if (startWith(packetList[i].mPacketName, "CS"))
		{
			line(str1, "#include \"" + packetList[i].mPacketName + "_Declare.h\"");
		}
	}
	line(str1, "");
	FOR_I(packetCount)
	{
		if (startWith(packetList[i].mPacketName, "SC"))
		{
			line(str1, "#include \"" + packetList[i].mPacketName + "_Declare.h\"");
		}
	}
	line(str1, "");
	line(str1, "#endif", false);

	validPath(filePath);
	str1 = ANSIToUTF8(str1.c_str(), true);
	writeFile(filePath + "PacketDeclareHeader.h", str1);
}

// PacketDefine.h文件
void CodeNetPacket::generateCppPacketDefineFile(const myVector<PacketInfo>& packetList, string filePath)
{
	string str;
	line(str, "#ifndef _PACKET_DEFINE_H_");
	line(str, "#define _PACKET_DEFINE_H_");
	line(str, "");
	line(str, "#include \"ServerDefine.h\"");
	line(str, "");
	line(str, "enum class PACKET_TYPE : ushort");
	line(str, "{");
	line(str, "\tMIN,");
	line(str, "");
	line(str, "\tCS_MIN = 3000,");
	uint packetCount = packetList.size();
	FOR_I(packetCount)
	{
		if (startWith(packetList[i].mPacketName, "CS"))
		{
			line(str, "\t" + packetNameToUpper(packetList[i].mPacketName) + ",");
		}
	}
	line(str, "\tCS_MAX,");
	line(str, "");
	line(str, "\tSC_MIN = 6000,");
	FOR_I(packetCount)
	{
		if (startWith(packetList[i].mPacketName, "SC"))
		{
			line(str, "\t" + packetNameToUpper(packetList[i].mPacketName) + ",");
		}
	}
	line(str, "\tSC_MAX,");
	line(str, "};");
	line(str, "");
	line(str, "#endif", false);

	validPath(filePath);
	str = ANSIToUTF8(str.c_str(), true);
	writeFile(filePath + "PacketDefine.h", str);
}

// PacketRegister.cpp文件
void CodeNetPacket::generateCppPacketRegisteFile(const myVector<PacketInfo>& packetList, string filePath)
{
	string str;
	line(str, "#include \"PacketHeader.h\"");
	line(str, "#include \"GameLog.h\"");
	line(str, "#include \"NetServer.h\"");
	line(str, "#include \"PacketFactoryManager.h\"");
	line(str, "#include \"PacketRegister.h\"");
	line(str, "");
	line(str, "#define PACKET_FACTORY(packet, type) mPacketFactoryManager->addFactory<packet>(PACKET_TYPE::type, NAME(packet));");
	line(str, "");
	line(str, "void PacketRegister::registeAll()");
	line(str, "{");
	line(str, "\tuint preCount = mPacketFactoryManager->getFactoryCount();");
	uint packetCount = packetList.size();
	FOR_I(packetCount)
	{
		if (startWith(packetList[i].mPacketName, "CS"))
		{
			line(str, "\tPACKET_FACTORY(" + packetList[i].mPacketName + ", " + packetNameToUpper(packetList[i].mPacketName) + ");");
		}
	}
	line(str, "\tmPacketFactoryManager->checkRegisteCount(preCount, (int)PACKET_TYPE::CS_MAX - (int)PACKET_TYPE::CS_MIN - 1, \"CS\");");
	line(str, "");
	line(str, "\tpreCount = mPacketFactoryManager->getFactoryCount();");
	FOR_I(packetCount)
	{
		if (startWith(packetList[i].mPacketName, "SC"))
		{
			line(str, "\tPACKET_FACTORY(" + packetList[i].mPacketName + ", " + packetNameToUpper(packetList[i].mPacketName) + ");");
		}
	}
	line(str, "\tmPacketFactoryManager->checkRegisteCount(preCount, (int)PACKET_TYPE::SC_MAX - (int)PACKET_TYPE::SC_MIN - 1, \"SC\");");
	line(str, "};", false);

	validPath(filePath);
	str = ANSIToUTF8(str.c_str(), true);
	writeFile(filePath + "PacketRegister.cpp", str);
}

// Packet_Declare.h文件
void CodeNetPacket::generateCppPacketDeclareFile(const PacketInfo& packetInfo, string csFilePath, string scFilePath)
{
	string headerMacro = "_" + packetNameToUpper(packetInfo.mPacketName) + "_DECLARE_H_";
	string file;
	line(file, "#ifndef " + headerMacro);
	line(file, "#define " + headerMacro);
	line(file, "");
	line(file, "#define " + packetInfo.mPacketName + "_Declare \\");
	line(file, "public:\\");
	// 注册成员变量
	uint memberCount = packetInfo.mMemberList.size();
	if (memberCount != 0)
	{
		line(file, "\tvoid fillParams() override\\");
		line(file, "\t{\\");
		FOR_I(memberCount)
		{
			line(file, cppPushParamString(packetInfo.mMemberList[i]));
		}
		line(file, "\t}\\");
	}
	else
	{
		line(file, "\tvoid fillParams() override{}\\");
	}
	// 成员变量的声明
	line(file, "public:\\");
	FOR_I(memberCount)
	{
		line(file, cppMemberDeclareString(packetInfo.mMemberList[i]));
	}
	removeLast(file, '\\');
	line(file, "");
	line(file, "#endif", false);

	if (startWith(packetInfo.mPacketName, "CS"))
	{
		validPath(csFilePath);
		file = ANSIToUTF8(file.c_str(), true);
		writeFile(csFilePath + packetInfo.mPacketName + "_Declare.h", file);
	}
	else if(startWith(packetInfo.mPacketName, "SC"))
	{
		validPath(scFilePath);
		file = ANSIToUTF8(file.c_str(), true);
		writeFile(scFilePath + packetInfo.mPacketName + "_Declare.h", file);
	}
}

// StringDefinePacket.h和StringDefinePacket.cpp
void CodeNetPacket::generateStringDefinePacket(const myVector<string>& packetList, string filePath)
{
	// 头文件
	string header;
	uint cmdCount = packetList.size();
	FOR_I(cmdCount)
	{
		line(header, "DECLARE_STRING(" + packetList[i] + ");");
	}
	validPath(filePath);
	header = ANSIToUTF8(header.c_str(), true);
	writeFile(filePath + "StringDefinePacket.h", header);

	// 源文件
	string source;
	line(source, "#include \"StringDefine.h\"");
	line(source, "#include \"PacketHeader.h\"");
	line(source, "");
	FOR_I(cmdCount)
	{
		line(source, "DEFINE_STRING(" + packetList[i] + ");");
	}

	source = ANSIToUTF8(source.c_str(), true);
	writeFile(filePath + "StringDefinePacket.cpp", source);
}

// PacketDefine.cs文件
void CodeNetPacket::generateCSharpPacketDefineFile(const myVector<PacketInfo>& packetList, string filePath)
{
	string str;
	line(str, "using System;");
	line(str, "using System.Collections.Generic;");
	line(str, "");
	line(str, "public class PACKET_TYPE");
	line(str, "{");
	line(str, "\tpublic static ushort MIN = 0;");
	line(str, "");
	int csPacketNumber = 3000;
	line(str, "\tpublic static ushort CS_MIN = " + intToString(csPacketNumber) + ";");
	uint packetCount = packetList.size();
	FOR_I(packetCount)
	{
		if (startWith(packetList[i].mPacketName, "CS"))
		{
			++csPacketNumber;
			line(str, "\tpublic static ushort " + packetNameToUpper(packetList[i].mPacketName) + " = " + intToString(csPacketNumber) + ";");
		}
	}
	++csPacketNumber;
	line(str, "\tpublic static ushort CS_MAX = " + intToString(csPacketNumber) + ";");
	line(str, "");
	int scPacketNumber = 6000;
	line(str, "\tpublic static ushort SC_MIN = " + intToString(scPacketNumber) + ";");
	FOR_I(packetCount)
	{
		if (startWith(packetList[i].mPacketName, "SC"))
		{
			++scPacketNumber;
			line(str, "\tpublic static ushort " + packetNameToUpper(packetList[i].mPacketName) + " = " + intToString(scPacketNumber) + ";");
		}
	}
	++scPacketNumber;
	line(str, "\tpublic static ushort SC_MAX = " + intToString(scPacketNumber) + ";");
	line(str, "}", false);

	validPath(filePath);
	str = ANSIToUTF8(str.c_str(), true);
	writeFile(filePath + "PacketDefine.cs", str);
}

// PacketRegister.cs文件
void CodeNetPacket::generateCSharpPacketRegisteFile(const myVector<PacketInfo>& packetList, string filePath)
{
	string str;
	line(str, "using System;");
	line(str, "using System.Collections;");
	line(str, "using System.Collections.Generic;");
	line(str, "");
	line(str, "public class PacketRegister : GameBase");
	line(str, "{");
	line(str, "\tpublic static void registeAll()");
	line(str, "\t{");
	line(str, "\t\tint preCount = mSocketFactory.getPacketTypeCount();");
	uint packetCount = packetList.size();
	FOR_I(packetCount)
	{
		if (startWith(packetList[i].mPacketName, "CS"))
		{
			line(str, "\t\tregistePacket(Typeof<" + packetList[i].mPacketName + ">(), PACKET_TYPE." + packetNameToUpper(packetList[i].mPacketName) + ");");
		}
	}
	line(str, "\t\tmSocketFactory.checkRegisteCount(PACKET_TYPE.CS_MAX - PACKET_TYPE.CS_MIN - 1, preCount, \"CS\");");
	line(str, "");
	line(str, "\t\tpreCount = mSocketFactory.getPacketTypeCount();");
	FOR_I(packetCount)
	{
		if (startWith(packetList[i].mPacketName, "SC"))
		{
			line(str, "\t\tregistePacket(Typeof<" + packetList[i].mPacketName + ">(), PACKET_TYPE." + packetNameToUpper(packetList[i].mPacketName) + ");");
		}
	}
	line(str, "\t\tmSocketFactory.checkRegisteCount(PACKET_TYPE.SC_MAX - PACKET_TYPE.SC_MIN - 1, preCount, \"SC\");");
	line(str, "\t}");
	line(str, "\tprotected static void registePacket(Type classType, ushort type)");
	line(str, "\t{");
	line(str, "\t\tmSocketFactory.registePacket(classType, type);");
	line(str, "\t}");
	line(str, "}", false);

	validPath(filePath);
	str = ANSIToUTF8(str.c_str(), true);
	writeFile(filePath + "PacketRegister.cs", str);
}

// Packet_Declare.cs文件
void CodeNetPacket::generateCSharpFile(const PacketInfo& packetInfo, string csFilePath, string scFilePath)
{
	const int prefixLength = 2;
	if (packetInfo.mPacketName.substr(0, prefixLength) != "CS" && packetInfo.mPacketName.substr(0, prefixLength) != "SC")
	{
		ERROR("包名前缀错误");
		return;
	}
	string file;
	line(file, "using System;");
	line(file, "using System.Collections;");
	line(file, "using System.Collections.Generic;");
	line(file, "");
	line(file, "public partial class " + packetInfo.mPacketName + " : SocketPacket");
	line(file, "{");
	uint memberCount = packetInfo.mMemberList.size();
	FOR_I(memberCount)
	{
		line(file, cSharpMemberDeclareString(packetInfo.mMemberList[i]));
	}
	if (memberCount > 0)
	{
		line(file, "\tprotected override void fillParams()");
		line(file, "\t{");
		FOR_I(memberCount)
		{
			line(file, cSharpPushParamString(packetInfo.mMemberList[i]));
		}
		line(file, "\t}");
	}
	line(file, "}", false);

	if (startWith(packetInfo.mPacketName, "CS"))
	{
		validPath(csFilePath);
		file = ANSIToUTF8(file.c_str(), true);
		writeFile(csFilePath + packetInfo.mPacketName + "_Declare.cs", file);
	}
	else if (startWith(packetInfo.mPacketName, "SC"))
	{
		validPath(scFilePath);
		file = ANSIToUTF8(file.c_str(), true);
		writeFile(scFilePath + packetInfo.mPacketName + "_Declare.cs", file);
	}
}