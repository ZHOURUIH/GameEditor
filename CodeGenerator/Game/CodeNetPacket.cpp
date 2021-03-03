#include "CodeNetPacket.h"

void CodeNetPacket::generate()
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
	deleteFolder(cppCSDeclarePath);
	deleteFolder(cppSCDeclarePath);
	// c#的只删除代码文件,不删除meta文件
	myVector<string> csharpCSFileList;
	findFiles(csharpCSHeaderPath, csharpCSFileList, ".cs");
	FOR_VECTOR_CONST(csharpCSFileList)
	{
		deleteFile(csharpCSFileList[i]);
	}
	myVector<string> csharpSCFileList;
	findFiles(csharpSCHeaderPath, csharpSCFileList, ".cs");
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
		generateCSharpDecalreFile(packetInfoList[i], csharpCSHeaderPath, csharpSCHeaderPath);
		generateCSharpSCPacketFile(packetInfoList[i].mPacketName, csharpSCPacketPath);
		packetList.push_back(packetInfoList[i].mPacketName);
	}
	// c++
	generateCppPacketDefineFile(packetInfoList, cppPacketDefinePath);
	generateCppPacketRegisteFile(packetInfoList, cppPacketDefinePath);
	generateCppPacketTotalHeaderFile(packetInfoList, cppPacketDefinePath);
	generateStringDefinePacket(packetList, cppStringDefinePath);
	// c#
	generateCSharpPacketDefineFile(packetInfoList, csharpPacketDefinePath);
	generateCSharpPacketRegisteFile(packetInfoList, csharpPacketDefinePath);
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

	writeFile(filePath + "PacketDefine.h", ANSIToUTF8(str.c_str(), true));
}

// PacketRegister.cpp文件
void CodeNetPacket::generateCppPacketRegisteFile(const myVector<PacketInfo>& packetList, string filePath)
{
	string str;
	line(str, "#include \"GameHeader.h\"");
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

	writeFile(filePath + "PacketRegister.cpp", ANSIToUTF8(str.c_str(), true));
}

// StringDefinePacket.h和StringDefinePacket.cpp
void CodeNetPacket::generateStringDefinePacket(const myVector<string>& packetList, string filePath)
{
	// 头文件
	string header;
	uint cmdCount = packetList.size();
	FOR_I(cmdCount)
	{
		line(header, stringDeclare(packetList[i]));
	}
	writeFile(filePath + "StringDefinePacket.h", ANSIToUTF8(header.c_str(), true));

	// 源文件
	string source;
	line(source, "#include \"GameHeader.h\"");
	line(source, "");
	FOR_I(cmdCount)
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
			line(header, "\tvoid fillParams() override{}");
		}
		else
		{
			line(header, "\tvoid fillParams() override");
			line(header, "\t{");
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
	// 如果下一行是宏,则说明是旧格式的文件,需要被替换
	if (findSubstr(headerLines[bracketIndex + 1], packetName + "_Declare;"))
	{
		myVector<string> newLines;
		newLines.push_back("public:");
		FOR_VECTOR_CONST(memberList)
		{
			newLines.push_back("\t" + cppMemberDeclareString(memberList[i]));
		}
		// 宏的下一行应该是public:,否则是无法识别的格式
		if (headerLines[bracketIndex + 2] != "public:")
		{
			ERROR("网络消息包文件解析错误,宏的下一行应该是public:");
			return;
		}
		newLines.push_back("public:");
		if (memberListCount == 0)
		{
			newLines.push_back("\tvoid fillParams() override{}");
		}
		else
		{
			newLines.push_back("\tvoid fillParams() override");
			newLines.push_back("\t{");
			FOR_I(memberListCount)
			{
				newLines.push_back("\t\t" + cppPushParamString(memberList[i]));
			}
			newLines.push_back("\t}");
		}
		headerLines.erase(bracketIndex + 2);
		headerLines.erase(bracketIndex + 1);
		// 将新加的行插入到原本的文件中
		FOR_VECTOR(newLines)
		{
			headerLines.insert(headerLines.begin() + bracketIndex + 1 + i, newLines[i]);
		}
		END(newLines);
	}
	// 新格式的文件,也就是没有其他的宏,替换变量声明和注册
	else
	{
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
			int endBracketIndex = headerLines.find("\t}", fillParamIndex + 2);
			// 移除所有已注册的变量
			FOR_I(oldVariableCount)
			{
				headerLines.erase(fillParamIndex + 2 + oldVariableCount - 1 - i);
			}
			// 如果没有新的变量,则移除大括号,将大括号合并到函数名所在行
			if (memberListCount == 0)
			{
				headerLines.erase(fillParamIndex + 2);
				headerLines.erase(fillParamIndex + 1);
				headerLines[fillParamIndex] = "\t\tvoid fillParams() override{}";
			}
			else
			{
				// 添加新的变量
				FOR_I(memberListCount)
				{
					headerLines.insert(fillParamIndex + 2 + i, "\t\t" + cppPushParamString(memberList[i]));
				}
			}
		}
		// 函数体没有变量注册
		else
		{
			// 如果新的变量数量也为0,则不做处理,大于0时才处理
			if (memberListCount > 0)
			{
				headerLines[fillParamIndex] += "\t\tvoid fillParams() override";
				headerLines.insert(fillParamIndex + 1, "\t{");
				// 添加新的变量
				FOR_I(memberListCount)
				{
					headerLines.insert(fillParamIndex + 2 + i, "\t\t" + cppPushParamString(memberList[i]));
				}
				headerLines.insert(fillParamIndex + 2 + memberListCount, "\t}");
			}
		}
	}
	// 生成新的文件
	string newFile;
	FOR_VECTOR(headerLines)
	{
		newFile += headerLines[i];
		if (i != headerLinesCount - 1)
		{
			newFile += "\r\n";
		}
	}
	END(headerLines);
	writeFile(fullPath, ANSIToUTF8(newFile.c_str(), true));
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
		line(source, "\tCharacterPlayer* player = (CharacterPlayer*)getCharacter();");
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
			line(header, "\tvoid fillParams() override{}");
		}
		else
		{
			line(header, "\tvoid fillParams() override");
			line(header, "\t{");
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
void CodeNetPacket::generateCSharpPacketRegisteFile(const myVector<PacketInfo>& packetList, string filePath)
{
	string str;
	line(str, "using System;");
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
	line(str, "\t\tmSocketFactoryThread.registePacket(classType, type);");
	line(str, "\t}");
	line(str, "}", false);

	writeFile(filePath + "PacketRegister.cs", ANSIToUTF8(str.c_str(), true));
}

// Packet_Declare.cs文件
void CodeNetPacket::generateCSharpDecalreFile(const PacketInfo& packetInfo, string csFilePath, string scFilePath)
{
	const int prefixLength = 2;
	if (packetInfo.mPacketName.substr(0, prefixLength) != "CS" && packetInfo.mPacketName.substr(0, prefixLength) != "SC")
	{
		ERROR("包名前缀错误");
		return;
	}
	string file;
	line(file, "using System;");
	line(file, "");
	line(file, "public partial class " + packetInfo.mPacketName + " : SocketPacket");
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
		writeFile(csFilePath + packetInfo.mPacketName + "_Declare.cs", ANSIToUTF8(file.c_str(), true));
	}
	else if (startWith(packetInfo.mPacketName, "SC"))
	{
		writeFile(scFilePath + packetInfo.mPacketName + "_Declare.cs", ANSIToUTF8(file.c_str(), true));
	}
}

void CodeNetPacket::generateCSharpSCPacketFile(const string& packetName, string filePath)
{
	if (!startWith(packetName, "SC"))
	{
		return;
	}
	string fullPath = filePath + packetName + ".cs";
	if (!isFileExist(fullPath))
	{
		string file;
		line(file, "using System;");
		line(file, "");
		line(file, "public partial class " + packetName + " : SocketPacket");
		line(file, "{");
		line(file, "\tpublic override void execute()");
		line(file, "\t{");
		line(file, "\t}");
		line(file, "}", false);

		writeFile(fullPath, ANSIToUTF8(file.c_str(), true));
	}
}