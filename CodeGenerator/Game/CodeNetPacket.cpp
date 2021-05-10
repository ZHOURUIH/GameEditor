#include "CodeNetPacket.h"

void CodeNetPacket::generate()
{
	string cppCSDeclarePath = cppGamePath + "Socket/PacketDeclareCS/";
	string cppSCDeclarePath = cppGamePath + "Socket/PacketDeclareSC/";
	string cppCSPacketPath = cppGamePath + "Socket/ClientServer/";
	string cppSCPacketPath = cppGamePath + "Socket/ServerClient/";
	string cppPacketDefinePath = cppGamePath + "Socket/";
	string csharpCSHeaderGamePath = csGamePath + "Socket/PacketHeaderCS/";
	string csharpCSHeaderHotfixPath = csHotfixGamePath + "Socket/PacketHeaderCS/";
	string csharpSCHeaderGamePath = csGamePath + "Socket/PacketHeaderSC/";
	string csharpSCHeaderHotfixPath = csHotfixGamePath + "Socket/PacketHeaderSC/";
	string csharpSCPacketGamePath = csGamePath + "Socket/ServerClient/";
	string csharpSCPacketHotfixPath = csHotfixGamePath + "Socket/ServerClient/";
	string csharpPacketDefinePath = csHotfixGamePath + "Socket/";

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
		// 去除所有制表符,分号
		removeAll(line, '\t', ';');
		// 没有成员变量的消息包
		if (line == "{}")
		{
			PacketInfo info;
			int leftIndex = lines[i - 1].find_first_of('[');
			if (leftIndex != -1)
			{
				info.mPacketName = lines[i - 1].substr(0, leftIndex);
				int rightIndex = lines[i - 1].find_first_of(']');
				info.mShowInfo = stringToBool(lines[i - 1].substr(leftIndex + 1, rightIndex - leftIndex - 1));
			}
			else
			{
				info.mPacketName = lines[i - 1];
				info.mShowInfo = true;
			}
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
			int leftIndex = tempPacketName.find_first_of('[');
			if (leftIndex != -1)
			{
				info.mPacketName = tempPacketName.substr(0, leftIndex);
				int rightIndex = tempPacketName.find_first_of(']');
				info.mShowInfo = stringToBool(tempPacketName.substr(leftIndex + 1, rightIndex - leftIndex - 1));
			}
			else
			{
				info.mPacketName = tempPacketName;
				info.mShowInfo = true;
			}
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

	// 打开服务器的消息注册文件,找到当前的消息版本号,然后版本号自增
	int packetVersion = findPacketVersion(cppPacketDefinePath + "PacketRegister.cpp") + 1;

	// 删除旧文件
	deleteFolder(cppCSDeclarePath);
	deleteFolder(cppSCDeclarePath);
	// c#的只删除代码文件,不删除meta文件
	myVector<string> csharpCSFileList;
	findFiles(csharpCSHeaderGamePath, csharpCSFileList, ".cs");
	findFiles(csharpCSHeaderHotfixPath, csharpCSFileList, ".cs");
	FOR_VECTOR_CONST(csharpCSFileList)
	{
		deleteFile(csharpCSFileList[i]);
	}
	myVector<string> csharpSCFileList;
	findFiles(csharpSCHeaderGamePath, csharpSCFileList, ".cs");
	findFiles(csharpSCHeaderHotfixPath, csharpSCFileList, ".cs");
	FOR_VECTOR_CONST(csharpSCFileList)
	{
		deleteFile(csharpSCFileList[i]);
	}

	myVector<string> packetList;
	FOR_VECTOR_CONST(packetInfoList)
	{
		// 生成代码文件
		// .h代码
		generateCppCSPacketFileHeader(packetInfoList[i], cppCSPacketPath);
		generateCppCSPacketFileSource(packetInfoList[i], cppCSPacketPath);
		generateCppSCPacketFile(packetInfoList[i], cppSCPacketPath);
		// .cs代码
		generateCSharpDecalreFile(packetInfoList[i], csharpCSHeaderHotfixPath, csharpCSHeaderGamePath, csharpSCHeaderHotfixPath, csharpSCHeaderGamePath);
		generateCSharpSCPacketFile(packetInfoList[i].mPacketName, csharpSCPacketHotfixPath, csharpSCPacketGamePath);
		packetList.push_back(packetInfoList[i].mPacketName);
	}
	// c++
	generateCppPacketDefineFile(packetInfoList, cppPacketDefinePath);
	generateCppPacketRegisteFile(packetInfoList, cppPacketDefinePath, packetVersion);
	generateCppPacketTotalHeaderFile(packetInfoList, cppPacketDefinePath);
	generateStringDefinePacket(packetList, cppStringDefinePath);
	// c#
	generateCSharpPacketDefineFile(packetInfoList, csharpPacketDefinePath);
	generateCSharpPacketRegisteFile(packetInfoList, csharpPacketDefinePath, packetVersion);
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
	writeFile(filePath + "PacketHeader.h", ANSIToUTF8(str0.c_str(), true));
}

// PacketDefine.h文件
void CodeNetPacket::generateCppPacketDefineFile(const myVector<PacketInfo>& packetList, string filePath)
{
	string str;
	line(str, "#ifndef _PACKET_DEFINE_H_");
	line(str, "#define _PACKET_DEFINE_H_");
	line(str, "");
	line(str, "#include \"FrameDefine.h\"");
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

	writeFile(filePath + "PacketDefine.h", ANSIToUTF8(str.c_str(), true));
}

// PacketRegister.cpp文件
void CodeNetPacket::generateCppPacketRegisteFile(const myVector<PacketInfo>& packetList, string filePath, int packetVersion)
{
	string str;
	line(str, "#include \"GameHeader.h\"");
	line(str, "");
	line(str, "#define PACKET_FACTORY(packet, type) mPacketFactoryManager->addFactory<packet>(PACKET_TYPE::type, NAME(packet));");
	line(str, "");
	line(str, "int PacketRegister::PACKET_VERSION = " + intToString(packetVersion) + ";");
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

	writeFile(filePath + "PacketRegister.cpp", ANSIToUTF8(str.c_str(), true));
}

// StringDefinePacket.h和StringDefinePacket.cpp
void CodeNetPacket::generateStringDefinePacket(const myVector<string>& packetList, string filePath)
{
	// 头文件
	string header;
	line(header, "#ifdef _STRING_DEFINE_PACKET_H_");
	line(header, "#error \"特殊头文件,只能被StringDefine.h所包含\"");
	line(header, "#else");
	line(header, "#define _STRING_DEFINE_PACKET_H_");
	line(header, "");
	uint count = packetList.size();
	FOR_I(count)
	{
		line(header, stringDeclare(packetList[i]));
	}
	line(header, "");
	line(header, "#endif");
	writeFile(filePath + "StringDefinePacket.h", ANSIToUTF8(header.c_str(), true));

	// 源文件
	string source;
	line(source, "#include \"GameHeader.h\"");
	line(source, "");
	FOR_I(count)
	{
		line(source, stringDefine(packetList[i]));
	}
	writeFile(filePath + "StringDefinePacket.cpp", ANSIToUTF8(source.c_str(), true));
}

// CSPacket.h和CSPacket.cpp
void CodeNetPacket::generateCppCSPacketFileHeader(const PacketInfo& packetInfo, string filePath)
{
	const string& packetName = packetInfo.mPacketName;
	if (!startWith(packetName, "CS"))
	{
		return;
	}

	// CSPacket.h
	string headerFullPath = filePath + packetName + ".h";
	if (!isFileExist(headerFullPath))
	{
		string header;
		string marco = nameToUpper(packetName.substr(2));
		line(header, "#ifndef _CS" + marco + "_H_");
		line(header, "#define _CS" + marco + "_H_");
		line(header, "");
		line(header, "#include \"Packet.h\"");
		line(header, "");
		line(header, "class " + packetName + " : public Packet");
		line(header, "{");
		line(header, "public:");
		const auto& memberList = packetInfo.mMemberList;
		FOR_VECTOR_CONST(memberList)
		{
			line(header, "\t" + cppMemberDeclareString(memberList[i]));
		}
		line(header, "public:");
		if (memberListCount == 0)
		{
			line(header, "\tvoid fillParams() override");
			line(header, "\t{");
			line(header, "\t\tmShowInfo = " + boolToString(packetInfo.mShowInfo) + ";");
			line(header, "\t}");
		}
		else
		{
			line(header, "\tvoid fillParams() override");
			line(header, "\t{");
			line(header, "\t\tmShowInfo = " + boolToString(packetInfo.mShowInfo) + ";");
			FOR_I(memberListCount)
			{
				line(header, "\t\t" + cppPushParamString(memberList[i]));
			}
			line(header, "\t}");
		}
		line(header, "\tvoid execute() override;");
		line(header, "\tvoid debugInfo(char* buffer, int size) override");
		line(header, "\t{");
		line(header, "\t\tPACKET_DEBUG(\"\");");
		line(header, "\t}");
		line(header, "};");
		line(header, "");
		line(header, "#endif", false);

		writeFile(headerFullPath, ANSIToUTF8(header.c_str(), true));
	}
	else
	{
		updateOldFormatPackteFile(packetInfo, headerFullPath);
	}
}

void CodeNetPacket::updateOldFormatPackteFile(const PacketInfo& packetInfo, string fullPath)
{
	const string& packetName = packetInfo.mPacketName;
	// 打开文件,并解析文件
	string headerFile;
	openTxtFile(fullPath, headerFile);
	headerFile = UTF8ToANSI(headerFile.c_str(), true);
	myVector<string> headerLines;
	split(headerFile.c_str(), "\r\n", headerLines, false);
	// 找到第一个{
	int bracketIndex = headerLines.find("{");
	if (bracketIndex < 0)
	{
		ERROR("网络消息包文件解析错误,找不到{");
		return;
	}
	if (bracketIndex + 1 >= (int)headerLines.size())
	{
		ERROR("网络消息包文件解析错误,{出现在最后一行");
		return;
	}

	const auto& memberList = packetInfo.mMemberList;
	// 找到第一个public:
	int firstPublicIndex = headerLines.find("public:");
	int secondPublicIndex = headerLines.find("public:", firstPublicIndex + 1);
	if (firstPublicIndex < 0 || secondPublicIndex < 0)
	{
		ERROR("网络消息包文件解析错误,public:的数量小于2个");
		return;
	}
	// 移除两个public:之间旧的变量声明
	uint oldVariableCount = secondPublicIndex - firstPublicIndex - 1;
	FOR_I(oldVariableCount)
	{
		headerLines.erase(secondPublicIndex - 1 - i);
	}
	FOR_VECTOR_CONST(memberList)
	{
		headerLines.insert(firstPublicIndex + 1 + i, "\t" + cppMemberDeclareString(memberList[i]));
	}
	secondPublicIndex += memberList.size() - oldVariableCount;
	// 找到fillParams()
	int fillParamIndex = -1;
	FOR_VECTOR(headerLines)
	{
		if (findSubstr(headerLines[i], "void fillParams()"))
		{
			fillParamIndex = i;
			break;
		}
	}
	END(headerLines);
	if (fillParamIndex < 0)
	{
		ERROR("网络消息包文件解析错误,找不到fillParams函数");
		return;
	}
	// 函数体有变量注册
	if (headerLines[fillParamIndex + 1] == "\t{")
	{
		int removeOldStartIndex = fillParamIndex + 2;
		if (findSubstr(headerLines[removeOldStartIndex], "mShowInfo = "))
		{
			headerLines[removeOldStartIndex] = "\t\tmShowInfo = " + boolToString(packetInfo.mShowInfo) + ";";
			++removeOldStartIndex;
			// 移除所有已注册的变量
			FOR_I(oldVariableCount)
			{
				headerLines.erase(removeOldStartIndex + oldVariableCount - 1 - i);
			}
		}
		else
		{
			// 移除所有已注册的变量
			FOR_I(oldVariableCount)
			{
				headerLines.erase(removeOldStartIndex + oldVariableCount - 1 - i);
			}
			headerLines.insert(removeOldStartIndex, "\t\tmShowInfo = " + boolToString(packetInfo.mShowInfo) + ";");
		}
		if (memberListCount > 0)
		{
			// 添加新的变量
			FOR_I(memberListCount)
			{
				headerLines.insert(fillParamIndex + 3 + i, "\t\t" + cppPushParamString(memberList[i]));
			}
		}
	}
	// 函数体没有变量注册
	else
	{
		if (memberListCount > 0)
		{
			headerLines[fillParamIndex] = "\tvoid fillParams() override";
			headerLines.insert(fillParamIndex + 1, "\t{");
			headerLines.insert(fillParamIndex + 2, "\t\tmShowInfo = " + boolToString(packetInfo.mShowInfo) + ";");
			// 添加新的变量
			FOR_I(memberListCount)
			{
				headerLines.insert(fillParamIndex + 3 + i, "\t\t" + cppPushParamString(memberList[i]));
			}
			headerLines.insert(fillParamIndex + 3 + memberListCount, "\t}");
		}
		else
		{
			headerLines[fillParamIndex] = "\tvoid fillParams() override";
			headerLines.insert(fillParamIndex + 1, "\t{");
			headerLines.insert(fillParamIndex + 2, "\t\tmShowInfo = " + boolToString(packetInfo.mShowInfo) + ";");
			headerLines.insert(fillParamIndex + 3, "\t}");
		}
	}
	// 生成新的文件
	string newFile;
	uint newCount = headerLines.size();
	FOR_I(newCount)
	{
		newFile += headerLines[i];
		if (i != headerLinesCount - 1)
		{
			newFile += "\r\n";
		}
	}
	writeFile(fullPath, ANSIToUTF8(newFile.c_str(), true));
}

int CodeNetPacket::findPacketVersion(const string& filePath)
{
	int packetVersion = 0;
	string fileContent;
	openTxtFile(filePath, fileContent);
	myVector<string> lines;
	split(fileContent.c_str(), "\r\n", lines);
	FOR_VECTOR(lines)
	{
		int index = 0;
		if (findString(lines[i].c_str(), "PACKET_VERSION = ", &index))
		{
			int pos0 = lines[i].find_first_of('=', index);
			int pos1 = lines[i].find_first_of(';', index);
			string versionStr = lines[i].substr(pos0 + 1, pos1 - pos0 - 1);
			packetVersion = stringToInt(versionStr);
			break;
		}
	}
	END(lines);
	return packetVersion;
}

void CodeNetPacket::generateCppCSPacketFileSource(const PacketInfo& packetInfo, string filePath)
{
	const string& packetName = packetInfo.mPacketName;
	if (!startWith(packetName, "CS"))
	{
		return;
	}

	// CSPacket.cpp
	string sourceFullPath = filePath + packetName + ".cpp";
	if (!isFileExist(sourceFullPath))
	{
		string source;
		line(source, "#include \"GameHeader.h\"");
		line(source, "");
		line(source, "void " + packetName + "::execute()");
		line(source, "{");
		line(source, "\tCharacterPlayer* player = getCharacter();");
		line(source, "\tif (player == NULL)");
		line(source, "\t{");
		line(source, "\t\treturn;");
		line(source, "\t}");
		line(source, "}", false);

		writeFile(sourceFullPath, ANSIToUTF8(source.c_str(), true));
	}
}

// SCPacket.h文件
void CodeNetPacket::generateCppSCPacketFile(const PacketInfo& packetInfo, string filePath)
{
	const string& packetName = packetInfo.mPacketName;
	if (!startWith(packetName, "SC"))
	{
		return;
	}

	// SCPacket.h
	string headerFullPath = filePath + packetName + ".h";
	if (!isFileExist(headerFullPath))
	{
		string header;
		string marco = nameToUpper(packetName.substr(2));
		line(header, "#ifndef _SC" + marco + "_H_");
		line(header, "#define _SC" + marco + "_H_");
		line(header, "");
		line(header, "#include \"Packet.h\"");
		line(header, "");
		line(header, "class " + packetName + " : public Packet");
		line(header, "{");
		line(header, "public:");
		const auto& memberList = packetInfo.mMemberList;
		FOR_VECTOR_CONST(memberList)
		{
			line(header, "\t" + cppMemberDeclareString(memberList[i]));
		}
		line(header, "public:");
		if (memberListCount == 0)
		{
			line(header, "\tvoid fillParams() override");
			line(header, "\t{");
			line(header, "\t\tmShowInfo = " + boolToString(packetInfo.mShowInfo) + ";");
			line(header, "\t}");
		}
		else
		{
			line(header, "\tvoid fillParams() override");
			line(header, "\t{");
			line(header, "\t\tmShowInfo = " + boolToString(packetInfo.mShowInfo) + ";");
			FOR_I(memberListCount)
			{
				line(header, "\t\t" + cppPushParamString(memberList[i]));
			}
			line(header, "\t}");
		}
		line(header, "\tvoid debugInfo(char* buffer, int size) override");
		line(header, "\t{");
		line(header, "\t\tPACKET_DEBUG(\"\")");
		line(header, "\t}");
		line(header, "};");
		line(header, "");
		line(header, "#endif", false);

		writeFile(headerFullPath, ANSIToUTF8(header.c_str(), true));
	}
	else
	{
		updateOldFormatPackteFile(packetInfo, headerFullPath);
	}
}

// PacketDefine.cs文件
void CodeNetPacket::generateCSharpPacketDefineFile(const myVector<PacketInfo>& packetList, string filePath)
{
	string str;
	line(str, "using System;");
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

	writeFile(filePath + "PacketDefine.cs", ANSIToUTF8(str.c_str(), true));
}

// PacketRegister.cs文件
void CodeNetPacket::generateCSharpPacketRegisteFile(const myVector<PacketInfo>& packetList, string filePath, int packetVersion)
{
	string str;
	line(str, "using System;");
	line(str, "");
	line(str, "public class PacketRegister : GB");
	line(str, "{");
	line(str, "\tpublic static int PACKET_VERSION = " + intToString(packetVersion) + ";");
	line(str, "\tpublic static void registeAll()");
	line(str, "\t{");
	line(str, "\t\tint preCount = mSocketFactory.getPacketTypeCount();");
	uint packetCount = packetList.size();
	FOR_I(packetCount)
	{
		if (startWith(packetList[i].mPacketName, "CS"))
		{
			line(str, "\t\tregistePacket(typeof(" + packetList[i].mPacketName + "), PACKET_TYPE." + packetNameToUpper(packetList[i].mPacketName) + ");");
		}
	}
	line(str, "\t\tmSocketFactory.checkRegisteCount(PACKET_TYPE.CS_MAX - PACKET_TYPE.CS_MIN - 1, preCount, \"CS\");");
	line(str, "");
	line(str, "\t\tpreCount = mSocketFactory.getPacketTypeCount();");
	FOR_I(packetCount)
	{
		if (startWith(packetList[i].mPacketName, "SC"))
		{
			line(str, "\t\tregistePacket(typeof(" + packetList[i].mPacketName + "), PACKET_TYPE." + packetNameToUpper(packetList[i].mPacketName) + ");");
		}
	}
	line(str, "\t\tmSocketFactory.checkRegisteCount(PACKET_TYPE.SC_MAX - PACKET_TYPE.SC_MIN - 1, preCount, \"SC\");");
	line(str, "\t}");
	line(str, "\tprotected static void registePacket(Type classType, ushort type)");
	line(str, "\t{");
	line(str, "\t\tmSocketFactory.registePacket(classType, type);");
	line(str, "\t\tmSocketFactoryThread.registePacket(classType, type);");
	line(str, "\t}");
	line(str, "}", false);

	writeFile(filePath + "PacketRegister.cs", ANSIToUTF8(str.c_str(), true));
}

// Packet_Declare.cs文件
void CodeNetPacket::generateCSharpDecalreFile(const PacketInfo& packetInfo, string csHotfixFilePath, string csGameFilePath, string scHotfixFilePath, string scGameFilePath)
{
	const int prefixLength = 2;
	if (packetInfo.mPacketName.substr(0, prefixLength) != "CS" && packetInfo.mPacketName.substr(0, prefixLength) != "SC")
	{
		ERROR("包名前缀错误");
		return;
	}
	bool isHotfixPacket = false;
	if (startWith(packetInfo.mPacketName, "CS"))
	{
		isHotfixPacket = packetInfo.mPacketName != "CSHeartBeat" && packetInfo.mPacketName != "CSPing";
	}
	else if (startWith(packetInfo.mPacketName, "SC"))
	{
		isHotfixPacket = packetInfo.mPacketName != "SCHeartBeat" && packetInfo.mPacketName != "SCPing";
	}
	string file;
	line(file, "using System;");
	line(file, "");
	if (isHotfixPacket)
	{
		line(file, "public partial class " + packetInfo.mPacketName + " : ILRSocketPacket");
	}
	else
	{
		line(file, "public partial class " + packetInfo.mPacketName + " : SocketPacket");
	}
	line(file, "{");
	uint memberCount = packetInfo.mMemberList.size();
	FOR_I(memberCount)
	{
		line(file, "\t" + cSharpMemberDeclareString(packetInfo.mMemberList[i]));
	}
	if (memberCount > 0)
	{
		line(file, "\tprotected override void fillParams()");
		line(file, "\t{");
		FOR_I(memberCount)
		{
			line(file, "\t\t" + cSharpPushParamString(packetInfo.mMemberList[i]));
		}
		line(file, "\t}");
	}
	line(file, "}", false);

	if (startWith(packetInfo.mPacketName, "CS"))
	{
		string realPath = isHotfixPacket ? csHotfixFilePath : csGameFilePath;
		writeFile(realPath + packetInfo.mPacketName + "_Declare.cs", ANSIToUTF8(file.c_str(), true));
	}
	else if (startWith(packetInfo.mPacketName, "SC"))
	{
		string realPath = isHotfixPacket ? scHotfixFilePath : scGameFilePath;
		writeFile(realPath + packetInfo.mPacketName + "_Declare.cs", ANSIToUTF8(file.c_str(), true));
	}
}

void CodeNetPacket::generateCSharpSCPacketFile(const string& packetName, string fileHotfixPath, string fileGamePath)
{
	if (!startWith(packetName, "SC"))
	{
		return;
	}
	bool isHotfixPacket = packetName != "SCHeartBeat" && packetName != "SCPing";
	string fullPath = isHotfixPacket ? fileHotfixPath + packetName + ".cs" : fileGamePath + packetName + ".cs";
	if (!isFileExist(fullPath))
	{
		string file;
		line(file, "using System;");
		line(file, "");
		if (isHotfixPacket)
		{
			line(file, "public partial class " + packetName + " : ILRSocketPacket");
		}
		else
		{
			line(file, "public partial class " + packetName + " : SocketPacket");
		}
		line(file, "{");
		line(file, "\tpublic override void execute()");
		line(file, "\t{");
		line(file, "\t}");
		line(file, "}", false);

		writeFile(fullPath, ANSIToUTF8(file.c_str(), true));
	}
}