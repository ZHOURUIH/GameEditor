#include "CodeNetPacket.h"

void CodeNetPacket::generate()
{
	print("正在生成网络消息");
	myVector<PacketStruct> structInfoList;
	myVector<PacketInfo> packetInfoList;
	parsePacketConfig(structInfoList, packetInfoList);

	int packetVersion = 0;
	generateCpp(structInfoList, packetInfoList, packetVersion);

	if (!csGamePath.empty())
	{
		generateCSharp(structInfoList, packetInfoList, packetVersion);
	}
	print("完成生成网络消息");
	print("");
}

void CodeNetPacket::generateVirtualClient()
{
	if (VirtualClientSocketPath.empty())
	{
		ERROR("未配置虚拟客户端项目路径");
		return;
	}
	print("正在生成虚拟客户端网络消息");
	myVector<PacketStruct> structInfoList;
	myVector<PacketInfo> packetInfoList;
	parsePacketConfig(structInfoList, packetInfoList);

	int packetVersion = findPacketVersion(cppGamePath + "Socket/GamePacketRegister.cpp");
	generateCSharpVirtualClient(structInfoList, packetInfoList, packetVersion);
	print("完成生成虚拟客户端网络消息");
	print("");
}

void CodeNetPacket::parsePacketConfig(myVector<PacketStruct>& structInfoList, myVector<PacketInfo>& packetInfoList)
{
	// 解析模板文件
	myVector<string> csLines = openTxtFileLines("PacketCS.txt");
	myVector<string> scLines = openTxtFileLines("PacketSC.txt");
	myVector<string> structLines = openTxtFileLines("PacketStruct.txt");
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

	// 解析结构体定义
	bool structStart = false;
	myVector<PacketMember> tempStructMemberList;
	int tempStructNameLine = 0;
	FOR_VECTOR(structLines)
	{
		string line = structLines[i];
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
			PacketStruct info;
			parseStructName(structLines[i - 1], info);
			info.mComment = structLines[i - 2];
			structInfoList.push_back(info);
			continue;
		}
		// 成员变量列表起始
		if (line == "{")
		{
			structStart = true;
			tempStructNameLine = i - 1;
			tempStructMemberList.clear();
			continue;
		}
		// 成员变量列表结束
		if (line == "}")
		{
			if (!structStart)
			{
				ERROR("未找到前一个匹配的{, PacketStruct,前5行内容:");
				int printStartLine = (int)i - 5;
				clampMin(printStartLine, 0);
				for (int j = printStartLine; j <= (int)i; ++j)
				{
					ERROR(structLines[j]);
				}
			}
			PacketStruct info;
			parseStructName(structLines[tempStructNameLine], info);
			info.mMemberList = tempStructMemberList;
			info.mComment = structLines[tempStructNameLine - 1];
			structInfoList.push_back(info);
			structStart = false;
			tempStructMemberList.clear();
			tempStructNameLine = -1;
			continue;
		}
		if (structStart)
		{
			tempStructMemberList.push_back(parseMemberLine(line));
		}
	}

	// 解析消息定义
	myVector<string> allLines;
	allLines.addRange(csLines);
	allLines.addRange(scLines);
	bool packetStart = false;
	myVector<PacketMember> tempMemberList;
	int tempPacketNameLine = 0;
	FOR_VECTOR(allLines)
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
				ERROR("未找到前一个匹配的{, NetPacket,前5行内容:");
				int printStartLine = (int)i - 5;
				clampMin(printStartLine, 0);
				for (int j = printStartLine; j <= (int)i; ++j)
				{
					ERROR(allLines[j]);
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
}

void CodeNetPacket::generateCpp(const myVector<PacketStruct>& structInfoList, const myVector<PacketInfo>& packetInfoList, int& packetVersion)
{
	myVector<PacketInfo> gamePacketList;
	myVector<PacketInfo> gameCorePacketList;
	myVector<string> gamePacketNameList;
	myVector<string> gameCorePacketNameList;
	for (const PacketInfo& packetInfo : packetInfoList)
	{
		if (packetInfo.mOwner == PACKET_OWNER::GAME)
		{
			gamePacketNameList.push_back(packetInfo.mPacketName);
			gamePacketList.push_back(packetInfo);
		}
		else
		{
			gameCorePacketNameList.push_back(packetInfo.mPacketName);
			gameCorePacketList.push_back(packetInfo);
		}
	}

	myVector<PacketStruct> gameStructList;
	myVector<PacketStruct> gameCoreStructList;
	for (const auto& info : structInfoList)
	{
		if (info.mOwner == PACKET_OWNER::GAME)
		{
			gameStructList.push_back(info);
		}
		else
		{
			gameCoreStructList.push_back(info);
		}
	}
	// Game层的消息
	string cppGameCSPacketPath = cppGamePath + "Socket/ClientServer/";
	string cppGameSCPacketPath = cppGamePath + "Socket/ServerClient/";
	string cppGameStructPath = cppGamePath + "Socket/Struct/";
	string cppGamePacketDefinePath = cppGamePath + "Socket/";
	// 删除无用的消息
	// c++ CS
	myVector<string> cppGameCSFiles;
	findFiles(cppGameCSPacketPath, cppGameCSFiles, nullptr, 0);
	for (int i = 0; i < cppGameCSFiles.size(); ++i)
	{
		if (!gamePacketNameList.contains(getFileNameNoSuffix(cppGameCSFiles[i], true)))
		{
			deleteFile(cppGameCSFiles[i]);
			cppGameCSFiles.erase(i--);
		}
	}
	// c++ SC
	myVector<string> cppGameSCFiles;
	findFiles(cppGameSCPacketPath, cppGameSCFiles, nullptr, 0);
	for (int i = 0; i < cppGameSCFiles.size(); ++i)
	{
		if (!gamePacketNameList.contains(getFileNameNoSuffix(cppGameSCFiles[i], true)))
		{
			deleteFile(cppGameSCFiles[i]);
			cppGameSCFiles.erase(i--);
		}
	}

	// 打开服务器的消息注册文件,找到当前的消息版本号,然后版本号自增
	packetVersion = findPacketVersion(cppGamePacketDefinePath + "GamePacketRegister.cpp") + 1;
	// 生成c++代码
	for (const PacketInfo& packetInfo : gamePacketList)
	{
		// 找到有没有此文件,有就在原来的文件上修改
		string csHeaderPath = cppGameCSPacketPath;
		string csSourcePath = cppGameCSPacketPath;
		string scHeaderPath = cppGameSCPacketPath;
		for (const string& file : cppGameCSFiles)
		{
			if (endWith(file, packetInfo.mPacketName + ".h"))
			{
				csHeaderPath = getFilePath(file) + "/";
			}
			if (endWith(file, packetInfo.mPacketName + ".cpp"))
			{
				csSourcePath = getFilePath(file) + "/";
			}
		}
		for (const string& file : cppGameSCFiles)
		{
			if (endWith(file, packetInfo.mPacketName + ".h"))
			{
				scHeaderPath = getFilePath(file) + "/";
			}
		}
		generateCppCSPacketFileHeader(packetInfo, csHeaderPath);
		generateCppCSPacketFileSource(packetInfo, csSourcePath);
		generateCppSCPacketFileHeader(packetInfo, scHeaderPath);
		generateCppSCPacketFileSource(packetInfo, scHeaderPath);
	}
	generateCppGamePacketDefineFile(gamePacketList, cppGamePacketDefinePath);
	generateCppGamePacketRegisteFile(gamePacketList, cppGamePacketDefinePath, packetVersion);

	for (const PacketStruct& info : gameStructList)
	{
		generateCppStruct(info, cppGameStructPath);
	}

	// GameCore层的消息
	string cppGameCoreCSPacketPath = cppGameCorePath + "Socket/ClientServer/";
	string cppGameCoreSCPacketPath = cppGameCorePath + "Socket/ServerClient/";
	string cppGameCoreStructPath = cppGameCorePath + "Socket/Struct/";
	string cppGameCorePacketDefinePath = cppGameCorePath + "Socket/";
	// 删除无用的消息
	// c++ CS
	myVector<string> cppGameCoreCSFiles;
	findFiles(cppGameCoreCSPacketPath, cppGameCoreCSFiles, nullptr, 0);
	for (int i = 0; i < cppGameCoreCSFiles.size(); ++i)
	{
		if (!gameCorePacketNameList.contains(getFileNameNoSuffix(cppGameCoreCSFiles[i], true)))
		{
			deleteFile(cppGameCoreCSFiles[i]);
			cppGameCoreCSFiles.erase(i--);
		}
	}
	// c++ SC
	myVector<string> cppGameCoreSCFiles;
	findFiles(cppGameCoreSCPacketPath, cppGameCoreSCFiles, nullptr, 0);
	for (int i = 0; i < cppGameCoreSCFiles.size(); ++i)
	{
		if (!gameCorePacketNameList.contains(getFileNameNoSuffix(cppGameCoreSCFiles[i], true)))
		{
			deleteFile(cppGameCoreSCFiles[i]);
			cppGameCoreSCFiles.erase(i--);
		}
	}

	// 生成c++代码
	for (const PacketInfo& packetInfo : gameCorePacketList)
	{
		// 找到有没有此文件,有就在原来的文件上修改
		string csHeaderPath = cppGameCoreCSPacketPath;
		string csSourcePath = cppGameCoreCSPacketPath;
		string scHeaderPath = cppGameCoreSCPacketPath;
		for (const string& file : cppGameCoreCSFiles)
		{
			if (endWith(file, packetInfo.mPacketName + ".h"))
			{
				csHeaderPath = getFilePath(file) + "/";
			}
			if (endWith(file, packetInfo.mPacketName + ".cpp"))
			{
				csSourcePath = getFilePath(file) + "/";
			}
		}
		for (const string& file : cppGameCoreSCFiles)
		{
			if (endWith(file, packetInfo.mPacketName + ".h"))
			{
				scHeaderPath = getFilePath(file) + "/";
			}
		}
		generateCppCSPacketFileHeader(packetInfo, csHeaderPath);
		generateCppCSPacketFileSource(packetInfo, csSourcePath);
		generateCppSCPacketFileHeader(packetInfo, scHeaderPath);
		generateCppSCPacketFileSource(packetInfo, scHeaderPath);
	}
	generateCppGameCorePacketDefineFile(gameCorePacketList, cppGameCorePacketDefinePath);
	generateCppGameCorePacketRegisteFile(gameCorePacketList, cppGameCorePacketDefinePath);

	for (const PacketStruct& info : gameCoreStructList)
	{
		generateCppStruct(info, cppGameCoreStructPath);
	}
}

void CodeNetPacket::generateCSharp(const myVector<PacketStruct>& structInfoList, const myVector<PacketInfo>& packetInfoList, const int packetVersion)
{
	string csharpCSGamePath = csGamePath + "Socket/ClientServer/";
	string csharpCSHotfixPath = ClientHotFixPath + "Socket/ClientServer/";
	string csharpSCGamePath = csGamePath + "Socket/ServerClient/";
	string csharpSCHotfixPath = ClientHotFixPath + "Socket/ServerClient/";
	string csharpStructGamePath = csGamePath + "Socket/Struct/";
	string csharpStructHotfixPath = ClientHotFixPath + "Socket/Struct/";
	string csharpPacketDefinePath = ClientHotFixPath + "Socket/";

	myVector<PacketInfo> gamePacketList;
	myVector<PacketInfo> gameCorePacketList;
	myVector<string> gamePacketNameList;
	myVector<string> gameCorePacketNameList;
	for (const PacketInfo& packetInfo : packetInfoList)
	{
		if (packetInfo.mOwner == PACKET_OWNER::GAME)
		{
			gamePacketNameList.push_back(packetInfo.mPacketName);
			gamePacketList.push_back(packetInfo);
		}
		else
		{
			gameCorePacketNameList.push_back(packetInfo.mPacketName);
			gameCorePacketList.push_back(packetInfo);
		}
	}

	myVector<string> hotfixList;
	myVector<string> noHotfixList;
	for (const PacketInfo& packetInfo : packetInfoList)
	{
		if (packetInfo.mHotFix)
		{
			hotfixList.push_back(packetInfo.mPacketName);
		}
		else
		{
			noHotfixList.push_back(packetInfo.mPacketName);
		}
	}
	myVector<string> hotfixStructList;
	myVector<string> noHotfixStructList;
	for (const PacketStruct& structInfo : structInfoList)
	{
		if (structInfo.mHotFix)
		{
			hotfixStructList.push_back(structInfo.mStructName);
		}
		else
		{
			noHotfixStructList.push_back(structInfo.mStructName);
		}
	}
	// 删除无用的消息
	// c# CS非热更
	myVector<string> csharpCSNoHotfixFiles;
	findFiles(csharpCSGamePath, csharpCSNoHotfixFiles, ".cs");
	for (const string& file : csharpCSNoHotfixFiles)
	{
		if (!noHotfixList.contains(getFileNameNoSuffix(file, true)))
		{
			deleteFile(file);
			deleteFile(file + ".meta");
		}
	}
	// c# CS热更
	myVector<string> csharpCSHotfixFiles;
	findFiles(csharpCSHotfixPath, csharpCSHotfixFiles, ".cs");
	for (const string& file : csharpCSHotfixFiles)
	{
		if (!hotfixList.contains(getFileNameNoSuffix(file, true)))
		{
			deleteFile(file);
		}
	}
	// c# SC非热更
	myVector<string> csharpSCNoHotfixFiles;
	findFiles(csharpSCGamePath, csharpSCNoHotfixFiles, ".cs");
	for (const string& file : csharpSCNoHotfixFiles)
	{
		if (!noHotfixList.contains(getFileNameNoSuffix(file, true)))
		{
			deleteFile(file);
			deleteFile(file + ".meta");
		}
	}
	// c# SC热更
	myVector<string> csharpSCHotfixFiles;
	findFiles(csharpSCHotfixPath, csharpSCHotfixFiles, ".cs");
	for (const string& file : csharpSCHotfixFiles)
	{
		if (!hotfixList.contains(getFileNameNoSuffix(file, true)))
		{
			deleteFile(file);
		}
	}
	// 删除无用的结构体代码
	myVector<string> csharpGameStructFiles;
	findFiles(csharpStructGamePath, csharpGameStructFiles, ".cs");
	for (const string& file : csharpGameStructFiles)
	{
		if (!noHotfixStructList.contains(getFileNameNoSuffix(file, true)))
		{
			deleteFile(file);
			deleteFile(file + ".meta");
		}
	}
	myVector<string> csharpHotfixStructFiles;
	findFiles(csharpStructHotfixPath, csharpHotfixStructFiles, ".cs");
	for (const string& file : csharpHotfixStructFiles)
	{
		if (!hotfixStructList.contains(getFileNameNoSuffix(file, true)))
		{
			deleteFile(file);
		}
	}

	// 生成cs代码
	for (const PacketInfo& packetInfo : packetInfoList)
	{
		generateCSharpPacketFile(packetInfo, csharpCSHotfixPath, csharpCSGamePath, csharpSCHotfixPath, csharpSCGamePath);
	}
	generateCSharpPacketDefineFile(gamePacketList, gameCorePacketList, csharpPacketDefinePath);
	generateCSharpPacketRegisteFile(packetInfoList, csharpPacketDefinePath, packetVersion);

	// 生成结构体代码
	for (const PacketStruct& item : structInfoList)
	{
		generateCSharpStruct(item, csharpStructGamePath, csharpStructHotfixPath);
	}
}

void CodeNetPacket::generateCSharpVirtualClient(const myVector<PacketStruct>& structInfoList, const myVector<PacketInfo>& packetInfoList, const int packetVersion)
{
	string csharpCSGamePath = VirtualClientSocketPath + "ClientServer/";
	string csharpSCGamePath = VirtualClientSocketPath + "ServerClient/";
	string csharpStructGamePath = VirtualClientSocketPath + "Struct/";
	string csharpPacketDefinePath = VirtualClientSocketPath;

	myVector<PacketInfo> gamePacketList;
	myVector<PacketInfo> gameCorePacketList;
	myVector<string> packetNameList;
	for (const PacketInfo& packetInfo : packetInfoList)
	{
		packetNameList.push_back(packetInfo.mPacketName);
		if (packetInfo.mOwner == PACKET_OWNER::GAME)
		{
			gamePacketList.push_back(packetInfo);
		}
		else
		{
			gameCorePacketList.push_back(packetInfo);
		}
	}
	myVector<string> structNameList;
	for (const PacketStruct& structInfo : structInfoList)
	{
		structNameList.push_back(structInfo.mStructName);
	}

	// 删除无用的消息
	// CS
	myVector<string> csharpCSFiles;
	findFiles(csharpCSGamePath, csharpCSFiles, ".cs");
	for (const string& file : csharpCSFiles)
	{
		if (!packetNameList.contains(getFileNameNoSuffix(file, true)))
		{
			deleteFile(file);
			deleteFile(file + ".meta");
		}
	}
	// SC
	myVector<string> csharpSCFiles;
	findFiles(csharpSCGamePath, csharpSCFiles, ".cs");
	for (const string& file : csharpSCFiles)
	{
		if (!packetNameList.contains(getFileNameNoSuffix(file, true)))
		{
			deleteFile(file);
			deleteFile(file + ".meta");
		}
	}
	// 删除无用的结构体代码
	myVector<string> csharpStructFiles;
	findFiles(csharpStructGamePath, csharpStructFiles, ".cs");
	for (const string& file : csharpStructFiles)
	{
		if (!structNameList.contains(getFileNameNoSuffix(file, true)))
		{
			deleteFile(file);
		}
	}

	// 生成cs代码
	for (const PacketInfo& packetInfo : packetInfoList)
	{
		generateCSharpPacketFile(packetInfo, csharpCSGamePath, csharpCSGamePath, csharpSCGamePath, csharpSCGamePath);
	}
	generateCSharpPacketDefineFile(gamePacketList, gameCorePacketList, csharpPacketDefinePath);
	generateCSharpPacketRegisteFile(packetInfoList, csharpPacketDefinePath, packetVersion);

	// 生成结构体代码
	for (const PacketStruct& item : structInfoList)
	{
		generateCSharpStruct(item, csharpStructGamePath, csharpStructGamePath);
	}
}

// PacketDefine.h文件
void CodeNetPacket::generateCppGamePacketDefineFile(const myVector<PacketInfo>& packetList, const string& filePath)
{
	const string fullPath = filePath + "GamePacketDefine.h";
	myVector<string> generateList;
	generateList.push_back("\tconstexpr static ushort MIN = 0;");
	generateList.push_back("");
	int csMinValue = 10000;
	generateList.push_back("\tconstexpr static ushort CS_MIN = " + intToString(csMinValue) + ";");
	uint packetCount = packetList.size();
	FOR_I(packetCount)
	{
		if (startWith(packetList[i].mPacketName, "CS"))
		{
			generateList.push_back("\tconstexpr static ushort " + packetList[i].mPacketName + " = " + intToString(++csMinValue) + ";");
		}
	}
	generateList.push_back("");
	int scMinValue = 20000;
	generateList.push_back("\tconstexpr static ushort SC_MIN = " + intToString(scMinValue) + ";");
	FOR_I(packetCount)
	{
		if (startWith(packetList[i].mPacketName, "SC"))
		{
			generateList.push_back("\tconstexpr static ushort " + packetList[i].mPacketName + " = " + intToString(++scMinValue) + ";");
		}
	}
	if (isFileExist(fullPath))
	{
		myVector<string> codeList;
		int lineStart = -1;
		if (!findCustomCode(fullPath, codeList, lineStart,
			[](const string& codeLine) { return endWith(codeLine, "// auto generate start"); },
			[](const string& codeLine) { return endWith(codeLine, "// auto generate end"); }))
		{
			return;
		}
		for (const string& line : generateList)
		{
			codeList.insert(++lineStart, line);
		}
		writeFile(fullPath, ANSIToUTF8(codeListToString(codeList).c_str(), true));
	}
	else
	{
		string str;
		line(str, "#pragma once");
		line(str, "");
		line(str, "#include \"FrameDefine.h\"");
		line(str, "");
		line(str, "class PACKET_TYPE");
		line(str, "{");
		line(str, "public:");
		for (const string& code : generateList)
		{
			line(str, code);
		}
		line(str, "};", false);

		writeFile(fullPath, ANSIToUTF8(str.c_str(), true));
	}
}

void CodeNetPacket::generateCppGameCorePacketDefineFile(const myVector<PacketInfo>& packetList, const string& filePath)
{
	const string fullPath = filePath + "GameCorePacketDefine.h";
	myVector<string> generateList;
	generateList.push_back("\tconstexpr static ushort MIN = 0;");
	generateList.push_back("");
	int csMinValue = 3000;
	generateList.push_back("\tconstexpr static ushort CS_MIN = " + intToString(csMinValue) + ";");
	uint packetCount = packetList.size();
	FOR_I(packetCount)
	{
		if (startWith(packetList[i].mPacketName, "CS"))
		{
			generateList.push_back("\tconstexpr static ushort " + packetList[i].mPacketName + " = " + intToString(++csMinValue) + ";");
		}
	}
	generateList.push_back("");
	int scMinValue = 6000;
	generateList.push_back("\tconstexpr static ushort SC_MIN = " + intToString(scMinValue) + ";");
	FOR_I(packetCount)
	{
		if (startWith(packetList[i].mPacketName, "SC"))
		{
			generateList.push_back("\tconstexpr static ushort " + packetList[i].mPacketName + " = " + intToString(++scMinValue) + ";");
		}
	}
	if (isFileExist(fullPath))
	{
		myVector<string> codeList;
		int lineStart = -1;
		if (!findCustomCode(fullPath, codeList, lineStart,
			[](const string& codeLine) { return endWith(codeLine, "// auto generate start"); },
			[](const string& codeLine) { return endWith(codeLine, "// auto generate end"); }))
		{
			return;
		}
		for (const string& line : generateList)
		{
			codeList.insert(++lineStart, line);
		}
		writeFile(fullPath, ANSIToUTF8(codeListToString(codeList).c_str(), true));
	}
	else
	{
		string str;
		line(str, "#pragma once");
		line(str, "");
		line(str, "#include \"FrameDefine.h\"");
		line(str, "");
		line(str, "class MICRO_LEGEND_CORE_API PACKET_TYPE_CORE");
		line(str, "{");
		line(str, "public:");
		for (const string& code : generateList)
		{
			line(str, code);
		}
		line(str, "};", false);

		writeFile(filePath + "GameCorePacketDefine.h", ANSIToUTF8(str.c_str(), true));
	}
}

// PacketRegister.cpp文件
void CodeNetPacket::generateCppGamePacketRegisteFile(const myVector<PacketInfo>& packetList, const string& filePath, int packetVersion)
{
	string str;
	line(str, "#include \"GameHeader.h\"");
	
	line(str, "");
	line(str, "int GamePacketRegister::PACKET_VERSION = " + intToString(packetVersion) + ";");
	line(str, "void GamePacketRegister::registeAll()");
	
	line(str, "{");
	myVector<PacketInfo> udpCSList;
	for (const auto& info : packetList)
	{
		if (startWith(info.mPacketName, "CS") && info.mUDP)
		{
			udpCSList.push_back(info);
		}
	}
	for (const auto& info : packetList)
	{
		const string& packetName = info.mPacketName;
		if (!startWith(packetName, "CS"))
		{
			continue;
		}
		line(str, "\tmPacketFactoryManager->addFactory<" + packetName + ">(PACKET_TYPE::" + packetName + ");");
	}
	line(str, "");
	myVector<PacketInfo> udpSCList;
	for (const auto& info : packetList)
	{
		if (!startWith(info.mPacketName, "SC"))
		{
			continue;
		}
		if (info.mUDP)
		{
			udpSCList.push_back(info);
		}
	}
	for (const auto& info : packetList)
	{
		const string& packetName = info.mPacketName;
		if (!startWith(packetName, "SC"))
		{
			continue;
		}
		line(str, "\tmPacketFactoryManager->addFactory<" + packetName + ">(PACKET_TYPE::" + packetName + ");");
	}
	if (udpCSList.size() > 0)
	{
		line(str, "");
		for (const auto& info : udpCSList)
		{
			line(str, "\tmPacketFactoryManager->addUDPPacket(PACKET_TYPE::" + info.mPacketName + "); ");
		}
	}
	if (udpSCList.size() > 0)
	{
		line(str, "");
		for (const auto& info : udpSCList)
		{
			line(str, "\tmPacketFactoryManager->addUDPPacket(PACKET_TYPE::" + info.mPacketName + "); ");
		}
	}
	line(str, "};", false);

	writeFile(filePath + "GamePacketRegister.cpp", ANSIToUTF8(str.c_str(), true));
}

void CodeNetPacket::generateCppGameCorePacketRegisteFile(const myVector<PacketInfo>& packetList, const string& filePath)
{
	string str;
	line(str, "#include \"GameCoreHeader.h\"");

	line(str, "");
	line(str, "void GameCorePacketRegister::registeAll()");

	line(str, "{");
	myVector<PacketInfo> udpCSList;
	for (const auto& info : packetList)
	{
		if (startWith(info.mPacketName, "CS") && info.mUDP)
		{
			udpCSList.push_back(info);
		}
	}
	for (const auto& info : packetList)
	{
		const string& packetName = info.mPacketName;
		if (!startWith(packetName, "CS"))
		{
			continue;
		}
		line(str, "\tmPacketFactoryManager->addFactory<" + packetName + ">(PACKET_TYPE_CORE::" + packetName + ");");
	}
	line(str, "");
	myVector<PacketInfo> udpSCList;
	for (const auto& info : packetList)
	{
		if (!startWith(info.mPacketName, "SC"))
		{
			continue;
		}
		if (info.mUDP)
		{
			udpSCList.push_back(info);
		}
	}
	for (const auto& info : packetList)
	{
		const string& packetName = info.mPacketName;
		if (!startWith(packetName, "SC"))
		{
			continue;
		}
		line(str, "\tmPacketFactoryManager->addFactory<" + packetName + ">(PACKET_TYPE_CORE::" + packetName + ");");
	}
	if (udpCSList.size() > 0)
	{
		line(str, "");
		for (const auto& info : udpCSList)
		{
			line(str, "\tmPacketFactoryManager->addUDPPacket(PACKET_TYPE_CORE::" + info.mPacketName + "); ");
		}
	}
	if (udpSCList.size() > 0)
	{
		line(str, "");
		for (const auto& info : udpSCList)
		{
			line(str, "\tmPacketFactoryManager->addUDPPacket(PACKET_TYPE_CORE::" + info.mPacketName + "); ");
		}
	}
	line(str, "};", false);

	writeFile(filePath + "GameCorePacketRegister.cpp", ANSIToUTF8(str.c_str(), true));
}

// CSPacket.h
void CodeNetPacket::generateCppCSPacketFileHeader(const PacketInfo& packetInfo, const string& filePath)
{
	const string& packetName = packetInfo.mPacketName;
	if (!startWith(packetName, "CS"))
	{
		return;
	}

	bool hasOptional = false;
	for (const auto& item : packetInfo.mMemberList)
	{
		if (item.mOptional)
		{
			hasOptional = true;
			break;
		}
	}

	myVector<string> generateCodes;
	generateCodes.push_back(packetInfo.mComment);
	if (packetInfo.mOwner == PACKET_OWNER::GAME_CORE)
	{
		generateCodes.push_back("class MICRO_LEGEND_CORE_API " + packetName + " : public Packet");
	}
	else
	{
		generateCodes.push_back("class " + packetName + " : public Packet");
	}
	generateCodes.push_back("{");
	generateCodes.push_back("\tBASE(" + packetName + ", Packet);");
	if (hasOptional)
	{
		generateCodes.push_back("public:");
		generateCodes.push_back("\tenum class Field : byte");
		generateCodes.push_back("\t{");
		FOR_I(packetInfo.mMemberList.size())
		{
			const auto& item = packetInfo.mMemberList[i];
			if (item.mOptional)
			{
				if (i >= 64)
				{
					ERROR("可选字段的下标不能超过63");
					break;
				}
				generateCodes.push_back("\t\t" + item.mMemberNameNoPrefix + " = " + intToString(i) + ",");
			}
		}
		generateCodes.push_back("\t};");
	}
	generateCodes.push_back("public:");
	generateCppPacketMemberDeclare(packetInfo.mMemberList, generateCodes);
	generateCodes.push_back("public:");
	generateCodes.push_back("\t" + packetName + "()");
	generateCodes.push_back("\t{");
	if (packetInfo.mOwner == PACKET_OWNER::GAME_CORE)
	{
		generateCodes.push_back("\t\tmType = PACKET_TYPE_CORE::" + packetName + ";");
	}
	else
	{
		generateCodes.push_back("\t\tmType = PACKET_TYPE::" + packetName + ";");
	}
	generateCodes.push_back("\t\tmShowInfo = " + boolToString(packetInfo.mShowInfo) + ";");
	generateCodes.push_back("\t}");
	generateCppPacketReadWrite(packetInfo, generateCodes);
	generateCodes.push_back("\tvoid execute() override;");

	// CSPacket.h
	string headerFullPath = filePath + packetName + ".h";
	if (isFileExist(headerFullPath))
	{
		myVector<string> codeList;
		int lineStart = -1;
		if (!findCustomCode(headerFullPath, codeList, lineStart,
			[](const string& codeLine) { return codeLine == "// auto generate start"; },
			[](const string& codeLine) { return endWith(codeLine, "// auto generate end"); }))
		{
			return;
		}
		for (const string& line : generateCodes)
		{
			codeList.insert(++lineStart, line);
		}
		writeFile(headerFullPath, ANSIToUTF8(codeListToString(codeList).c_str(), true));
	}
	else
	{
		myVector<string> codeList;
		codeList.push_back("#pragma once");
		codeList.push_back("");
		codeList.push_back("#include \"Packet.h\"");
		if (packetInfo.mOwner == PACKET_OWNER::GAME_CORE)
		{
			codeList.push_back("#include \"GameCorePacketDefine.h\"");
		}
		else
		{
			codeList.push_back("#include \"GamePacketDefine.h\"");
		}
		codeList.push_back("");
		codeList.push_back("// auto generate start");
		codeList.addRange(generateCodes);
		codeList.push_back("\t// auto generate end");
		codeList.push_back("\tvoid debugInfo(Array<1024>& buffer) override");
		codeList.push_back("\t{");
		codeList.push_back("\t\tdebug(buffer, "");");
		codeList.push_back("\t}");
		codeList.push_back("};");
		writeFile(headerFullPath, ANSIToUTF8(codeListToString(codeList).c_str(), true));
	}
}

void CodeNetPacket::generateCppStruct(const PacketStruct& structInfo, const string& filePath)
{
	const string& structName = structInfo.mStructName;

	// 是否需要移动构造,当有列表,或者字符串等可移动的变量时,就需要有移动构造
	bool hasMoveConstruct = false;
	bool hasOptional = false;
	for (const PacketMember& member : structInfo.mMemberList)
	{
		if (!hasMoveConstruct && (member.mTypeName == "string" || startWith(member.mTypeName, "Vector<")))
		{
			hasMoveConstruct = true;
		}
		if (!hasOptional && member.mOptional)
		{
			hasOptional = true;
		}
	}

	// PacketStruct.h
	const string headerFullPath = filePath + structName + ".h";
	myVector<string> headerCodeList;
	headerCodeList.push_back("#pragma once");
	headerCodeList.push_back("");
	headerCodeList.push_back("#include \"SerializableBitData.h\"");
	headerCodeList.push_back("");
	headerCodeList.push_back(structInfo.mComment);
	if (structInfo.mOwner == PACKET_OWNER::GAME_CORE)
	{
		headerCodeList.push_back("class MICRO_LEGEND_CORE_API " + structName + " : public SerializableBitData");
	}
	else
	{
		headerCodeList.push_back("class " + structName + " : public SerializableBitData");
	}
	headerCodeList.push_back("{");
	headerCodeList.push_back("\tBASE(" + structName + ", SerializableBitData);");
	// 是否有可选字段
	if (hasOptional)
	{
		headerCodeList.push_back("public:");
		headerCodeList.push_back("\tenum class Field : byte");
		headerCodeList.push_back("\t{");
		FOR_I (structInfo.mMemberList.size())
		{
			const PacketMember& member = structInfo.mMemberList[i];
			if (member.mOptional)
			{
				if (i >= 64)
				{
					ERROR("可选字段的下标不能超过63");
					break;
				}
				headerCodeList.push_back("\t\t" + member.mMemberNameNoPrefix + " = " + intToString(i) + ",");
			}
		}
		headerCodeList.push_back("\t};");
	}
	headerCodeList.push_back("public:");
	generateCppPacketMemberDeclare(structInfo.mMemberList, headerCodeList);
	headerCodeList.push_back("public:");
	headerCodeList.push_back("\t" + structName + "() = default;");
	string constructParams;
	string constructMoveParams;
	const int memberCount = structInfo.mMemberList.size();
	// 当结构体成员数量不超过6时,提供带参构造和带可移动参的构造
	if (memberCount <= 6)
	{
		FOR_I(memberCount)
		{
			const PacketMember& member = structInfo.mMemberList[i];
			// 成员变量的命名格式都是以m开头,且后面的第一个字母是大写,所以需要去除m,将大写字母变为小写
			string tempParamName = member.mMemberName.substr(2);
			tempParamName.insert(0, 1, toLower(member.mMemberName[1]));
			if (member.mTypeName == "string" || 
				startWith(member.mTypeName, "Vector<") || 
				member.mTypeName == "Vector2" || 
				member.mTypeName == "Vector2UShort" || 
				member.mTypeName == "Vector2Int" || 
				member.mTypeName == "Vector3" || 
				member.mTypeName == "Vector4")
			{
				constructParams += "const " + member.mTypeName + "& " + tempParamName;
			}
			else
			{
				constructParams += member.mTypeName + " " + tempParamName;
			}
			if (i != memberCount - 1)
			{
				constructParams += ", ";
			}
		}
		headerCodeList.push_back("\t" + structName + "(" + constructParams + ");");
		if (hasMoveConstruct)
		{
			FOR_I(memberCount)
			{
				const PacketMember& member = structInfo.mMemberList[i];
				// 成员变量的命名格式都是以m开头,且后面的第一个字母是大写,所以需要去除m,将大写字母变为小写
				string tempParamName = member.mMemberName.substr(2);
				tempParamName.insert(0, 1, toLower(member.mMemberName[1]));
				if (member.mTypeName == "string" || startWith(member.mTypeName, "Vector<"))
				{
					constructMoveParams += "" + member.mTypeName + "&& " + tempParamName;
				}
				else
				{
					constructMoveParams += member.mTypeName + " " + tempParamName;
				}
				if (i != memberCount - 1)
				{
					constructMoveParams += ", ";
				}
			}
			headerCodeList.push_back("\t" + structName + "(" + constructMoveParams + ");");
		}		
	}
	if (hasMoveConstruct)
	{
		headerCodeList.push_back("\t" + structName + "(const " + structName + "& other);");
		headerCodeList.push_back("\t" + structName + "(" + structName + "&& other);");
		headerCodeList.push_back("\t" + structName + "& operator=(" + structName + "&& other);");
	}
	headerCodeList.push_back("\t" + structName + "& operator=(const " + structName + "& other);");
	headerCodeList.push_back("\tbool readFromBuffer(SerializerBitRead* reader) override;");
	headerCodeList.push_back("\tbool writeToBuffer(SerializerBitWrite* serializer) const override;");
	headerCodeList.push_back("\tvoid resetProperty() override;");
	if (hasOptional)
	{
		headerCodeList.push_back("\tstatic constexpr ullong fullOptionFlag()");
		headerCodeList.push_back("\t{");
		headerCodeList.push_back("\t\tullong fieldFlag = 0;");
		for (const PacketMember& item : structInfo.mMemberList)
		{
			if (item.mOptional)
			{
				headerCodeList.push_back("\t\tsetBitOne(fieldFlag, (byte)Field::" + item.mMemberNameNoPrefix + ");");
			}
		}
		headerCodeList.push_back("\t\treturn fieldFlag;");
		headerCodeList.push_back("\t}");
	}
	
	headerCodeList.push_back("};");
	writeFile(headerFullPath, ANSIToUTF8(codeListToString(headerCodeList).c_str(), true));
	
	// PacketStruct.cpp
	string sourceFullPath = filePath + structName + ".cpp";
	myVector<string> sourceCodeList;
	if (structInfo.mOwner == PACKET_OWNER::GAME)
	{
		sourceCodeList.push_back("#include \"GameHeader.h\"");
	}
	else
	{
		sourceCodeList.push_back("#include \"GameCoreHeader.h\"");
	}
	if (constructParams.length() > 0)
	{
		sourceCodeList.push_back("");
		sourceCodeList.push_back(structName + "::" + structName + "(" + constructParams + ") :");
		FOR_I(memberCount)
		{
			const PacketMember& member = structInfo.mMemberList[i];
			const string endComma = i != memberCount - 1 ? "," : "";
			// 成员变量的命名格式都是以m开头,且后面的第一个字母是大写,所以需要去除m,将大写字母变为小写
			string tempParamName = member.mMemberName.substr(2);
			tempParamName.insert(0, 1, toLower(member.mMemberName[1]));
			sourceCodeList.push_back("\t" + member.mMemberName + "(" + tempParamName + ")" + endComma);
		}
		sourceCodeList.push_back("{}");
	}
	if (constructMoveParams.length() > 0)
	{
		sourceCodeList.push_back("");
		sourceCodeList.push_back(structName + "::" + structName + "(" + constructMoveParams + ") :");
		FOR_I(memberCount)
		{
			const PacketMember& member = structInfo.mMemberList[i];
			const string endComma = i != memberCount - 1 ? "," : "";
			// 成员变量的命名格式都是以m开头,且后面的第一个字母是大写,所以需要去除m,将大写字母变为小写
			string tempParamName = member.mMemberName.substr(2);
			tempParamName.insert(0, 1, toLower(member.mMemberName[1]));
			if (member.mTypeName == "string" || startWith(member.mTypeName, "Vector<"))
			{
				sourceCodeList.push_back("\t" + member.mMemberName + "(move(" + tempParamName + "))" + endComma);
			}
			else
			{
				sourceCodeList.push_back("\t" + member.mMemberName + "(" + tempParamName + ")" + endComma);
			}
		}
		sourceCodeList.push_back("{}");
	}
	if (hasMoveConstruct)
	{
		sourceCodeList.push_back("");
		sourceCodeList.push_back(structName + "::" + structName + "(const " + structName + "& other) :");
		FOR_I(memberCount)
		{
			const PacketMember& member = structInfo.mMemberList[i];
			const string endComma = i != memberCount - 1 ? "," : "";
			sourceCodeList.push_back("\t" + member.mMemberName + "(other." + member.mMemberName + ")" + endComma);
		}
		sourceCodeList.push_back("{}");
		sourceCodeList.push_back("");
		sourceCodeList.push_back(structName + "::" + structName + "(" + structName + "&& other) :");
		FOR_I(memberCount)
		{
			const PacketMember& member = structInfo.mMemberList[i];
			const string endComma = i != memberCount - 1 ? "," : "";
			if (member.mTypeName == "string" || startWith(member.mTypeName, "Vector<"))
			{
				sourceCodeList.push_back("\t" + member.mMemberName + "(move(other." + member.mMemberName + "))" + endComma);
			}
			else
			{
				sourceCodeList.push_back("\t" + member.mMemberName + "(other." + member.mMemberName + ")" + endComma);
			}
		}
		sourceCodeList.push_back("{}");
		sourceCodeList.push_back("");
		sourceCodeList.push_back(structName + "& " + structName + "::operator=(" + structName + "&& other)");
		sourceCodeList.push_back("{");
		FOR_I(memberCount)
		{
			const PacketMember& member = structInfo.mMemberList[i];
			if (member.mTypeName == "string" || startWith(member.mTypeName, "Vector<"))
			{
				sourceCodeList.push_back("\t" + member.mMemberName + " = move(other." + member.mMemberName + ");");
			}
			else
			{
				sourceCodeList.push_back("\t" + member.mMemberName + " = other." + member.mMemberName + ";");
			}
		}
		sourceCodeList.push_back("\treturn *this;");
		sourceCodeList.push_back("}");
	}
	sourceCodeList.push_back("");
	sourceCodeList.push_back(structName + "& " + structName + "::operator=(const " + structName + "& other)");
	sourceCodeList.push_back("{");
	for (const PacketMember& member : structInfo.mMemberList)
	{
		sourceCodeList.push_back("\t" + member.mMemberName + " = other." + member.mMemberName + ";");
	}
	sourceCodeList.push_back("\treturn *this;");
	sourceCodeList.push_back("}");

	// readFromBuffer
	sourceCodeList.push_back("");
	sourceCodeList.push_back("bool " + structName + "::readFromBuffer(SerializerBitRead* reader)");
	sourceCodeList.push_back("{");
	if (hasOptional)
	{
		sourceCodeList.push_back("\t// 从缓冲区读取位标记");
		sourceCodeList.push_back("\tbool useFlag = false;");
		sourceCodeList.push_back("\treader->readBool(useFlag);");
		sourceCodeList.push_back("\tullong fieldFlag = FrameDefine::FULL_FIELD_FLAG;");
		sourceCodeList.push_back("\tif (useFlag)");
		sourceCodeList.push_back("\t{");
		sourceCodeList.push_back("\t\treader->readUnsigned(fieldFlag);");
		sourceCodeList.push_back("\t}");
		sourceCodeList.push_back("");
		sourceCodeList.push_back("\t// 再根据位标记读取字段数据");
	}
	sourceCodeList.push_back("\tbool success = true;");
	for (const PacketMember& item : structInfo.mMemberList)
	{
		// 可选字段需要特别判断一下
		string preTable = "\t";
		if (item.mOptional)
		{
			sourceCodeList.push_back("\tif (hasBit(fieldFlag, (byte)Field::" + item.mMemberNameNoPrefix + "))");
			sourceCodeList.push_back("\t{");
			preTable += "\t";
		}
		if (item.mTypeName == "string")
		{
			sourceCodeList.push_back(preTable + "success = success && reader->readString(" + item.mMemberName + ");");
		}
		else if (startWith(item.mTypeName, "Vector<"))
		{
			int lastPos;
			findSubstr(item.mTypeName, ">", &lastPos);
			const string elementType = item.mTypeName.substr(strlen("Vector<"), lastPos - strlen("Vector<"));
			if (elementType == "string")
			{
				sourceCodeList.push_back(preTable + "success = success && reader->readStringList(" + item.mMemberName + ");");
			}
			else if (elementType == "bool")
			{
				ERROR("不支持bool的列表");
			}
			else if (elementType == "char" || elementType == "short" || elementType == "int" || elementType == "llong")
			{
				sourceCodeList.push_back(preTable + "success = success && reader->readSignedList(" + item.mMemberName + ");");
			}
			else if (elementType == "byte" || elementType == "ushort" || elementType == "uint" || elementType == "ullong")
			{
				sourceCodeList.push_back(preTable + "success = success && reader->readUnsignedList(" + item.mMemberName + ");");
			}
			else if (elementType == "float")
			{
				sourceCodeList.push_back(preTable + "success = success && reader->readFloatList(" + item.mMemberName + ");");
			}
			else if (elementType == "double")
			{
				sourceCodeList.push_back(preTable + "success = success && reader->readFloatList(" + item.mMemberName + ");");
			}
			else
			{
				ERROR("结构体中不支持自定义结构体:" + elementType);
			}
		}
		else if (item.mTypeName == "bool")
		{
			sourceCodeList.push_back(preTable + "success = success && reader->readBool(" + item.mMemberName + ");");
		}
		else if (item.mTypeName == "char" || item.mTypeName == "short" || item.mTypeName == "int" || item.mTypeName == "llong")
		{
			sourceCodeList.push_back(preTable + "success = success && reader->readSigned(" + item.mMemberName + ");");
		}
		else if (item.mTypeName == "byte" || item.mTypeName == "ushort" || item.mTypeName == "uint" || item.mTypeName == "ullong")
		{
			sourceCodeList.push_back(preTable + "success = success && reader->readUnsigned(" + item.mMemberName + ");");
		}
		else if (item.mTypeName == "float")
		{
			sourceCodeList.push_back(preTable + "success = success && reader->readFloat(" + item.mMemberName + ");");
		}
		else if (item.mTypeName == "double")
		{
			sourceCodeList.push_back(preTable + "success = success && reader->readDouble(" + item.mMemberName + ");");
		}
		else if (item.mTypeName == "Vector2")
		{
			sourceCodeList.push_back(preTable + "success = success && reader->readVector2(" + item.mMemberName + ");");
		}
		else if (item.mTypeName == "Vector2UShort")
		{
			sourceCodeList.push_back(preTable + "success = success && reader->readVector2UShort(" + item.mMemberName + ");");
		}
		else if (item.mTypeName == "Vector2Int")
		{
			sourceCodeList.push_back(preTable + "success = success && reader->readVector2Int(" + item.mMemberName + ");");
		}
		else if (item.mTypeName == "Vector3")
		{
			sourceCodeList.push_back(preTable + "success = success && reader->readVector3(" + item.mMemberName + ");");
		}
		else if (item.mTypeName == "Vector4")
		{
			sourceCodeList.push_back(preTable + "success = success && reader->readVector4(" + item.mMemberName + ");");
		}
		else
		{
			ERROR("结构体中不支持自定义结构体:" + item.mTypeName);
		}
		if (item.mOptional)
		{
			sourceCodeList.push_back("\t}");
		}
	}
	sourceCodeList.push_back("\treturn success;");
	sourceCodeList.push_back("}");

	// writeToBuffer
	sourceCodeList.push_back("");
	sourceCodeList.push_back("bool " + structName + "::writeToBuffer(SerializerBitWrite* serializer) const");
	sourceCodeList.push_back("{");
	if (hasOptional)
	{
		sourceCodeList.push_back("\t// 将位标记写入到缓冲区");
		sourceCodeList.push_back("\t// 如果没有可选字段,则不使用位标记(在生成代码时会进行判断,所以不需要运行时再判断)");
		sourceCodeList.push_back("\t// 如果有可选字段,但是所有字段都需要同步,则也不使用位标记");
		sourceCodeList.push_back("\tbool useFlag = fullOptionFlag() != mFieldFlag;");
		sourceCodeList.push_back("\tserializer->writeBool(useFlag);");
		sourceCodeList.push_back("\tif (useFlag)");
		sourceCodeList.push_back("\t{");
		sourceCodeList.push_back("\t\tserializer->writeUnsigned(mFieldFlag);");
		sourceCodeList.push_back("\t}");
		sourceCodeList.push_back("\t");
		sourceCodeList.push_back("\t// 再根据位标记将字段数据写入缓冲区");
	}
	sourceCodeList.push_back("\tbool success = true;");
	for (const PacketMember& item : structInfo.mMemberList)
	{
		// 可选字段需要特别判断一下
		string preTable = "\t";
		if (item.mOptional)
		{
			sourceCodeList.push_back("\tif (isFieldValid(Field::" + item.mMemberNameNoPrefix + "))");
			sourceCodeList.push_back("\t{");
			preTable += "\t";
		}
		if (item.mTypeName == "string")
		{
			sourceCodeList.push_back(preTable + "success = success && serializer->writeString(" + item.mMemberName + ");");
		}
		else if (startWith(item.mTypeName, "Vector<"))
		{
			int lastPos;
			findSubstr(item.mTypeName, ">", &lastPos);
			const string elementType = item.mTypeName.substr(strlen("Vector<"), lastPos - strlen("Vector<"));
			if (elementType == "string")
			{
				sourceCodeList.push_back(preTable + "success = success && serializer->writeStringList(" + item.mMemberName + ");");
			}
			else if (elementType == "bool")
			{
				ERROR("不支持bool的列表");
			}
			else if (elementType == "char" || elementType == "short" || elementType == "int" || elementType == "llong")
			{
				sourceCodeList.push_back(preTable + "success = success && serializer->writeSignedList(" + item.mMemberName + ");");
			}
			else if (elementType == "byte" || elementType == "ushort" || elementType == "uint" || elementType == "ullong")
			{
				sourceCodeList.push_back(preTable + "success = success && serializer->writeUnsignedList(" + item.mMemberName + ");");
			}
			else if (elementType == "float")
			{
				sourceCodeList.push_back(preTable + "success = success && serializer->writeFloatList(" + item.mMemberName + ");");
			}
			else if (elementType == "double")
			{
				sourceCodeList.push_back(preTable + "success = success && serializer->writeDoubleList(" + item.mMemberName + ");");
			}
			else
			{
				ERROR("结构体中不支持自定义结构体:" + elementType);
			}
		}
		else if (item.mTypeName == "bool")
		{
			sourceCodeList.push_back(preTable + "success = success && serializer->writeBool(" + item.mMemberName + ");");
		}
		else if (item.mTypeName == "char" || item.mTypeName == "short" || item.mTypeName == "int" || item.mTypeName == "llong")
		{
			sourceCodeList.push_back(preTable + "success = success && serializer->writeSigned(" + item.mMemberName + ");");
		}
		else if (item.mTypeName == "byte" || item.mTypeName == "ushort" || item.mTypeName == "uint" || item.mTypeName == "ullong")
		{
			sourceCodeList.push_back(preTable + "success = success && serializer->writeUnsigned(" + item.mMemberName + ");");
		}
		else if (item.mTypeName == "bool")
		{
			sourceCodeList.push_back(preTable + "success = success && serializer->writeBool(" + item.mMemberName + ");");
		}
		else if (item.mTypeName == "float")
		{
			sourceCodeList.push_back(preTable + "success = success && serializer->writeFloat(" + item.mMemberName + ");");
		}
		else if (item.mTypeName == "double")
		{
			sourceCodeList.push_back(preTable + "success = success && serializer->writeDouble(" + item.mMemberName + ");");
		}
		else if (item.mTypeName == "Vector2")
		{
			sourceCodeList.push_back(preTable + "success = success && serializer->writeVector2(" + item.mMemberName + ");");
		}
		else if (item.mTypeName == "Vector2Int")
		{
			sourceCodeList.push_back(preTable + "success = success && serializer->writeVector2Int(" + item.mMemberName + ");");
		}
		else if (item.mTypeName == "Vector2UShort")
		{
			sourceCodeList.push_back(preTable + "success = success && serializer->writeVector2UShort(" + item.mMemberName + ");");
		}
		else if (item.mTypeName == "Vector3")
		{
			sourceCodeList.push_back(preTable + "success = success && serializer->writeVector3(" + item.mMemberName + ");");
		}
		else if (item.mTypeName == "Vector4")
		{
			sourceCodeList.push_back(preTable + "success = success && serializer->writeVector4(" + item.mMemberName + ");");
		}
		else
		{
			ERROR("结构体中不支持自定义结构体:" + item.mTypeName);
		}
		if (item.mOptional)
		{
			sourceCodeList.push_back("\t}");
		}
	}
	sourceCodeList.push_back("\treturn success;");
	sourceCodeList.push_back("}");
	
	// resetProperty
	sourceCodeList.push_back("");
	sourceCodeList.push_back("void " + structName + "::resetProperty()");
	sourceCodeList.push_back("{");
	sourceCodeList.push_back("\tbase::resetProperty();");
	for (const PacketMember& item : structInfo.mMemberList)
	{
		if (item.mTypeName == "Vector<bool>")
		{
			ERROR("不支持Vector<bool>类型,请使用Vector<byte>代替,packetType:" + structName);
		}
		if (item.mTypeName == "string" || 
			startWith(item.mTypeName, "Vector<") || 
			item.mTypeName == "Vector2" || 
			item.mTypeName == "Vector2UShort" || 
			item.mTypeName == "Vector2Int" || 
			item.mTypeName == "Vector3" || 
			item.mTypeName == "Vector4")
		{
			sourceCodeList.push_back("\t" + item.mMemberName + ".clear();");
		}
		else if (item.mTypeName == "bool")
		{
			sourceCodeList.push_back("\t" + item.mMemberName + " = false;");
		}
		else if (isPodInteger(item.mTypeName))
		{
			sourceCodeList.push_back("\t" + item.mMemberName + " = 0;");
		}
		else if (item.mTypeName == "float")
		{
			sourceCodeList.push_back("\t" + item.mMemberName + " = 0.0f;");
		}
		else if (item.mTypeName == "double")
		{
			sourceCodeList.push_back("\t" + item.mMemberName + " = 0.0;");
		}
		else
		{
			ERROR("结构体中不支持自定义结构体:" + item.mTypeName);
		}
	}
	sourceCodeList.push_back("}");

	writeFile(sourceFullPath, ANSIToUTF8(codeListToString(sourceCodeList).c_str(), true));
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

// CSPacket.cpp
void CodeNetPacket::generateCppCSPacketFileSource(const PacketInfo& packetInfo, const string& filePath)
{
	const string& packetName = packetInfo.mPacketName;
	if (!startWith(packetName, "CS"))
	{
		return;
	}

	const string sourceFullPath = filePath + packetName + ".cpp";
	if (!isFileExist(sourceFullPath))
	{
		string source;
		if (packetInfo.mOwner == PACKET_OWNER::GAME)
		{
			line(source, "#include \"GameHeader.h\"");
		}
		else if (packetInfo.mOwner == PACKET_OWNER::GAME_CORE)
		{
			line(source, "#include \"GameCoreHeader.h\"");
		}
		line(source, "");
		line(source, "void " + packetName + "::execute()");
		line(source, "{");
		line(source, "\tCharacterPlayer* player = getPlayer(mClient->getPlayerGUID());");
		line(source, "\tif (player == nullptr)");
		line(source, "\t{");
		line(source, "\t\treturn;");
		line(source, "\t}");
		line(source, "}", false);

		writeFile(sourceFullPath, ANSIToUTF8(source.c_str(), true));
	}
}

void CodeNetPacket::generateCppPacketMemberDeclare(const myVector<PacketMember>& memberList, myVector<string>& generateCodes)
{
	for (const PacketMember& item : memberList)
	{
		if (item.mTypeName == "byte" ||
			item.mTypeName == "char" ||
			item.mTypeName == "short" ||
			item.mTypeName == "ushort" ||
			item.mTypeName == "int" ||
			item.mTypeName == "uint" ||
			item.mTypeName == "llong" ||
			item.mTypeName == "ullong")
		{
			generateCodes.push_back("\t" + item.mTypeName + " " + item.mMemberName + " = 0;");
		}
		else if (item.mTypeName == "float" ||
				 item.mTypeName == "double")
		{
			generateCodes.push_back("\t" + item.mTypeName + " " + item.mMemberName + " = 0.0f;");
		}
		else if (item.mTypeName == "bool")
		{
			generateCodes.push_back("\t" + item.mTypeName + " " + item.mMemberName + " = false;");
		}
		else
		{
			generateCodes.push_back("\t" + item.mTypeName + " " + item.mMemberName + ";");
		}
	}
}

void CodeNetPacket::generateCppPacketReadWrite(const PacketInfo& packetInfo, myVector<string>& generateCodes)
{
	if (packetInfo.mMemberList.size() > 0)
	{
		// readFromBuffer
		generateCodes.push_back("\tbool readFromBuffer(SerializerBitRead* reader) override");
		generateCodes.push_back("\t{");
		generateCodes.push_back("\t\tbool success = true;");
		for (const PacketMember& item : packetInfo.mMemberList)
		{
			string preTable = "\t\t";
			if (item.mOptional)
			{
				generateCodes.push_back("\t\tif (isFieldValid(Field::" + item.mMemberNameNoPrefix + "))");
				generateCodes.push_back("\t\t{");
				preTable += "\t";
			}
			if (item.mTypeName == "string")
			{
				generateCodes.push_back(preTable + "success = success && reader->readString(" + item.mMemberName + ");");
			}
			else if (startWith(item.mTypeName, "Vector<"))
			{
				int lastPos;
				findSubstr(item.mTypeName, ">", &lastPos);
				const string elementType = item.mTypeName.substr(strlen("Vector<"), lastPos - strlen("Vector<"));
				if (elementType == "string")
				{
					generateCodes.push_back(preTable + "success = success && reader->readStringList(" + item.mMemberName + ");");
				}
				else if (elementType == "bool")
				{
					ERROR("不支持bool的列表");
				}
				else if (elementType == "char" || elementType == "short" || elementType == "int" || elementType == "llong")
				{
					generateCodes.push_back(preTable + "success = success && reader->readSignedList(" + item.mMemberName + ");");
				}
				else if (elementType == "byte" || elementType == "ushort" || elementType == "uint" || elementType == "ullong")
				{
					generateCodes.push_back(preTable + "success = success && reader->readUnsignedList(" + item.mMemberName + ");");
				}
				else if (elementType == "float")
				{
					generateCodes.push_back(preTable + "success = success && reader->readFloatList(" + item.mMemberName + ");");
				}
				else if (elementType == "double")
				{
					generateCodes.push_back(preTable + "success = success && reader->readDoubleList(" + item.mMemberName + ");");
				}
				else if (elementType == "Vector2")
				{
					generateCodes.push_back(preTable + "success = success && reader->readVector2(" + item.mMemberName + ");");
				}
				else if (elementType == "Vector2UShort")
				{
					generateCodes.push_back(preTable + "success = success && reader->readVector2UShort(" + item.mMemberName + ");");
				}
				else if (elementType == "Vector2Int")
				{
					generateCodes.push_back(preTable + "success = success && reader->readVector2Int(" + item.mMemberName + ");");
				}
				else if (elementType == "Vector3")
				{
					generateCodes.push_back(preTable + "success = success && reader->readVector3(" + item.mMemberName + ");");
				}
				else if (elementType == "Vector4")
				{
					generateCodes.push_back(preTable + "success = success && reader->readVector4(" + item.mMemberName + ");");
				}
				else
				{
					generateCodes.push_back(preTable + "success = success && reader->readCustomList(" + item.mMemberName + ");");
				}
			}
			else if (item.mTypeName == "bool")
			{
				generateCodes.push_back(preTable + "success = success && reader->readBool(" + item.mMemberName + ");");
			}
			else if (item.mTypeName == "char" || item.mTypeName == "short" || item.mTypeName == "int" || item.mTypeName == "llong")
			{
				generateCodes.push_back(preTable + "success = success && reader->readSigned(" + item.mMemberName + ");");
			}
			else if (item.mTypeName == "byte" || item.mTypeName == "ushort" || item.mTypeName == "uint" || item.mTypeName == "ullong")
			{
				generateCodes.push_back(preTable + "success = success && reader->readUnsigned(" + item.mMemberName + ");");
			}
			else if (item.mTypeName == "float")
			{
				generateCodes.push_back(preTable + "success = success && reader->readFloat(" + item.mMemberName + ");");
			}
			else if (item.mTypeName == "double")
			{
				generateCodes.push_back(preTable + "success = success && reader->readDouble(" + item.mMemberName + ");");
			}
			else if (item.mTypeName == "Vector2")
			{
				generateCodes.push_back(preTable + "success = success && reader->readVector2(" + item.mMemberName + ");");
			}
			else if (item.mTypeName == "Vector2UShort")
			{
				generateCodes.push_back(preTable + "success = success && reader->readVector2UShort(" + item.mMemberName + ");");
			}
			else if (item.mTypeName == "Vector2Int")
			{
				generateCodes.push_back(preTable + "success = success && reader->readVector2Int(" + item.mMemberName + ");");
			}
			else if (item.mTypeName == "Vector3")
			{
				generateCodes.push_back(preTable + "success = success && reader->readVector3(" + item.mMemberName + ");");
			}
			else if (item.mTypeName == "Vector4")
			{
				generateCodes.push_back(preTable + "success = success && reader->readVector4(" + item.mMemberName + ");");
			}
			else
			{
				generateCodes.push_back(preTable + "success = success && reader->readCustom(" + item.mMemberName + ");");
			}
			if (item.mOptional)
			{
				generateCodes.push_back("\t\t}");
			}
		}
		generateCodes.push_back("\t\treturn success;");
		generateCodes.push_back("\t}");

		// writeToBuffer
		generateCodes.push_back("\tbool writeToBuffer(SerializerBitWrite* serializer) const override");
		generateCodes.push_back("\t{");
		generateCodes.push_back("\t\tbool success = true;");
		for (const PacketMember& item : packetInfo.mMemberList)
		{
			string preTable = "\t\t";
			if (item.mOptional)
			{
				generateCodes.push_back("\t\tif (isFieldValid(Field::" + item.mMemberNameNoPrefix + "))");
				generateCodes.push_back("\t\t{");
				preTable += "\t";
			}
			if (item.mTypeName == "string")
			{
				generateCodes.push_back(preTable + "success = success && serializer->writeString(" + item.mMemberName + ");");
			}
			else if (startWith(item.mTypeName, "Vector<"))
			{
				int lastPos;
				findSubstr(item.mTypeName, ">", &lastPos);
				const string elementType = item.mTypeName.substr(strlen("Vector<"), lastPos - strlen("Vector<"));
				if (elementType == "string")
				{
					generateCodes.push_back(preTable + "success = success && serializer->writeStringList(" + item.mMemberName + ") success;");
				}
				else if (elementType == "bool")
				{
					ERROR("不支持bool的列表");
				}
				else if (elementType == "char" || elementType == "short" || elementType == "int" || elementType == "llong")
				{
					generateCodes.push_back(preTable + "success = success && serializer->writeSignedList(" + item.mMemberName + ");");
				}
				else if (elementType == "byte" || elementType == "ushort" || elementType == "uint" || elementType == "ullong")
				{
					generateCodes.push_back(preTable + "success = success && serializer->writeUnsignedList(" + item.mMemberName + ");");
				}
				else if (elementType == "float")
				{
					generateCodes.push_back(preTable + "success = success && serializer->writeFloatList(" + item.mMemberName + ");");
				}
				else if (elementType == "double")
				{
					generateCodes.push_back(preTable + "success = success && serializer->writeDoubleList(" + item.mMemberName + ");");
				}
				else if (elementType == "Vector2")
				{
					generateCodes.push_back(preTable + "success = success && serializer->writeVector2List(" + item.mMemberName + ");");
				}
				else if (elementType == "Vector2Int")
				{
					generateCodes.push_back(preTable + "success = success && serializer->writeVector2IntList(" + item.mMemberName + ");");
				}
				else if (elementType == "Vector2UShort")
				{
					generateCodes.push_back(preTable + "success = success && serializer->writeVector2UShortList(" + item.mMemberName + ");");
				}
				else if (elementType == "Vector3")
				{
					generateCodes.push_back(preTable + "success = success && serializer->writeVector3List(" + item.mMemberName + ");");
				}
				else if (elementType == "Vector4")
				{
					generateCodes.push_back(preTable + "success = success && serializer->writeVector4List(" + item.mMemberName + ");");
				}
				else
				{
					generateCodes.push_back(preTable + "success = success && serializer->writeCustomList(" + item.mMemberName + ");");
				}
			}
			else if (item.mTypeName == "bool")
			{
				generateCodes.push_back(preTable + "success = success && serializer->writeBool(" + item.mMemberName + ");");
			}
			else if (item.mTypeName == "char" || item.mTypeName == "short" || item.mTypeName == "int" || item.mTypeName == "llong")
			{
				generateCodes.push_back(preTable + "success = success && serializer->writeSigned(" + item.mMemberName + ");");
			}
			else if (item.mTypeName == "byte" || item.mTypeName == "ushort" || item.mTypeName == "uint" || item.mTypeName == "ullong")
			{
				generateCodes.push_back(preTable + "success = success && serializer->writeUnsigned(" + item.mMemberName + ");");
			}
			else if (item.mTypeName == "float")
			{
				generateCodes.push_back(preTable + "success = success && serializer->writeFloat(" + item.mMemberName + ");");
			}
			else if (item.mTypeName == "double")
			{
				generateCodes.push_back(preTable + "success = success && serializer->writeDouble(" + item.mMemberName + ");");
			}
			else if (item.mTypeName == "Vector2")
			{
				generateCodes.push_back(preTable + "success = success && serializer->writeVector2(" + item.mMemberName + ");");
			}
			else if (item.mTypeName == "Vector2Int")
			{
				generateCodes.push_back(preTable + "success = success && serializer->writeVector2Int(" + item.mMemberName + ");");
			}
			else if (item.mTypeName == "Vector2UShort")
			{
				generateCodes.push_back(preTable + "success = success && serializer->writeVector2UShort(" + item.mMemberName + ");");
			}
			else if (item.mTypeName == "Vector3")
			{
				generateCodes.push_back(preTable + "success = success && serializer->writeVector3(" + item.mMemberName + ");");
			}
			else if (item.mTypeName == "Vector4")
			{
				generateCodes.push_back(preTable + "success = success && serializer->writeVector4(" + item.mMemberName + ");");
			}
			else
			{
				generateCodes.push_back(preTable + "success = success && serializer->writeCustom(" + item.mMemberName + ");");
			}
			if (item.mOptional)
			{
				generateCodes.push_back("\t\t}");
			}
		}
		generateCodes.push_back("\t\treturn success;");
		generateCodes.push_back("\t}");

		// resetProperty
		generateCodes.push_back("\tvoid resetProperty() override");
		generateCodes.push_back("\t{");
		generateCodes.push_back("\t\tbase::resetProperty();");
		int startLineCount = generateCodes.size();
		for (const PacketMember& item : packetInfo.mMemberList)
		{
			if (item.mTypeName == "string" || 
				startWith(item.mTypeName, "Vector<") || 
				item.mTypeName == "Vector2" || 
				item.mTypeName == "Vector2UShort" || 
				item.mTypeName == "Vector2Int" || 
				item.mTypeName == "Vector3" || 
				item.mTypeName == "Vector4")
			{
				generateCodes.push_back("\t\t" + item.mMemberName + ".clear();");
			}
			else if (item.mTypeName == "bool")
			{
				generateCodes.push_back("\t\t" + item.mMemberName + " = false;");
			}
			else if (item.mTypeName == "float" || item.mTypeName == "double")
			{
				generateCodes.push_back("\t\t" + item.mMemberName + " = 0.0f;");
			}
			else if (isPodInteger(item.mTypeName))
			{
				generateCodes.push_back("\t\t" + item.mMemberName + " = 0;");
			}
			else
			{
				generateCodes.push_back("\t\t" + item.mMemberName + ".resetProperty();");
			}
		}
		generateCodes.push_back("\t}");
	}
	else
	{
		generateCodes.push_back("\tbool readFromBuffer(SerializerBitRead* reader) override { return true; }");
		generateCodes.push_back("\tbool writeToBuffer(SerializerBitWrite* serializer) const override { return true; }");
		generateCodes.push_back("\tvoid resetProperty() override");
		generateCodes.push_back("\t{");
		generateCodes.push_back("\t\tbase::resetProperty();");
		generateCodes.push_back("\t}");
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

	bool hasOptional = false;
	for (const auto& item : packetInfo.mMemberList)
	{
		if (item.mOptional)
		{
			hasOptional = true;
			break;
		}
	}

	myVector<string> generateCodes;
	generateCodes.push_back(packetInfo.mComment);
	if (packetInfo.mOwner == PACKET_OWNER::GAME_CORE)
	{
		generateCodes.push_back("class MICRO_LEGEND_CORE_API " + packetName + " : public Packet");
	}
	else
	{
		generateCodes.push_back("class " + packetName + " : public Packet");
	}
	generateCodes.push_back("{");
	generateCodes.push_back("\tBASE(" + packetName + ", Packet);");
	if (hasOptional)
	{
		generateCodes.push_back("public:");
		generateCodes.push_back("\tenum class Field : byte");
		generateCodes.push_back("\t{");
		FOR_I(packetInfo.mMemberList.size())
		{
			const auto& item = packetInfo.mMemberList[i];
			if (item.mOptional)
			{
				if (i >= 64)
				{
					ERROR("可选字段的下标不能超过63");
					break;
				}
				generateCodes.push_back("\t\t" + item.mMemberNameNoPrefix + " = " + intToString(i) + ",");
			}
		}
		generateCodes.push_back("\t};");
	}
	generateCodes.push_back("public:");
	generateCppPacketMemberDeclare(packetInfo.mMemberList, generateCodes);
	generateCodes.push_back("private:");
	generateCodes.push_back("\tstatic " + packetName + " mStaticObject;");
	generateCodes.push_back("public:");
	generateCodes.push_back("\t" + packetName + "()");
	generateCodes.push_back("\t{");
	if (packetInfo.mOwner == PACKET_OWNER::GAME_CORE)
	{
		generateCodes.push_back("\t\tmType = PACKET_TYPE_CORE::" + packetName + ";");
	}
	else
	{
		generateCodes.push_back("\t\tmType = PACKET_TYPE::" + packetName + ";");
	}
	generateCodes.push_back("\t\tmShowInfo = " + boolToString(packetInfo.mShowInfo) + ";");
	generateCodes.push_back("\t}");
	generateCodes.push_back("\tstatic " + packetName + "& get()");
	generateCodes.push_back("\t{");
	generateCodes.push_back("\t\tmStaticObject.resetProperty();");
	generateCodes.push_back("\t\treturn mStaticObject;");
	generateCodes.push_back("\t}");
	if (packetInfo.mOwner == PACKET_OWNER::GAME_CORE)
	{
		generateCodes.push_back("\tstatic constexpr ushort getStaticType() { return PACKET_TYPE_CORE::" + packetName + "; }");
	}
	else
	{
		generateCodes.push_back("\tstatic constexpr ushort getStaticType() { return PACKET_TYPE::" + packetName + "; }");
	}
	if (packetInfo.mMemberList.size() > 0)
	{
		generateCodes.push_back("\tstatic constexpr bool hasMember() { return true; }");
	}
	else
	{
		generateCodes.push_back("\tstatic constexpr bool hasMember() { return false; }");
	}
	generateCppPacketReadWrite(packetInfo, generateCodes);

	string headerFullPath = filePath + packetName + ".h";
	if (isFileExist(headerFullPath))
	{
		myVector<string> codeList;
		int lineStart = -1;
		if (!findCustomCode(headerFullPath, codeList, lineStart,
			[](const string& codeLine) { return codeLine == "// auto generate start"; },
			[](const string& codeLine) { return endWith(codeLine, "// auto generate end"); }))
		{
			return;
		}
		for (const string& line : generateCodes)
		{
			codeList.insert(++lineStart, line);
		}
		writeFile(headerFullPath, ANSIToUTF8(codeListToString(codeList).c_str(), true));
	}
	else
	{
		myVector<string> codeList;
		codeList.push_back("#pragma once");
		codeList.push_back("");
		codeList.push_back("#include \"Packet.h\"");
		if (packetInfo.mOwner == PACKET_OWNER::GAME_CORE)
		{
			codeList.push_back("#include \"GameCorePacketDefine.h\"");
		}
		else
		{
			codeList.push_back("#include \"GamePacketDefine.h\"");
		}
		codeList.push_back("");
		codeList.push_back("// auto generate start");
		codeList.addRange(generateCodes);
		codeList.push_back("\t// auto generate end");
		codeList.push_back("\tvoid debugInfo(Array<1024>& buffer) override");
		codeList.push_back("\t{");
		codeList.push_back("\t\tdebug(buffer, "");");
		codeList.push_back("\t}");
		codeList.push_back("};");
		writeFile(headerFullPath, ANSIToUTF8(codeListToString(codeList).c_str(), true));
	}
}

// SCPacket.cpp
void CodeNetPacket::generateCppSCPacketFileSource(const PacketInfo& packetInfo, const string& filePath)
{
	const string& packetName = packetInfo.mPacketName;
	if (!startWith(packetName, "SC"))
	{
		return;
	}
	string cppFullPath = filePath + packetName + ".cpp";
	if (isFileExist(cppFullPath))
	{
		myVector<string> codeList;
		int lineStart = -1;
		if (!findCustomCode(cppFullPath, codeList, lineStart,
			[](const string& codeLine) { return codeLine == "// auto generate start"; },
			[](const string& codeLine) { return endWith(codeLine, "// auto generate end"); }))
		{
			return;
		}
		codeList.insert(++lineStart, packetName + " " + packetName + "::mStaticObject;");
		writeFile(cppFullPath, ANSIToUTF8(codeListToString(codeList).c_str(), true));
	}
	else
	{
		myVector<string> codeList;
		if (packetInfo.mOwner == PACKET_OWNER::GAME_CORE)
		{
			codeList.push_back("#include \"GameCoreHeader.h\"");
		}
		else
		{
			codeList.push_back("#include \"GameHeader.h\"");
		}
		codeList.push_back("");
		codeList.push_back("// auto generate start");
		codeList.push_back(packetName + " " + packetName + "::mStaticObject;");
		codeList.push_back("// auto generate end");
		writeFile(cppFullPath, ANSIToUTF8(codeListToString(codeList).c_str(), true));
	}
}

// PacketDefine.cs文件
void CodeNetPacket::generateCSharpPacketDefineFile(const myVector<PacketInfo>& gamePacketList, const myVector<PacketInfo>& gameCorePacketList, const string& filePath)
{
	string str;
	line(str, "using System;");
	
	line(str, "");
	line(str, "public class PACKET_TYPE");
	line(str, "{");
	line(str, "\tpublic static ushort MIN = 0;");
	line(str, "");
	int csGameCorePacketNumber = 3000;
	for (const auto& item : gameCorePacketList)
	{
		if (startWith(item.mPacketName, "CS"))
		{
			line(str, "\tpublic static ushort " + packetNameToUpper(item.mPacketName) + " = " + intToString(++csGameCorePacketNumber) + ";");
		}
	}
	line(str, "");
	int scGameCorePacketNumber = 6000;
	for (const auto& item : gameCorePacketList)
	{
		if (startWith(item.mPacketName, "SC"))
		{
			line(str, "\tpublic static ushort " + packetNameToUpper(item.mPacketName) + " = " + intToString(++scGameCorePacketNumber) + ";");
		}
	}
	line(str, "");
	int csGamePacketNumber = 10000;
	for (const auto& item : gamePacketList)
	{
		if (startWith(item.mPacketName, "CS"))
		{
			line(str, "\tpublic static ushort " + packetNameToUpper(item.mPacketName) + " = " + intToString(++csGamePacketNumber) + ";");
		}
	}
	line(str, "");
	int scGamePacketNumber = 20000;
	for (const auto& item : gamePacketList)
	{
		if (startWith(item.mPacketName, "SC"))
		{
			line(str, "\tpublic static ushort " + packetNameToUpper(item.mPacketName) + " = " + intToString(++scGamePacketNumber) + ";");
		}
	}
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
	}
	if (udpSCList.size() > 0)
	{
		line(str, "");
		FOR_VECTOR(udpSCList)
		{
			line(str, "\t\tregisteUDP(PACKET_TYPE." + packetNameToUpper(udpSCList[i].mPacketName) + ", \"" + udpSCList[i].mPacketName + "\");");
		}
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

// CSPacket.cs文件
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

	myVector<string> generateCodes;
	generateCodes.push_back(packetInfo.mComment);
	generateCodes.push_back("public class " + packetName + " : NetPacketFrame");
	generateCodes.push_back("{");
	for (const PacketMember& item : packetInfo.mMemberList)
	{
		generateCodes.push_back("\t" + cSharpMemberDeclareString(item));
	}
	if (packetInfo.mMemberList.size() > 0)
	{
		// init
		generateCodes.push_back("\tpublic " + packetName + "()");
		generateCodes.push_back("\t{");
		for (const PacketMember& item : packetInfo.mMemberList)
		{
			generateCodes.push_back("\t\taddParam(" + item.mMemberName + ", " + (item.mOptional ? "true" : "false") + ");");
		}
		generateCodes.push_back("\t}");
	}
	
	if (isFileExist(fullPath))
	{
		myVector<string> codeList;
		int lineStart = -1;
		if (!findCustomCode(fullPath, codeList, lineStart,
			[](const string& codeLine) { return codeLine == "// auto generate start"; },
			[](const string& codeLine) { return endWith(codeLine, "// auto generate end"); }))
		{
			return;
		}
		for (const string& line : generateCodes)
		{
			codeList.insert(++lineStart, line);
		}
		writeFile(fullPath, ANSIToUTF8(codeListToString(codeList).c_str(), true));
	}
	else
	{
		myVector<string> codeList;
		codeList.push_back("using System;");
		codeList.push_back("");
		codeList.push_back("// auto generate start");
		codeList.addRange(generateCodes);
		codeList.push_back("\t// auto generate end");
		if (startWith(packetName, "SC"))
		{
			codeList.push_back("\tpublic override void execute()");
			codeList.push_back("\t{}");
		}
		codeList.push_back("}");
		writeFile(fullPath, ANSIToUTF8(codeListToString(codeList).c_str(), true));
	}
}

void CodeNetPacket::generateCSharpStruct(const PacketStruct& structInfo, const string& gamePath, const string& hotFixPath)
{
	myVector<string> codeList;
	codeList.push_back("using System;");
	codeList.push_back("using UnityEngine;");
	codeList.push_back("using System.Collections.Generic;");
	codeList.push_back("using static FrameUtility;");
	codeList.push_back("");
	codeList.push_back("public class " + structInfo.mStructName + " : NetStruct");
	codeList.push_back("{");
	for (const PacketMember& item : structInfo.mMemberList)
	{
		codeList.push_back("\t" + cSharpMemberDeclareString(item));
	}
	codeList.push_back("\tpublic " + structInfo.mStructName + "()");
	codeList.push_back("\t{");
	for (const PacketMember& item : structInfo.mMemberList)
	{
		codeList.push_back("\t\taddParam(" + item.mMemberName + ", " + (item.mOptional ? "true" : "false") + ");");
	}
	codeList.push_back("\t}");
	codeList.push_back("}");
	codeList.push_back("");
	codeList.push_back("public class " + structInfo.mStructName + "_List : SerializableBit");
	codeList.push_back("{");
	codeList.push_back("\tpublic List<" + structInfo.mStructName + "> mList = new List<" + structInfo.mStructName + ">();");
	codeList.push_back("\tpublic " + structInfo.mStructName + " this[int index]");
	codeList.push_back("\t{");
	codeList.push_back("\t\tget { return mList[index]; }");
	codeList.push_back("\t\tset { mList[index] = value; }");
	codeList.push_back("\t}");
	codeList.push_back("\tpublic int Count{ get { return mList.Count; } }");
	codeList.push_back("\tpublic override bool read(SerializerBitRead reader)");
	codeList.push_back("\t{");
	codeList.push_back("\t\treturn reader.readCustomList(mList, typeof(" + structInfo.mStructName + "));");
	codeList.push_back("\t}");
	codeList.push_back("\tpublic override void write(SerializerBitWrite writer)");
	codeList.push_back("\t{");
	codeList.push_back("\t\twriter.writeCustomList(mList);");
	codeList.push_back("\t}");
	codeList.push_back("\tpublic override void resetProperty()");
	codeList.push_back("\t{");
	codeList.push_back("\t\tbase.resetProperty();");
	codeList.push_back("\t\tUN_CLASS_LIST(mList);");
	codeList.push_back("\t}");
	codeList.push_back("}");

	const string fullPath = (structInfo.mHotFix ? hotFixPath : gamePath) + structInfo.mStructName + ".cs";
	writeFile(fullPath, ANSIToUTF8(codeListToString(codeList).c_str(), true));
}