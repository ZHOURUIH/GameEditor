#include "CodeNetPacket.h"

void CodeNetPacket::generate()
{
	string cppCSPacketPath = cppGamePath + "Socket/ClientServer/";
	string cppSCPacketPath = cppGamePath + "Socket/ServerClient/";
	string cppPacketDefinePath = cppGamePath + "Socket/";
	string csharpCSGamePath = csGamePath + "Socket/ClientServer/";
	string csharpCSHotfixPath = csHotfixGamePath + "Socket/ClientServer/";
	string csharpSCGamePath = csGamePath + "Socket/ServerClient/";
	string csharpSCHotfixPath = csHotfixGamePath + "Socket/ServerClient/";
	string csharpPacketDefinePath = csHotfixGamePath + "Socket/";

	// 解析模板文件
	myVector<string> csLines = openTxtFileLines("PacketCS.txt");
	myVector<string> scLines = openTxtFileLines("PacketSC.txt");
	if (csLines.size() == 0)
	{
		ERROR("未找到协议文件PacketCS.txt");
		return;
	}
	if (scLines.size() == 0)
	{
		ERROR("未找到协议文件PacketSC.txt");
		return;
	}
	myVector<string> allLines;
	allLines.addRange(csLines);
	allLines.addRange(scLines);
	bool packetStart = false;
	myVector<PacketInfo> packetInfoList;
	myVector<PacketMember> tempMemberList;
	int tempPacketNameLine;
	FOR_VECTOR_CONST(allLines)
	{
		string line = allLines[i];
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
			parsePacketName(allLines[i - 1], info);
			info.mComment = allLines[i - 2];
			packetInfoList.push_back(info);
			continue;
		}
		// 成员变量列表起始
		if (line == "{")
		{
			packetStart = true;
			tempPacketNameLine = i - 1;
			tempMemberList.clear();
			continue;
		}
		// 成员变量列表结束
		if (line == "}")
		{
			if (!packetStart)
			{
				cout << "未找到前一个匹配的{, NetPacket,前5行内容:" << endl;
				int printStartLine = (int)i - 5;
				clampMin(printStartLine, 0);
				for (int j = printStartLine; j <= (int)i; ++j)
				{
					cout << allLines[j] << endl;
				}
			}
			PacketInfo info;
			parsePacketName(allLines[tempPacketNameLine], info);
			info.mMemberList = tempMemberList;
			info.mComment = allLines[tempPacketNameLine - 1];
			packetInfoList.push_back(info);
			packetStart = false;
			tempMemberList.clear();
			tempPacketNameLine = -1;
			continue;
		}
		if (packetStart)
		{
			tempMemberList.push_back(parseMemberLine(line));
		}
	}

	// 打开服务器的消息注册文件,找到当前的消息版本号,然后版本号自增
	int packetVersion = findPacketVersion(cppPacketDefinePath + "PacketRegister.cpp") + 1;

	myVector<string> hotfixList;
	myVector<string> noHotfixList;
	myVector<string> packetList;
	FOR_VECTOR_CONST(packetInfoList)
	{
		const PacketInfo& packetInfo = packetInfoList[i];
		// 生成代码文件
		// .h.cpp代码
		if (cppGamePath.length() > 0)
		{
			generateCppCSPacketFileHeader(packetInfo, cppCSPacketPath);
			generateCppCSPacketFileSource(packetInfo, cppCSPacketPath);
			generateCppSCPacketFileHeader(packetInfo, cppSCPacketPath);
		}
		if (csGamePath.length() > 0)
		{
			// .cs代码
			generateCSharpPacketFile(packetInfo, csharpCSHotfixPath, csharpCSGamePath, csharpSCHotfixPath, csharpSCGamePath);
		}
		packetList.push_back(packetInfo.mPacketName);
		if (packetInfo.mHotFix)
		{
			hotfixList.push_back(packetInfo.mPacketName);
		}
		else
		{
			noHotfixList.push_back(packetInfo.mPacketName);
		}
	}
	// c++
	if (cppGamePath.length() > 0)
	{
		generateCppPacketDefineFile(packetInfoList, cppPacketDefinePath);
		generateCppPacketRegisteFile(packetInfoList, cppPacketDefinePath, packetVersion);
		generateStringDefinePacket(packetList, cppGameStringDefineFile);

		// 删除无用的消息
		// c++ CS
		myVector<string> cppCSFiles;
		findFiles(cppCSPacketPath, cppCSFiles, NULL, 0);
		FOR_VECTOR(cppCSFiles)
		{
			if (!packetList.contains(getFileNameNoSuffix(cppCSFiles[i], true)))
			{
				deleteFile(cppCSFiles[i]);
			}
		}
		END(cppCSFiles);
		// c++ SC
		myVector<string> cppSCFiles;
		findFiles(cppSCPacketPath, cppSCFiles, NULL, 0);
		FOR_VECTOR(cppSCFiles)
		{
			if (!packetList.contains(getFileNameNoSuffix(cppSCFiles[i], true)))
			{
				deleteFile(cppSCFiles[i]);
			}
		}
		END(cppSCFiles);
	}

	// c#
	if (csGamePath.length() > 0)
	{
		generateCSharpPacketDefineFile(packetInfoList, csharpPacketDefinePath);
		generateCSharpPacketRegisteFile(packetInfoList, csharpPacketDefinePath, packetVersion);

		// c# CS非热更
		myVector<string> csharpCSNoHotfixFiles;
		findFiles(csharpCSGamePath, csharpCSNoHotfixFiles, ".cs");
		FOR_VECTOR(csharpCSNoHotfixFiles)
		{
			const string& file = csharpCSNoHotfixFiles[i];
			if (!noHotfixList.contains(getFileNameNoSuffix(file, true)))
			{
				deleteFile(file);
				deleteFile(file + ".meta");
			}
		}
		END(csharpCSNoHotfixFiles);
		// c# CS热更
		myVector<string> csharpCSHotfixFiles;
		findFiles(csharpCSHotfixPath, csharpCSHotfixFiles, ".cs");
		FOR_VECTOR(csharpCSHotfixFiles)
		{
			const string& file = csharpCSHotfixFiles[i];
			if (!hotfixList.contains(getFileNameNoSuffix(file, true)))
			{
				deleteFile(file);
				deleteFile(file + ".meta");
			}
		}
		END(csharpCSHotfixFiles);
		// c# SC非热更
		myVector<string> csharpSCNoHotfixFiles;
		findFiles(csharpSCGamePath, csharpSCNoHotfixFiles, ".cs");
		FOR_VECTOR(csharpSCNoHotfixFiles)
		{
			const string& file = csharpSCNoHotfixFiles[i];
			if (!noHotfixList.contains(getFileNameNoSuffix(file, true)))
			{
				deleteFile(file);
				deleteFile(file + ".meta");
			}
		}
		END(csharpSCNoHotfixFiles);
		// c# SC热更
		myVector<string> csharpSCHotfixFiles;
		findFiles(csharpSCHotfixPath, csharpSCHotfixFiles, ".cs");
		FOR_VECTOR(csharpSCHotfixFiles)
		{
			const string& file = csharpSCHotfixFiles[i];
			if (!hotfixList.contains(getFileNameNoSuffix(file, true)))
			{
				deleteFile(file);
				deleteFile(file + ".meta");
			}
		}
		END(csharpSCHotfixFiles);
	}
}

// PacketDefine.h文件
void CodeNetPacket::generateCppPacketDefineFile(const myVector<PacketInfo>& packetList, const string& filePath)
{
	string str;
	line(str, "#pragma once");
	line(str, "");
	line(str, "#include \"FrameDefine.h\"");
	line(str, "");
	line(str, "class PACKET_TYPE");
	line(str, "{");
	line(str, "public:");
	line(str, "\tconstexpr static ushort MIN = 0;");
	line(str, "");
	line(str, "\tconstexpr static ushort GATE_CS_MIN = 1000;");
	line(str, "\tconstexpr static ushort GATE_CS_HEART_BEAT = 1001;");
	line(str, "\tconstexpr static ushort GATE_CS_SERVER_INFO = 1002;");
	line(str, "");
	line(str, "\tconstexpr static ushort GATE_SC_MIN = 1200;");
	line(str, "\tconstexpr static ushort GATE_SC_HEART_BEAT = 1201;");
	line(str, "");
	line(str, "\tconstexpr static ushort MAIL_CS_MIN = 1500;");
	line(str, "\tconstexpr static ushort MAIL_CS_HEART_BEAT = 1501;");
	line(str, "\tconstexpr static ushort MAIL_CS_SEND_MAIL = 1502;");
	line(str, "");
	line(str, "\tconstexpr static ushort MAIL_SC_MIN = 2000;");
	line(str, "\tconstexpr static ushort MAIL_SC_HEART_BEAT = 2001;");
	line(str, "");
	int csMinValue = 3000;
	line(str, "\tconstexpr static ushort CS_MIN = " + intToString(csMinValue) + ";");
	uint packetCount = packetList.size();
	FOR_I(packetCount)
	{
		if (startWith(packetList[i].mPacketName, "CS"))
		{
			line(str, "\tconstexpr static ushort " + packetNameToUpper(packetList[i].mPacketName) + " = " + intToString(++csMinValue) + ";");
		}
	}
	line(str, "");
	int scMinValue = 6000;
	line(str, "\tconstexpr static ushort SC_MIN = " + intToString(scMinValue) + ";");
	FOR_I(packetCount)
	{
		if (startWith(packetList[i].mPacketName, "SC"))
		{
			line(str, "\tconstexpr static ushort " + packetNameToUpper(packetList[i].mPacketName) + " = " + intToString(++scMinValue) + ";");
		}
	}
	line(str, "};", false);

	writeFile(filePath + "PacketDefine.h", ANSIToUTF8(str.c_str(), true));
}

// PacketRegister.cpp文件
void CodeNetPacket::generateCppPacketRegisteFile(const myVector<PacketInfo>& packetList, const string& filePath, int packetVersion)
{
	string str;
	line(str, "#include \"GameHeader.h\"");
	line(str, "");
	line(str, "int PacketRegister::PACKET_VERSION = " + intToString(packetVersion) + ";");
	line(str, "void PacketRegister::registeAll()");
	line(str, "{");
	myVector<PacketInfo> udpCSList;
	uint packetCount = packetList.size();
	FOR_I(packetCount)
	{
		const string& packetName = packetList[i].mPacketName;
		if (!startWith(packetName, "CS"))
		{
			continue;
		}
		line(str, "\tFrameBase::mPacketFactoryManager->addFactory<" + packetName + ">(PACKET_TYPE::" + packetNameToUpper(packetName) + ", NAME(" + packetName + "));");
		if (packetList[i].mUDP)
		{
			udpCSList.push_back(packetList[i]);
		}
	}
	line(str, "");
	myVector<PacketInfo> udpSCList;
	FOR_I(packetCount)
	{
		const string& packetName = packetList[i].mPacketName;
		if (!startWith(packetName, "SC"))
		{
			continue;
		}
		line(str, "\tFrameBase::mPacketFactoryManager->addFactory<" + packetName + ">(PACKET_TYPE::" + packetNameToUpper(packetName) + ", NAME(" + packetName + "));");
		if (packetList[i].mUDP)
		{
			udpSCList.push_back(packetList[i]);
		}
	}
	if (udpCSList.size() > 0)
	{
		line(str, "");
		FOR_VECTOR(udpCSList)
		{
			line(str, "\tFrameBase::mPacketFactoryManager->addUDPPacket(PACKET_TYPE::" + packetNameToUpper(udpCSList[i].mPacketName) + "); ");
		}
		END(udpCSList);
	}
	if (udpSCList.size() > 0)
	{
		line(str, "");
		FOR_VECTOR(udpSCList)
		{
			line(str, "\tFrameBase::mPacketFactoryManager->addUDPPacket(PACKET_TYPE::" + packetNameToUpper(udpSCList[i].mPacketName) + "); ");
		}
		END(udpSCList);
	}
	line(str, "};", false);

	writeFile(filePath + "PacketRegister.cpp", ANSIToUTF8(str.c_str(), true));
}

void CodeNetPacket::generateStringDefinePacket(const myVector<string>& packetList, const string& stringDefineFile)
{
	// 更新StringDefine.h的特定部分
	myVector<string> codeList;
	int lineStart = -1;
	if (!findCustomCode(stringDefineFile, codeList, lineStart,
		[](const string& codeLine) { return endWith(codeLine, "// Packet"); },
		[](const string& codeLine) { return codeLine.length() == 0 || findSubstr(codeLine, "}"); }))
	{
		return;
	}

	for (const string& item : packetList)
	{
		codeList.insert(++lineStart, stringDeclare(item));
	}
	writeFile(stringDefineFile, ANSIToUTF8(codeListToString(codeList).c_str(), true));
}

// CSPacket.h和CSPacket.cpp
void CodeNetPacket::generateCppCSPacketFileHeader(const PacketInfo& packetInfo, const string& filePath)
{
	const string& packetName = packetInfo.mPacketName;
	if (!startWith(packetName, "CS"))
	{
		return;
	}

	// CSPacket.h
	string headerFullPath = filePath + packetName + ".h";
	myVector<string> includeList;
	myVector<string> customList;
	findCppIncludeCustomCode(packetName, headerFullPath, includeList, customList);
	string header;
	line(header, "#pragma once");
	line(header, "");
	// #include部分
	FOR_VECTOR(includeList)
	{
		line(header, includeList[i]);
	}
	END(includeList);
	line(header, "");
	line(header, packetInfo.mComment);
	line(header, "class " + packetName + " : public Packet");
	line(header, "{");
	line(header, "public:");
	const auto& memberList = packetInfo.mMemberList;
	FOR_VECTOR_CONST(memberList)
	{
		line(header, "\t" + cppMemberDeclareString(memberList[i]));
	}
	line(header, "public:");
	line(header, "\tvoid fillParams() override");
	line(header, "\t{");
	line(header, "\t\tmShowInfo = " + boolToString(packetInfo.mShowInfo) + ";");
	line(header, "\t\tmExecuteInMain = " + boolToString(packetInfo.mServerExecuteInMain) + ";");
	FOR_I(memberListCount)
	{
		line(header, "\t\t" + cppPushParamString(memberList[i]));
	}
	line(header, "\t}");
	line(header, "\tvoid execute() override;");
	// 自定义代码部分
	FOR_VECTOR(customList)
	{
		line(header, customList[i]);
	}
	END(customList);
	line(header, "};", false);

	writeFile(headerFullPath, ANSIToUTF8(header.c_str(), true));
}

void CodeNetPacket::findCppIncludeCustomCode(const string& packetName, const string& fullPath, myVector<string>& includeList, myVector<string>& customList)
{
	if (isFileExist(fullPath))
	{
		myVector<string> fileLines = openTxtFileLines(fullPath);
		uint lineCount = fileLines.size();
		int customStart = -1;
		int customEnd = -1;
		bool isCSPacket = startWith(packetName, "CS");

		// #include部分
		FOR_I(lineCount)
		{
			if (startWith(fileLines[i], "#include \""))
			{
				includeList.push_back(fileLines[i]);
			}
		}

		// 查找自定义代码的起始
		if (isCSPacket)
		{
			FOR_I(lineCount)
			{
				// CS消息包execute函数声明到类结尾之间是自定义代码部分
				if (fileLines[i] == "\tvoid execute() override;")
				{
					customStart = i + 1;
					break;
				}
			}
		}
		else
		{
			bool findFillParams = false;
			FOR_I(lineCount)
			{
				// SC消息包fillParams函数结尾到类结尾之间是自定义代码部分
				if (fileLines[i] == "\tvoid fillParams() override")
				{
					findFillParams = true;
					continue;
				}
				if (findFillParams && fileLines[i] == "\t}")
				{
					customStart = i + 1;
					break;
				}
			}
		}

		// 查找自定义代码的终止
		if (customStart >= 0)
		{
			FOR_I(lineCount)
			{
				if (fileLines[lineCount - i - 1] == "};")
				{
					customEnd = lineCount - i - 2;
					break;
				}
			}
		}
		// 将自定义代码部分放入列表
		if (customStart >= 0 && customEnd >= 0 && customStart <= customEnd)
		{
			uint customLineCount = customEnd - customStart + 1;
			FOR_I(customLineCount)
			{
				customList.push_back(fileLines[customStart + i]);
			}
		}
	}
	else
	{
		includeList.push_back("#include \"Packet.h\"");
		customList.push_back("\tvoid debugInfo(Array<1024>& buffer) override");
		customList.push_back("\t{");
		customList.push_back("\t\tdebug(buffer, "");");
		customList.push_back("\t}");
	}
}

int CodeNetPacket::findPacketVersion(const string& filePath)
{
	int packetVersion = 0;
	for (const string& line : openTxtFileLines(filePath))
	{
		int index = 0;
		if (findString(line.c_str(), "PACKET_VERSION = ", &index))
		{
			int pos0 = (int)line.find_first_of('=', index);
			int pos1 = (int)line.find_first_of(';', index);
			packetVersion = stringToInt(line.substr(pos0 + 1, pos1 - pos0 - 1));
			break;
		}
	}
	return packetVersion;
}

void CodeNetPacket::generateCppCSPacketFileSource(const PacketInfo& packetInfo, const string& filePath)
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
		line(source, "\tconst CharacterPlayer* player = getPlayer(mClient->getPlayerGUID());");
		line(source, "\tif (player == nullptr)");
		line(source, "\t{");
		line(source, "\t\treturn;");
		line(source, "\t}");
		line(source, "}", false);

		writeFile(sourceFullPath, ANSIToUTF8(source.c_str(), true));
	}
}

// SCPacket.h文件
void CodeNetPacket::generateCppSCPacketFileHeader(const PacketInfo& packetInfo, const string& filePath)
{
	const string& packetName = packetInfo.mPacketName;
	if (!startWith(packetName, "SC"))
	{
		return;
	}

	// SCPacket.h
	string headerFullPath = filePath + packetName + ".h";
	myVector<string> includeList;
	myVector<string> customList;
	findCppIncludeCustomCode(packetName, headerFullPath, includeList, customList);

	string header;
	line(header, "#pragma once");
	line(header, "");
	// #include部分
	FOR_VECTOR(includeList)
	{
		line(header, includeList[i]);
	}
	END(includeList);
	line(header, "");
	line(header, packetInfo.mComment);
	line(header, "class " + packetName + " : public Packet");
	line(header, "{");
	line(header, "public:");
	const auto& memberList = packetInfo.mMemberList;
	FOR_VECTOR_CONST(memberList)
	{
		line(header, "\t" + cppMemberDeclareString(memberList[i]));
	}
	line(header, "public:");
	line(header, "\tvoid fillParams() override");
	line(header, "\t{");
	line(header, "\t\tmShowInfo = " + boolToString(packetInfo.mShowInfo) + ";");
	FOR_I(memberListCount)
	{
		line(header, "\t\t" + cppPushParamString(memberList[i]));
	}
	line(header, "\t}");
	// 自定义代码部分
	FOR_VECTOR(customList)
	{
		line(header, customList[i]);
	}
	END(customList);
	line(header, "};", false);

	writeFile(headerFullPath, ANSIToUTF8(header.c_str(), true));
}

// PacketDefine.cs文件
void CodeNetPacket::generateCSharpPacketDefineFile(const myVector<PacketInfo>& packetList, const string& filePath)
{
	string str;
	line(str, "using System;");
	
	line(str, "");
	line(str, "public class PACKET_TYPE");
	line(str, "{");
	line(str, "\tpublic static ushort MIN = 0;");
	line(str, "");
	int csPacketNumber = 3000;
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
	line(str, "");
	int scPacketNumber = 6000;
	FOR_I(packetCount)
	{
		if (startWith(packetList[i].mPacketName, "SC"))
		{
			++scPacketNumber;
			line(str, "\tpublic static ushort " + packetNameToUpper(packetList[i].mPacketName) + " = " + intToString(scPacketNumber) + ";");
		}
	}
	++scPacketNumber;
	line(str, "}", false);

	writeFile(filePath + "PacketDefine.cs", ANSIToUTF8(str.c_str(), true));
}

// PacketRegister.cs文件
void CodeNetPacket::generateCSharpPacketRegisteFile(const myVector<PacketInfo>& packetList, const string& filePath, int packetVersion)
{
	string str;
	line(str, "using System;");
	line(str, "using static FrameBase;");
	line(str, "");
	line(str, "public class PacketRegister");
	line(str, "{");
	line(str, "\tpublic static int PACKET_VERSION = " + intToString(packetVersion) + ";");
	line(str, "\tpublic static void registeAll()");
	line(str, "\t{");
	myVector<PacketInfo> udpCSList;
	uint packetCount = packetList.size();
	FOR_I(packetCount)
	{
		if (!startWith(packetList[i].mPacketName, "CS"))
		{
			continue;
		}
		line(str, "\t\tregistePacket(typeof(" + packetList[i].mPacketName + "), PACKET_TYPE." + packetNameToUpper(packetList[i].mPacketName) + ");");
		if (packetList[i].mUDP)
		{
			udpCSList.push_back(packetList[i]);
		}
	}
	line(str, "");
	myVector<PacketInfo> udpSCList;
	FOR_I(packetCount)
	{
		if (!startWith(packetList[i].mPacketName, "SC"))
		{
			continue;
		}
		line(str, "\t\tregistePacket(typeof(" + packetList[i].mPacketName + "), PACKET_TYPE." + packetNameToUpper(packetList[i].mPacketName) + ");");
		if (packetList[i].mUDP)
		{
			udpSCList.push_back(packetList[i]);
		}
	}
	if (udpCSList.size() > 0)
	{
		line(str, "");
		FOR_VECTOR(udpCSList)
		{
			line(str, "\t\tregisteUDP(PACKET_TYPE." + packetNameToUpper(udpCSList[i].mPacketName) + ", \"" + udpCSList[i].mPacketName + "\");");
		}
		END(udpCSList);
	}
	if (udpSCList.size() > 0)
	{
		line(str, "");
		FOR_VECTOR(udpSCList)
		{
			line(str, "\t\tregisteUDP(PACKET_TYPE." + packetNameToUpper(udpSCList[i].mPacketName) + ", \"" + udpSCList[i].mPacketName + "\");");
		}
		END(udpSCList);
	}
	line(str, "\t}");
	line(str, "\tprotected static void registePacket(Type classType, ushort type)");
	line(str, "\t{");
	line(str, "\t\tmNetPacketTypeManager.registePacket(classType, type);");
	line(str, "\t}");
	line(str, "\tprotected static void registeUDP(ushort type, string packetName)");
	line(str, "\t{");
	line(str, "\t\tmNetPacketTypeManager.registeUDPPacketName(type, packetName);");
	line(str, "\t}");
	line(str, "}", false);

	writeFile(filePath + "PacketRegister.cs", ANSIToUTF8(str.c_str(), true));
}

void CodeNetPacket::generateCSharpPacketFile(const PacketInfo& packetInfo, const string& csFileHotfixPath, const string& csFileGamePath, const string& scFileHotfixPath, const string& scFileGamePath)
{
	string packetName = packetInfo.mPacketName;
	if (!startWith(packetName, "CS") && !startWith(packetName, "SC"))
	{
		return;
	}

	myVector<string> usingList;
	myVector<string> customList;
	string fullPath;
	if (startWith(packetName, "CS"))
	{
		fullPath = packetInfo.mHotFix ? csFileHotfixPath + packetName + ".cs" : csFileGamePath + packetName + ".cs";
	}
	else if (startWith(packetName, "SC"))
	{
		fullPath = packetInfo.mHotFix ? scFileHotfixPath + packetName + ".cs" : scFileGamePath + packetName + ".cs";
	}

	findCSharpUsingListCustomCode(packetName, fullPath, usingList, customList);

	string file;
	// using部分
	FOR_VECTOR(usingList)
	{
		line(file, usingList[i]);
	}
	END(usingList);
	line(file, "");
	// 固定格式部分
	line(file, packetInfo.mComment);
	line(file, "public class " + packetName + " : NetPacketFrame");
	line(file, "{");
	uint memberCount = packetInfo.mMemberList.size();
	FOR_I(memberCount)
	{
		line(file, "\t" + cSharpMemberDeclareString(packetInfo.mMemberList[i], packetInfo.mHotFix));
	}
	if (memberCount > 0)
	{
		// fillParams
		line(file, "\tprotected override void fillParams()");
		line(file, "\t{");
		FOR_I(memberCount)
		{
			line(file, "\t\t" + cSharpPushParamString(packetInfo.mMemberList[i]));
		}
		line(file, "\t}");
	}
	// 自定义代码部分
	FOR_VECTOR(customList)
	{
		line(file, customList[i]);
	}
	END(customList);
	line(file, "}", false);

	writeFile(fullPath, ANSIToUTF8(file.c_str(), true));
}

void CodeNetPacket::findCSharpUsingListCustomCode(const string& packetName, const string& fullPath, myVector<string>& usingList, myVector<string>& customList)
{
	if (isFileExist(fullPath))
	{
		myVector<string> fileLists = openTxtFileLines(fullPath);
		// 命名空间部分
		uint lines = fileLists.size();
		FOR_I(lines)
		{
			if (!startWith(fileLists[i], "using "))
			{
				break;
			}
			usingList.push_back(fileLists[i]);
		}

		// 自定义函数体部分,从fillParams函数结束到类结束之间都是自定义函数部分
		int customStart = -1;
		int customEnd = -1;
		bool findFillParams = false;
		FOR_I(lines)
		{
			if (fileLists[i] == "\tprotected override void fillParams()")
			{
				findFillParams = true;
				continue;
			}
			if (findFillParams && fileLists[i] == "\t}")
			{
				customStart = i + 1;
				break;
			}
		}
		FOR_I(lines)
		{
			if (fileLists[lines - i - 1] == "}")
			{
				customEnd = lines - i - 2;
				break;
			}
		}

		// 如果没有fillParams的函数定义,则说明没有参数,那类体全部都是自定义代码
		if (customStart < 0 || customEnd < 0)
		{
			FOR_I(lines)
			{
				if (startWith(fileLists[i], "public class "))
				{
					if (fileLists[i + 1] == "{}")
					{
						break;
					}
					else if (fileLists[i + 1] == "{")
					{
						customStart = i + 2;
						continue;
					}
				}
				if (customStart >= 0 && fileLists[i] == "}")
				{
					customEnd = i - 1;
					break;
				}
			}
		}
		// 将找到的自定义代码保存到列表中
		if (customStart >= 0 && customEnd >= 0 && customStart <= customEnd)
		{
			int customCount = customEnd - customStart + 1;
			FOR_I((uint)customCount)
			{
				customList.push_back(fileLists[customStart + i]);
			}
		}
	}
	else
	{
		usingList.push_back("using System;");
		usingList.push_back("using static GD;");
		if (startWith(packetName, "SC"))
		{
			usingList.push_back("using static GBR;");
			customList.push_back("\tpublic override void execute()");
			customList.push_back("\t{}");
		}
	}
}