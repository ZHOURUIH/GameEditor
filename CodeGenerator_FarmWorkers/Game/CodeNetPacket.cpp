#include "CodeNetPacket.h"

void CodeNetPacket::generate()
{
	print("��������������Ϣ");
	myVector<PacketStruct> structInfoList;
	myVector<PacketInfo> packetInfoList;
	parsePacketConfig(structInfoList, packetInfoList);

	int packetVersion = 0;
	generateCpp(structInfoList, packetInfoList, packetVersion);

	if (!csGamePath.empty())
	{
		generateCSharp(structInfoList, packetInfoList, packetVersion);
	}
	print("�������������Ϣ");
	print("");
}

void CodeNetPacket::generateVirtualClient()
{
	if (VirtualClientSocketPath.empty())
	{
		ERROR("δ��������ͻ�����Ŀ·��");
		return;
	}
	print("������������ͻ���������Ϣ");
	myVector<PacketStruct> structInfoList;
	myVector<PacketInfo> packetInfoList;
	parsePacketConfig(structInfoList, packetInfoList);

	int packetVersion = findPacketVersion(cppGamePath + "Socket/GamePacketRegister.cpp");
	generateCSharpVirtualClient(structInfoList, packetInfoList, packetVersion);
	print("�����������ͻ���������Ϣ");
	print("");
}

void CodeNetPacket::parsePacketConfig(myVector<PacketStruct>& structInfoList, myVector<PacketInfo>& packetInfoList)
{
	// ����ģ���ļ�
	myVector<string> csLines = openTxtFileLines("PacketCS.txt");
	myVector<string> scLines = openTxtFileLines("PacketSC.txt");
	myVector<string> structLines = openTxtFileLines("PacketStruct.txt");
	if (csLines.size() == 0)
	{
		ERROR("δ�ҵ�Э���ļ�PacketCS.txt");
		return;
	}
	if (scLines.size() == 0)
	{
		ERROR("δ�ҵ�Э���ļ�PacketSC.txt");
		return;
	}

	// �����ṹ�嶨��
	bool structStart = false;
	myVector<PacketMember> tempStructMemberList;
	int tempStructNameLine = 0;
	FOR_VECTOR(structLines)
	{
		string line = structLines[i];
		// ����ע��
		if (startWith(line, "//"))
		{
			continue;
		}
		// ����������ע��,��ȥ��
		int pos = -1;
		if (findString(line.c_str(), "//", &pos))
		{
			line = line.substr(0, pos);
		}
		// ȥ�������Ʊ���,�ֺ�
		removeAll(line, '\t', ';');
		// û�г�Ա��������Ϣ��
		if (line == "{}")
		{
			PacketStruct info;
			parseStructName(structLines[i - 1], info);
			info.mComment = structLines[i - 2];
			structInfoList.push_back(info);
			continue;
		}
		// ��Ա�����б���ʼ
		if (line == "{")
		{
			structStart = true;
			tempStructNameLine = i - 1;
			tempStructMemberList.clear();
			continue;
		}
		// ��Ա�����б�����
		if (line == "}")
		{
			if (!structStart)
			{
				ERROR("δ�ҵ�ǰһ��ƥ���{, PacketStruct,ǰ5������:");
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
			PacketMember member = parseMemberLine(line);
			member.mIndex = tempStructMemberList.size();
			tempStructMemberList.push_back(member);
			if (tempStructMemberList.size() >= 64 && tempStructMemberList[tempStructMemberList.size() - 1].mOptional)
			{
				ERROR("��֧��ǰ64���ֶ���������Ϊ��ѡ�ֶ�,����:" + structLines[tempStructNameLine]);
			}
		}
	}

	// ������Ϣ����
	myVector<string> allLines;
	allLines.addRange(csLines);
	allLines.addRange(scLines);
	bool packetStart = false;
	myVector<PacketMember> tempMemberList;
	int tempPacketNameLine = 0;
	FOR_VECTOR(allLines)
	{
		string line = allLines[i];
		// ����ע��
		if (startWith(line, "//"))
		{
			continue;
		}
		// ����������ע��,��ȥ��
		int pos = -1;
		if (findString(line.c_str(), "//", &pos))
		{
			line = line.substr(0, pos);
		}
		// ȥ�������Ʊ���,�ֺ�
		removeAll(line, '\t', ';');
		// û�г�Ա��������Ϣ��
		if (line == "{}")
		{
			PacketInfo info;
			parsePacketName(allLines[i - 1], info);
			info.mComment = allLines[i - 2];
			packetInfoList.push_back(info);
			continue;
		}
		// ��Ա�����б���ʼ
		if (line == "{")
		{
			packetStart = true;
			tempPacketNameLine = i - 1;
			tempMemberList.clear();
			continue;
		}
		// ��Ա�����б�����
		if (line == "}")
		{
			if (!packetStart)
			{
				ERROR("δ�ҵ�ǰһ��ƥ���{, NetPacket,ǰ5������:");
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
			PacketMember member = parseMemberLine(line);
			member.mIndex = tempMemberList.size();
			tempMemberList.push_back(member);
			if (tempMemberList.size() >= 64 && tempMemberList[tempMemberList.size() - 1].mOptional)
			{
				ERROR("��֧��ǰ64���ֶ���������Ϊ��ѡ�ֶ�,����:" + allLines[tempPacketNameLine]);
			}
		}
	}
}

void CodeNetPacket::generateCpp(const myVector<PacketStruct>& structInfoList, const myVector<PacketInfo>& packetInfoList, int& packetVersion)
{
	myVector<PacketInfo> gamePacketList;
	myVector<string> gamePacketNameList;
	for (const PacketInfo& packetInfo : packetInfoList)
	{
		gamePacketNameList.push_back(packetInfo.mPacketName);
		gamePacketList.push_back(packetInfo);
	}
	// Game�����Ϣ
	string cppGameCSPacketPath = cppGamePath + "Socket/ClientServer/";
	string cppGameSCPacketPath = cppGamePath + "Socket/ServerClient/";
	string cppGameStructPath = cppGamePath + "Socket/Struct/";
	string cppGamePacketDefinePath = cppGamePath + "Socket/";
	// ɾ�����õ���Ϣ
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

	// �򿪷���������Ϣע���ļ�,�ҵ���ǰ����Ϣ�汾��,Ȼ��汾������
	packetVersion = findPacketVersion(cppGamePacketDefinePath + "GamePacketRegister.cpp") + 1;
	// ����c++����
	for (const PacketInfo& packetInfo : gamePacketList)
	{
		// �ҵ���û�д��ļ�,�о���ԭ�����ļ����޸�
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

	for (const PacketStruct& info : structInfoList)
	{
		generateCppStruct(info, cppGameStructPath);
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

	myVector<string> gamePacketNameList;
	for (const PacketInfo& packetInfo : packetInfoList)
	{
		gamePacketNameList.push_back(packetInfo.mPacketName);
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
	// ɾ�����õ���Ϣ
	// c# CS���ȸ�
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
	// c# CS�ȸ�
	myVector<string> csharpCSHotfixFiles;
	findFiles(csharpCSHotfixPath, csharpCSHotfixFiles, ".cs");
	for (const string& file : csharpCSHotfixFiles)
	{
		if (!hotfixList.contains(getFileNameNoSuffix(file, true)))
		{
			deleteFile(file);
		}
	}
	// c# SC���ȸ�
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
	// c# SC�ȸ�
	myVector<string> csharpSCHotfixFiles;
	findFiles(csharpSCHotfixPath, csharpSCHotfixFiles, ".cs");
	for (const string& file : csharpSCHotfixFiles)
	{
		if (!hotfixList.contains(getFileNameNoSuffix(file, true)))
		{
			deleteFile(file);
		}
	}
	// ɾ�����õĽṹ�����
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

	// ����cs����
	for (const PacketInfo& packetInfo : packetInfoList)
	{
		generateCSharpPacketFile(packetInfo, csharpCSHotfixPath, csharpCSGamePath, csharpSCHotfixPath, csharpSCGamePath);
	}
	generateCSharpPacketDefineFile(packetInfoList, csharpPacketDefinePath);
	generateCSharpPacketRegisteFile(packetInfoList, csharpPacketDefinePath, packetVersion);

	// ���ɽṹ�����
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

	myVector<string> packetNameList;
	for (const PacketInfo& packetInfo : packetInfoList)
	{
		packetNameList.push_back(packetInfo.mPacketName);
	}
	myVector<string> structNameList;
	for (const PacketStruct& structInfo : structInfoList)
	{
		structNameList.push_back(structInfo.mStructName);
	}

	// ɾ�����õ���Ϣ
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
	// ɾ�����õĽṹ�����
	myVector<string> csharpStructFiles;
	findFiles(csharpStructGamePath, csharpStructFiles, ".cs");
	for (const string& file : csharpStructFiles)
	{
		if (!structNameList.contains(getFileNameNoSuffix(file, true)))
		{
			deleteFile(file);
		}
	}

	// ����cs����
	for (const PacketInfo& packetInfo : packetInfoList)
	{
		generateCSharpPacketFile(packetInfo, csharpCSGamePath, csharpCSGamePath, csharpSCGamePath, csharpSCGamePath);
	}
	generateCSharpPacketDefineFile(packetInfoList, csharpPacketDefinePath);
	generateCSharpPacketRegisteFile(packetInfoList, csharpPacketDefinePath, packetVersion);

	// ���ɽṹ�����
	for (const PacketStruct& item : structInfoList)
	{
		generateCSharpStruct(item, csharpStructGamePath, csharpStructGamePath);
	}
}

// PacketDefine.h�ļ�
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
		replaceFileLines(fullPath, 
			[](const string& codeLine) { return endWith(codeLine, "// auto generate start"); },
			[](const string& codeLine) { return endWith(codeLine, "// auto generate end"); }, generateList);
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

// PacketRegister.cpp�ļ�
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
	generateCodes.push_back("class " + packetName + " : public Packet");
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
					ERROR("��ѡ�ֶε��±겻�ܳ���63");
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
	generateCodes.push_back("\t\tmType = PACKET_TYPE::" + packetName + ";");
	generateCodes.push_back("\t\tmShowInfo = " + boolToString(packetInfo.mShowInfo) + ";");
	generateCodes.push_back("\t}");
	generateCppPacketReadWrite(packetInfo, generateCodes);
	generateCodes.push_back("\tvoid execute() override;");

	// CSPacket.h
	string headerFullPath = filePath + packetName + ".h";
	if (isFileExist(headerFullPath))
	{
		replaceFileLines(headerFullPath, 
			[](const string& codeLine) { return codeLine == "// auto generate start"; },
			[](const string& codeLine) { return endWith(codeLine, "// auto generate end"); }, generateCodes);
	}
	else
	{
		myVector<string> codeList;
		codeList.push_back("#pragma once");
		codeList.push_back("");
		codeList.push_back("#include \"Packet.h\"");
		codeList.push_back("#include \"GamePacketDefine.h\"");
		codeList.push_back("");
		codeList.push_back("// auto generate start");
		codeList.addRange(generateCodes);
		codeList.push_back("\t// auto generate end");
		codeList.push_back("\tvoid debugInfo(MyString<1024>& buffer) override");
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

	// �Ƿ���Ҫ�ƶ�����,�����б�,�����ַ����ȿ��ƶ��ı���ʱ,����Ҫ���ƶ�����
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
	headerCodeList.push_back("class " + structName + " : public SerializableBitData");
	headerCodeList.push_back("{");
	headerCodeList.push_back("\tBASE(" + structName + ", SerializableBitData);");
	// �Ƿ��п�ѡ�ֶ�
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
					ERROR("��ѡ�ֶε��±겻�ܳ���63");
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
	// ���ṹ���Ա����������6ʱ,�ṩ���ι���ʹ����ƶ��εĹ���
	if (memberCount <= 6)
	{
		FOR_I(memberCount)
		{
			const PacketMember& member = structInfo.mMemberList[i];
			// ��Ա������������ʽ������m��ͷ,�Һ���ĵ�һ����ĸ�Ǵ�д,������Ҫȥ��m,����д��ĸ��ΪСд
			string tempParamName = member.mMemberName.substr(2);
			tempParamName.insert(0, 1, toLower(member.mMemberName[1]));
			if (member.mTypeName == "string" || 
				startWith(member.mTypeName, "Vector<") || 
				member.mTypeName == "Vector2" || 
				member.mTypeName == "Vector2UShort" || 
				member.mTypeName == "Vector2UInt" || 
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
				// ��Ա������������ʽ������m��ͷ,�Һ���ĵ�һ����ĸ�Ǵ�д,������Ҫȥ��m,����д��ĸ��ΪСд
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
	
	myVector<myVector<PacketMember>> memberGroupList;
	generateMemberGroup(structInfo.mMemberList, memberGroupList);

	// PacketStruct.cpp
	string sourceFullPath = filePath + structName + ".cpp";
	myVector<string> sourceCodeList;
	sourceCodeList.push_back("#include \"GameHeader.h\"");
	if (constructParams.length() > 0)
	{
		sourceCodeList.push_back("");
		sourceCodeList.push_back(structName + "::" + structName + "(" + constructParams + ") :");
		FOR_I(memberCount)
		{
			const PacketMember& member = structInfo.mMemberList[i];
			const string endComma = i != memberCount - 1 ? "," : "";
			// ��Ա������������ʽ������m��ͷ,�Һ���ĵ�һ����ĸ�Ǵ�д,������Ҫȥ��m,����д��ĸ��ΪСд
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
			// ��Ա������������ʽ������m��ͷ,�Һ���ĵ�һ����ĸ�Ǵ�д,������Ҫȥ��m,����д��ĸ��ΪСд
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
		sourceCodeList.push_back("\t// �ӻ�������ȡλ���");
		sourceCodeList.push_back("\tbool useFlag = false;");
		sourceCodeList.push_back("\treader->readBool(useFlag);");
		sourceCodeList.push_back("\tullong fieldFlag = FrameDefine::FULL_FIELD_FLAG;");
		sourceCodeList.push_back("\tif (useFlag)");
		sourceCodeList.push_back("\t{");
		sourceCodeList.push_back("\t\treader->readUnsigned(fieldFlag);");
		sourceCodeList.push_back("\t}");
		sourceCodeList.push_back("");
		sourceCodeList.push_back("\t// �ٸ���λ��Ƕ�ȡ�ֶ�����");
	}
	sourceCodeList.push_back("\tbool success = true;");
	FOR_VECTOR(memberGroupList)
	{
		const myVector<PacketMember>& memberGroup = memberGroupList[i];
		if (memberGroup.size() == 1)
		{
			const PacketMember& item = memberGroup[0];
			// ��ѡ�ֶ���Ҫ�ر��ж�һ��
			if (item.mOptional)
			{
				sourceCodeList.push_back("\tif (hasBit(fieldFlag, (byte)Field::" + item.mMemberNameNoPrefix + "))");
				sourceCodeList.push_back("\t{");
				sourceCodeList.push_back("\t\t" + singleMemberReadLine(item.mMemberName, item.mTypeName, false));
				sourceCodeList.push_back("\t}");
			}
			else
			{
				sourceCodeList.push_back("\t" + singleMemberReadLine(item.mMemberName, item.mTypeName, false));
			}
		}
		else
		{
			myVector<string> nameList;
			string groupTypeName = expandMembersInGroup(memberGroup, nameList);
			myVector<string> list = multiMemberReadLine(nameList, groupTypeName, false);
			FOR_VECTOR(list)
			{
				sourceCodeList.push_back("\t" + list[i]);
			}
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
		sourceCodeList.push_back("\t// ��λ���д�뵽������");
		sourceCodeList.push_back("\t// ���û�п�ѡ�ֶ�,��ʹ��λ���(�����ɴ���ʱ������ж�,���Բ���Ҫ����ʱ���ж�)");
		sourceCodeList.push_back("\t// ����п�ѡ�ֶ�,���������ֶζ���Ҫͬ��,��Ҳ��ʹ��λ���");
		sourceCodeList.push_back("\tbool useFlag = fullOptionFlag() != mFieldFlag;");
		sourceCodeList.push_back("\tserializer->writeBool(useFlag);");
		sourceCodeList.push_back("\tif (useFlag)");
		sourceCodeList.push_back("\t{");
		sourceCodeList.push_back("\t\tserializer->writeUnsigned(mFieldFlag);");
		sourceCodeList.push_back("\t}");
		sourceCodeList.push_back("\t");
		sourceCodeList.push_back("\t// �ٸ���λ��ǽ��ֶ�����д�뻺����");
	}
	sourceCodeList.push_back("\tbool success = true;");
	FOR_VECTOR(memberGroupList)
	{
		const myVector<PacketMember>& memberGroup = memberGroupList[i];
		if (memberGroup.size() == 1)
		{
			const PacketMember& item = memberGroup[0];
			// ��ѡ�ֶ���Ҫ�ر��ж�һ��
			if (item.mOptional)
			{
				sourceCodeList.push_back("\tif (isFieldValid(Field::" + item.mMemberNameNoPrefix + "))");
				sourceCodeList.push_back("\t{");
				sourceCodeList.push_back("\t\t" + singleMemberWriteLine(item.mMemberName, item.mTypeName, false));
				sourceCodeList.push_back("\t}");
			}
			else
			{
				sourceCodeList.push_back("\t" + singleMemberWriteLine(item.mMemberName, item.mTypeName, false));
			}
		}
		else
		{
			myVector<string> nameList;
			string groupTypeName = expandMembersInGroup(memberGroup, nameList);
			myVector<string> list = multiMemberWriteLine(nameList, groupTypeName, false);
			FOR_VECTOR(list)
			{
				sourceCodeList.push_back("\t" + list[i]);
			}
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
			ERROR("��֧��Vector<bool>����,��ʹ��Vector<byte>����,packetType:" + structName);
		}
		if (item.mTypeName == "string" || 
			startWith(item.mTypeName, "Vector<") || 
			item.mTypeName == "Vector2" || 
			item.mTypeName == "Vector2UShort" || 
			item.mTypeName == "Vector2UInt" || 
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
			ERROR("�ṹ���в�֧���Զ���ṹ��:" + item.mTypeName);
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
		line(source, "#include \"GameHeader.h\"");
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

string CodeNetPacket::singleMemberReadLine(const string& memberName, const string& memberType, bool supportCustom)
{
	if (memberType == "string")
	{
		return "success = success && reader->readString(" + memberName + ");";
	}
	else if (startWith(memberType, "Vector<"))
	{
		int lastPos;
		findSubstr(memberType, ">", &lastPos);
		const string elementType = memberType.substr(strlen("Vector<"), lastPos - strlen("Vector<"));
		if (elementType == "string")
		{
			return "success = success && reader->readStringList(" + memberName + ");";
		}
		else if (elementType == "bool")
		{
			ERROR("��֧��bool���б�");
			return "";
		}
		else if (elementType == "char" || elementType == "short" || elementType == "int" || elementType == "llong")
		{
			return "success = success && reader->readSignedList(" + memberName + ");";
		}
		else if (elementType == "byte" || elementType == "ushort" || elementType == "uint" || elementType == "ullong")
		{
			return "success = success && reader->readUnsignedList(" + memberName + ");";
		}
		else if (elementType == "float")
		{
			return "success = success && reader->readFloatList(" + memberName + ");";
		}
		else if (elementType == "double")
		{
			return "success = success && reader->readDoubleList(" + memberName + ");";
		}
		else if (elementType == "Vector2")
		{
			return "success = success && reader->readVector2(" + memberName + ");";
		}
		else if (elementType == "Vector2UShort")
		{
			return "success = success && reader->readVector2UShort(" + memberName + ");";
		}
		else if (elementType == "Vector2UInt")
		{
			return "success = success && reader->readVector2UInt(" + memberName + ");";
		}
		else if (elementType == "Vector2Int")
		{
			return "success = success && reader->readVector2Int(" + memberName + ");";
		}
		else if (elementType == "Vector3")
		{
			return "success = success && reader->readVector3(" + memberName + ");";
		}
		else if (elementType == "Vector4")
		{
			return "success = success && reader->readVector4(" + memberName + ");";
		}
		else
		{
			if (supportCustom)
			{
				return "success = success && reader->readCustomList(" + memberName + ");";
			}
			else
			{
				ERROR("��֧���Զ���ṹ��:" + memberType);
			}
		}
	}
	else if (memberType == "bool")
	{
		return "success = success && reader->readBool(" + memberName + ");";
	}
	else if (memberType == "char" || memberType == "short" || memberType == "int" || memberType == "llong")
	{
		return "success = success && reader->readSigned(" + memberName + ");";
	}
	else if (memberType == "byte" || memberType == "ushort" || memberType == "uint" || memberType == "ullong")
	{
		return "success = success && reader->readUnsigned(" + memberName + ");";
	}
	else if (memberType == "float")
	{
		return "success = success && reader->readFloat(" + memberName + ");";
	}
	else if (memberType == "double")
	{
		return "success = success && reader->readDouble(" + memberName + ");";
	}
	else if (memberType == "Vector2")
	{
		return "success = success && reader->readVector2(" + memberName + ");";
	}
	else if (memberType == "Vector2UShort")
	{
		return "success = success && reader->readVector2UShort(" + memberName + ");";
	}
	else if (memberType == "Vector2UInt")
	{
		return "success = success && reader->readVector2UInt(" + memberName + ");";
	}
	else if (memberType == "Vector2Int")
	{
		return "success = success && reader->readVector2Int(" + memberName + ");";
	}
	else if (memberType == "Vector3")
	{
		return "success = success && reader->readVector3(" + memberName + ");";
	}
	else if (memberType == "Vector4")
	{
		return "success = success && reader->readVector4(" + memberName + ");";
	}
	if (supportCustom)
	{
		return "success = success && reader->readCustom(" + memberName + ");";
	}
	else
	{
		ERROR("��֧���Զ���ṹ��:" + memberType);
		return "";
	}
}

myVector<string> CodeNetPacket::multiMemberReadLine(const myVector<string>& memberNameList, const string& memberType, bool supportCustom)
{
	string members;
	FOR_VECTOR(memberNameList)
	{
		members += memberNameList[i];
		if (i < memberNameList.size() - 1)
		{
			members += ", ";
		}
	}

	myVector<string> list;
	if (memberType == "string")
	{
		FOR_VECTOR(memberNameList)
		{
			list.push_back("success = success && reader->readString(" + memberNameList[i] + ");");
		}
	}
	else if (startWith(memberType, "Vector<"))
	{
		int lastPos;
		findSubstr(memberType, ">", &lastPos);
		const string elementType = memberType.substr(strlen("Vector<"), lastPos - strlen("Vector<"));
		FOR_VECTOR(memberNameList)
		{
			if (elementType == "string")
			{
				list.push_back("success = success && reader->readStringList(" + memberNameList[i] + ");");
			}
			else if (elementType == "bool")
			{
				ERROR("��֧��bool���б�");
			}
			else if (elementType == "char" || elementType == "short" || elementType == "int" || elementType == "llong")
			{
				list.push_back("success = success && reader->readSignedList(" + memberNameList[i] + ");");
			}
			else if (elementType == "byte" || elementType == "ushort" || elementType == "uint" || elementType == "ullong")
			{
				list.push_back("success = success && reader->readUnsignedList(" + memberNameList[i] + ");");
			}
			else if (elementType == "float")
			{
				list.push_back("success = success && reader->readFloatList(" + memberNameList[i] + ");");
			}
			else if (elementType == "double")
			{
				list.push_back("success = success && reader->readDoubleList(" + memberNameList[i] + ");");
			}
			else if (elementType == "Vector2")
			{
				list.push_back("success = success && reader->readVector2List(" + memberNameList[i] + ");");
			}
			else if (elementType == "Vector2UShort")
			{
				list.push_back("success = success && reader->readVector2UShortList(" + memberNameList[i] + ");");
			}
			else if (elementType == "Vector2UInt")
			{
				list.push_back("success = success && reader->readVector2UIntList(" + memberNameList[i] + ");");
			}
			else if (elementType == "Vector2Int")
			{
				list.push_back("success = success && reader->readVector2IntList(" + memberNameList[i] + ");");
			}
			else if (elementType == "Vector3")
			{
				list.push_back("success = success && reader->readVector3List(" + memberNameList[i] + ");");
			}
			else if (elementType == "Vector4")
			{
				list.push_back("success = success && reader->readVector4List(" + memberNameList[i] + ");");
			}
			else
			{
				if (supportCustom)
				{
					list.push_back("success = success && reader->readCustomList(" + memberNameList[i] + ");");
				}
				else
				{
					ERROR("��֧���Զ���ṹ��:" + memberType);
				}
			}
		}
	}
	else if (memberType == "bool")
	{
		FOR_VECTOR(memberNameList)
		{
			list.push_back("success = success && reader->readBool(" + memberNameList[i] + ");");
		}
	}
	else if (memberType == "char" || memberType == "short" || memberType == "int" || memberType == "llong")
	{
		list.push_back("success = success && reader->readSigned(" + members + ");");
	}
	else if (memberType == "byte" || memberType == "ushort" || memberType == "uint" || memberType == "ullong")
	{
		list.push_back("success = success && reader->readUnsigned(" + members + ");");
	}
	else if (memberType == "float")
	{
		list.push_back("success = success && reader->readFloat(" + members + ");");
	}
	else if (memberType == "double")
	{
		list.push_back("success = success && reader->readDouble(" + members + ");");
	}
	else
	{
		if (supportCustom)
		{
			FOR_VECTOR(memberNameList)
			{
				list.push_back("success = success && reader->readCustom(" + memberNameList[i] + ");");
			}
		}
		else
		{
			ERROR("��֧���Զ���ṹ��:" + memberType);
		}
	}
	return list;
}

myVector<string> CodeNetPacket::multiMemberWriteLine(const myVector<string>& memberNameList, const string& memberType, bool supportCustom)
{
	string members;
	FOR_VECTOR(memberNameList)
	{
		members += memberNameList[i];
		if (i < memberNameList.size() - 1)
		{
			members += ", ";
		}
	}

	myVector<string> list;
	if (memberType == "string")
	{
		FOR_VECTOR(memberNameList)
		{
			list.push_back("success = success && serializer->writeString(" + memberNameList[i] + ");");
		}
	}
	else if (startWith(memberType, "Vector<"))
	{
		int lastPos;
		findSubstr(memberType, ">", &lastPos);
		const string elementType = memberType.substr(strlen("Vector<"), lastPos - strlen("Vector<"));
		FOR_VECTOR(memberNameList)
		{
			if (elementType == "string")
			{
				list.push_back("success = success && serializer->writeStringList(" + memberNameList[i] + ");");
			}
			else if (elementType == "bool")
			{
				ERROR("��֧��bool���б�");
			}
			else if (elementType == "char" || elementType == "short" || elementType == "int" || elementType == "llong")
			{
				list.push_back("success = success && serializer->writeSignedList(" + memberNameList[i] + ");");
			}
			else if (elementType == "byte" || elementType == "ushort" || elementType == "uint" || elementType == "ullong")
			{
				list.push_back("success = success && serializer->writeUnsignedList(" + memberNameList[i] + ");");
			}
			else if (elementType == "float")
			{
				list.push_back("success = success && serializer->writeFloatList(" + memberNameList[i] + ");");
			}
			else if (elementType == "double")
			{
				list.push_back("success = success && serializer->writeDoubleList(" + memberNameList[i] + ");");
			}
			else if (elementType == "Vector2")
			{
				list.push_back("success = success && serializer->writeVector2List(" + memberNameList[i] + ");");
			}
			else if (elementType == "Vector2UShort")
			{
				list.push_back("success = success && serializer->writeVector2UShortList(" + memberNameList[i] + ");");
			}
			else if (elementType == "Vector2UInt")
			{
				list.push_back("success = success && serializer->writeVector2UIntList(" + memberNameList[i] + ");");
			}
			else if (elementType == "Vector2Int")
			{
				list.push_back("success = success && serializer->writeVector2IntList(" + memberNameList[i] + ");");
			}
			else if (elementType == "Vector3")
			{
				list.push_back("success = success && serializer->writeVector3List(" + memberNameList[i] + ");");
			}
			else if (elementType == "Vector4")
			{
				list.push_back("success = success && serializer->writeVector4List(" + memberNameList[i] + ");");
			}
			else
			{
				if (supportCustom)
				{
					list.push_back("success = success && serializer->readCustomList(" + memberNameList[i] + ");");
				}
				else
				{
					ERROR("��֧���Զ���ṹ��:" + memberType);
				}
			}
		}
	}
	else if (memberType == "bool")
	{
		FOR_VECTOR(memberNameList)
		{
			list.push_back("success = success && serializer->writeBool(" + memberNameList[i] + ");");
		}
	}
	else if (memberType == "char" || memberType == "short" || memberType == "int" || memberType == "llong")
	{
		list.push_back("success = success && serializer->writeSigned(" + members + ");");
	}
	else if (memberType == "byte" || memberType == "ushort" || memberType == "uint" || memberType == "ullong")
	{
		list.push_back("success = success && serializer->writeUnsigned(" + members + ");");
	}
	else if (memberType == "float")
	{
		list.push_back("success = success && serializer->writeFloat(" + members + ");");
	}
	else if (memberType == "double")
	{
		list.push_back("success = success && serializer->writeDouble(" + members + ");");
	}
	else
	{
		if (supportCustom)
		{
			FOR_VECTOR(memberNameList)
			{
				list.push_back("success = success && serializer->writeCustom(" + memberNameList[i] + ");");
			}
		}
		else
		{
			ERROR("��֧���Զ���ṹ��:" + memberType);
		}
	}
	return list;
}

string CodeNetPacket::singleMemberReadLineCSharp(const string& memberName, const string& memberType)
{
	if (memberType == "string")
	{
		return "success = success && reader.readString(out " + memberName + ".mValue);";
	}
	else if (startWith(memberType, "Vector<"))
	{
		return "success = success && reader.readList(" + memberName + ".mValue);";
	}
	else
	{
		return "success = success && reader.read(out " + memberName + ".mValue);";
	}
}

string CodeNetPacket::singleMemberWriteLineCSharp(const string& memberName, const string& memberType)
{
	if (memberType == "string")
	{
		if (mUseILRuntime)
		{
			return "writer.writeString(" + memberName + ".mValue);";
		}
		else
		{
			return "writer.writeString(" + memberName + ");";
		}
	}
	else if (startWith(memberType, "Vector<"))
	{
		if (mUseILRuntime)
		{
			return "writer.writeList(" + memberName + ".mValue);";
		}
		else
		{
			return "writer.writeList(" + memberName + ");";
		}
	}
	if (memberType == "bool" || memberType == "char" || memberType == "byte" || memberType == "sbyte" || memberType == "short" || 
		memberType == "ushort" || memberType == "int" || memberType == "uint" || memberType == "llong" || memberType == "ullong" || 
		memberType == "float" || memberType == "double" || memberType == "Vector2" || memberType == "Vector2UShort" || 
		memberType == "Vector2Int" || memberType == "Vector2UInt" || memberType == "Vector3" || memberType == "Vector3Int" || memberType == "Vector4")
	{
		if (mUseILRuntime)
		{
			return "writer.write(" + memberName + ".mValue);";
		}
		else
		{
			return "writer.write(" + memberName + ");";
		}
	}
	ERROR("��֧�ֵ�����:" + memberType);
	return "";
}

myVector<string> CodeNetPacket::multiMemberReadLineCSharp(const myVector<string>& memberNameList, const string& memberType, bool supportCustom)
{
	myVector<string> list;
	if (memberType == "string")
	{
		FOR_VECTOR(memberNameList)
		{
			list.push_back("success = success && reader.readString(out " + memberNameList[i] + ");");
		}
		return list;
	}
	if (memberType == "bool")
	{
		FOR_VECTOR(memberNameList)
		{
			list.push_back("success = success && reader.read(out " + memberNameList[i] + ");");
		}
		return list;
	}
	if (startWith(memberType, "Vector<"))
	{
		int lastPos;
		findSubstr(memberType, ">", &lastPos);
		const string elementType = memberType.substr(strlen("Vector<"), lastPos - strlen("Vector<"));
		FOR_VECTOR(memberNameList)
		{
			if (elementType == "string" || elementType == "char" || elementType == "short" || elementType == "int" || elementType == "llong" || 
				elementType == "byte" || elementType == "ushort" || elementType == "uint" || elementType == "ullong" || elementType == "float" || 
				elementType == "double" || elementType == "Vector2" || elementType == "Vector2UShort" || elementType == "Vector2Int" || elementType == "Vector2UInt" ||
				elementType == "Vector3" || elementType == "Vector3Int" || elementType == "Vector4")
			{
				list.push_back("success = success && reader.readList(" + memberNameList[i] + ");");
			}
			else if (elementType == "bool")
			{
				ERROR("��֧��bool���б�");
			}
			else
			{
				if (supportCustom)
				{
					list.push_back("success = success && reader.readCustomList(" + memberNameList[i] + ");");
				}
				else
				{
					ERROR("��֧���Զ���ṹ��:" + memberType);
				}
			}
		}
		return list;
	}
	if (memberType == "byte" || memberType == "sbyte" || memberType == "short" || memberType == "ushort" || memberType == "int" || 
		memberType == "uint" || memberType == "llong" || memberType == "ullong" || memberType == "float" || memberType == "double" || 
		memberType == "Vector2" || memberType == "Vector2UShort" || memberType == "Vector2Int" || memberType == "Vector2UInt" || memberType == "Vector3" ||
		memberType == "Vector3Int" || memberType == "Vector4")
	{
		const int memberCount = memberNameList.size();
		if (memberCount <= 4)
		{
			string members;
			FOR_I(memberCount)
			{
				members += "out " + memberNameList[i];
				if (i < memberCount - 1)
				{
					members += ", ";
				}
			}
			list.push_back("success = success && reader.read(" + members + ");");
		}
		else
		{
			string tempVarName = "values" + memberNameList[0].substr(1, memberNameList[0].find_first_of('.') - 1);
			string csharpType = cppTypeToCSharpType(memberType);
			list.push_back("Span<" + csharpType + "> " + tempVarName + " = stackalloc " + csharpType + "[" + intToString(memberCount) + "];");
			list.push_back("success = success && reader.read(ref " + tempVarName + ");");
			for (int i = 0; i < memberCount; ++i)
			{
				list.push_back(memberNameList[i] + " = " + tempVarName + "[" + intToString(i) + "];");
			}
		}
	}
	else
	{
		if (supportCustom)
		{
			FOR_VECTOR(memberNameList)
			{
				list.push_back("success = success && reader.readCustomList(" + memberNameList[i] + ");");
			}
		}
		else
		{
			ERROR("��֧���Զ���ṹ��:" + memberType);
		}
	}
	return list;
}

myVector<string> CodeNetPacket::multiMemberWriteLineCSharp(const myVector<string>& memberNameList, const string& memberType, bool supportCustom)
{
	myVector<string> list;
	if (memberType == "string")
	{
		FOR_VECTOR(memberNameList)
		{
			list.push_back("writer.writeString(out " + memberNameList[i] + ");");
		}
		return list;
	}
	if (memberType == "bool")
	{
		FOR_VECTOR(memberNameList)
		{
			list.push_back("writer.write(out " + memberNameList[i] + ");");
		}
		return list;
	}
	if (startWith(memberType, "Vector<"))
	{
		int lastPos;
		findSubstr(memberType, ">", &lastPos);
		const string elementType = memberType.substr(strlen("Vector<"), lastPos - strlen("Vector<"));
		FOR_VECTOR(memberNameList)
		{
			if (elementType == "string" || elementType == "char" || elementType == "short" || elementType == "int" || elementType == "llong" ||
				elementType == "byte" || elementType == "ushort" || elementType == "uint" || elementType == "ullong" || elementType == "float" ||
				elementType == "double" || elementType == "Vector2" || elementType == "Vector2UShort" || elementType == "Vector2Int" || elementType == "Vector2UInt" ||
				elementType == "Vector3" || elementType == "Vector4")
			{
				list.push_back("writer.writeList(" + memberNameList[i] + ");");
			}
			else if (elementType == "bool")
			{
				ERROR("��֧��bool���б�");
			}
			else
			{
				if (supportCustom)
				{
					list.push_back("writer.writeCustomList(" + memberNameList[i] + ");");
				}
				else
				{
					ERROR("��֧���Զ���ṹ��:" + memberType);
				}
			}
		}
		return list;
	}
	if (memberType == "byte" || memberType == "sbyte" || memberType == "short" || memberType == "ushort" || memberType == "int" || 
		memberType == "uint" || memberType == "llong" || memberType == "ullong" || memberType == "float" || memberType == "double")
	{
		string members;
		FOR_VECTOR(memberNameList)
		{
			members += memberNameList[i];
			if (i < memberNameList.size() - 1)
			{
				members += ", ";
			}
		}
		list.push_back("writer.write(stackalloc " + cppTypeToCSharpType(memberType) + "[" + intToString(memberNameList.size()) + "]{ " + members + " });");
	}
	else
	{
		if (supportCustom)
		{
			FOR_VECTOR(memberNameList)
			{
				list.push_back("writer.writeCustom(" + memberNameList[i] + ");");
			}
		}
		else
		{
			ERROR("��֧���Զ���ṹ��:" + memberType);
		}
	}
	return list;
}

string CodeNetPacket::singleMemberWriteLine(const string& memberName, const string& memberType, bool supportCustom)
{
	if (memberType == "string")
	{
		return "success = success && serializer->writeString(" + memberName + ");";
	}
	else if (startWith(memberType, "Vector<"))
	{
		int lastPos;
		findSubstr(memberType, ">", &lastPos);
		const string elementType = memberType.substr(strlen("Vector<"), lastPos - strlen("Vector<"));
		if (elementType == "string")
		{
			return "success = success && serializer->writeStringList(" + memberName + ");";
		}
		else if (elementType == "bool")
		{
			ERROR("��֧��bool���б�");
			return "";
		}
		else if (elementType == "char" || elementType == "short" || elementType == "int" || elementType == "llong")
		{
			return "success = success && serializer->writeSignedList(" + memberName + ");";
		}
		else if (elementType == "byte" || elementType == "ushort" || elementType == "uint" || elementType == "ullong")
		{
			return "success = success && serializer->writeUnsignedList(" + memberName + ");";
		}
		else if (elementType == "float")
		{
			return "success = success && serializer->writeFloatList(" + memberName + ");";
		}
		else if (elementType == "double")
		{
			return "success = success && serializer->writeDoubleList(" + memberName + ");";
		}
		else if (elementType == "Vector2")
		{
			return "success = success && serializer->writeVector2List(" + memberName + ");";
		}
		else if (elementType == "Vector2UInt")
		{
			return "success = success && serializer->writeVector2UIntList(" + memberName + ");";
		}
		else if (elementType == "Vector2Int")
		{
			return "success = success && serializer->writeVector2IntList(" + memberName + ");";
		}
		else if (elementType == "Vector2UShort")
		{
			return "success = success && serializer->writeVector2UShortList(" + memberName + ");";
		}
		else if (elementType == "Vector3")
		{
			return "success = success && serializer->writeVector3List(" + memberName + ");";
		}
		else if (elementType == "Vector4")
		{
			return "success = success && serializer->writeVector4List(" + memberName + ");";
		}
		else
		{
			if (supportCustom)
			{
				return "success = success && serializer->writeCustomList(" + memberName + ");";
			}
			else
			{
				ERROR("��֧���Զ���ṹ��:" + memberName);
			}
		}
	}
	else if (memberType == "bool")
	{
		return "success = success && serializer->writeBool(" + memberName + ");";
	}
	else if (memberType == "char" || memberType == "short" || memberType == "int" || memberType == "llong")
	{
		return "success = success && serializer->writeSigned(" + memberName + ");";
	}
	else if (memberType == "byte" || memberType == "ushort" || memberType == "uint" || memberType == "ullong")
	{
		return "success = success && serializer->writeUnsigned(" + memberName + ");";
	}
	else if (memberType == "float")
	{
		return "success = success && serializer->writeFloat(" + memberName + ");";
	}
	else if (memberType == "double")
	{
		return "success = success && serializer->writeDouble(" + memberName + ");";
	}
	else if (memberType == "Vector2")
	{
		return "success = success && serializer->writeVector2(" + memberName + ");";
	}
	else if (memberType == "Vector2UInt")
	{
		return "success = success && serializer->writeVector2UInt(" + memberName + ");";
	}
	else if (memberType == "Vector2Int")
	{
		return "success = success && serializer->writeVector2Int(" + memberName + ");";
	}
	else if (memberType == "Vector2UShort")
	{
		return "success = success && serializer->writeVector2UShort(" + memberName + ");";
	}
	else if (memberType == "Vector3")
	{
		return "success = success && serializer->writeVector3(" + memberName + ");";
	}
	else if (memberType == "Vector4")
	{
		return "success = success && serializer->writeVector4(" + memberName + ");";
	}
	if (supportCustom)
	{
		return "success = success && serializer->writeCustom(" + memberName + ");";
	}
	else
	{
		ERROR("��֧���Զ���ṹ��:" + memberName);
		return "";
	}
}

bool CodeNetPacket::isSameType(const string& sourceType, const string& curType)
{
	// string��bool���Ͳ��ϲ�
	if (sourceType == "string" || sourceType == "bool" || curType == "string" || curType == "bool")
	{
		return false;
	}
	if (sourceType == curType)
	{
		return true;
	}
	if (sourceType == "float")
	{
		return curType == "Vector2" || curType == "Vector3" || curType == "Vector4";
	}
	if (sourceType == "ushort")
	{
		return curType == "Vector2UShort";
	}
	if (sourceType == "int")
	{
		return curType == "Vector2Int" || curType == "Vector3Int";
	}
	if (sourceType == "uint")
	{
		return curType == "Vector2UInt" || curType == "Vector3UInt";
	}
	return false;
}

string CodeNetPacket::toPODType(const string& type)
{
	if (type == "Vector2" || type == "Vector3" || type == "Vector4")
	{
		return "float";
	}
	if (type == "Vector2UShort")
	{
		return "ushort";
	}
	if (type == "Vector2Int" || type == "Vector3Int")
	{
		return "int";
	}
	if (type == "Vector2UInt" || type == "Vector3UInt")
	{
		return "uint";
	}
	return type;
}

bool CodeNetPacket::isCustomStructType(const string& type)
{
	return startWith(type, "NetStruct");
}

void CodeNetPacket::generateMemberGroup(const myVector<PacketMember>& memberList, myVector<myVector<PacketMember>>& memberNameList)
{
	myVector<PacketMember> sameTypeMemberList;
	FOR_VECTOR(memberList)
	{
		const PacketMember& item = memberList[i];
		if (i > 0)
		{
			const PacketMember& lastItem = memberList[i - 1];
			// �����һ����Ա�����뵱ǰ�����Ͳ�һ��,���ߵ�ǰ��һ��optional����,��֮ǰ�ı���д��,�Զ���Ľṹ�����͵ı���Ҳ����鵽һ��
			if (item.mOptional || lastItem.mOptional || !isSameType(toPODType(item.mTypeName), toPODType(lastItem.mTypeName)) || isCustomStructType(item.mTypeName))
			{
				memberNameList.push_back(sameTypeMemberList);
				sameTypeMemberList.clear();
			}
			// ����������
			sameTypeMemberList.push_back(item);
		}
		// �����жϵ�0��Ԫ��,��Ϊû����һ���������Ƚ�
		else
		{
			if (item.mOptional)
			{
				memberNameList.push_back(myVector<PacketMember>{ item });
			}
			else
			{
				sameTypeMemberList.push_back(item);
			}
		}
	}
	if (sameTypeMemberList.size() > 0)
	{
		memberNameList.push_back(sameTypeMemberList);
	}
}

string CodeNetPacket::expandMembersInGroup(const myVector<PacketMember>& memberList, myVector<string>& memberNameList)
{
	if (memberList.size() == 0)
	{
		return "";
	}
	FOR_VECTOR(memberList)
	{
		const string& typeName = memberList[i].mTypeName;
		const string& memberName = memberList[i].mMemberName;
		if (typeName == "Vector2" || typeName == "Vector2UShort" || typeName == "Vector2Int" || typeName == "Vector2UInt")
		{
			memberNameList.push_back(memberName + ".x");
			memberNameList.push_back(memberName + ".y");
		}
		else if (typeName == "Vector3" || typeName == "Vector3Int")
		{
			memberNameList.push_back(memberName + ".x");
			memberNameList.push_back(memberName + ".y");
			memberNameList.push_back(memberName + ".z");
		}
		else if (typeName == "Vector4")
		{
			memberNameList.push_back(memberName + ".x");
			memberNameList.push_back(memberName + ".y");
			memberNameList.push_back(memberName + ".z");
			memberNameList.push_back(memberName + ".w");
		}
		else
		{
			memberNameList.push_back(memberName);
		}
	}
	return toPODType(memberList[0].mTypeName);
}

string CodeNetPacket::expandMembersInGroupCSharp(const myVector<PacketMember>& memberList, myVector<string>& memberNameList, bool supportSimplify)
{
	if (memberList.size() == 0)
	{
		return "";
	}
	FOR_VECTOR(memberList)
	{
		const string& typeName = memberList[i].mTypeName;
		const string& memberName = memberList[i].mMemberName;
		if (typeName == "Vector2" || typeName == "Vector2UShort" || typeName == "Vector2Int" || typeName == "Vector2UInt")
		{
			memberNameList.push_back(memberName + ".mValue.x");
			memberNameList.push_back(memberName + ".mValue.y");
		}
		else if (typeName == "Vector3" || typeName == "Vector3Int")
		{
			memberNameList.push_back(memberName + ".mValue.x");
			memberNameList.push_back(memberName + ".mValue.y");
			memberNameList.push_back(memberName + ".mValue.z");
		}
		else if (typeName == "Vector4")
		{
			memberNameList.push_back(memberName + ".mValue.x");
			memberNameList.push_back(memberName + ".mValue.y");
			memberNameList.push_back(memberName + ".mValue.z");
			memberNameList.push_back(memberName + ".mValue.w");
		}
		else
		{
			if (mUseILRuntime || !supportSimplify)
			{
				memberNameList.push_back(memberName + ".mValue");
			}
			else
			{
				memberNameList.push_back(memberName);
			}
		}
	}
	return toPODType(memberList[0].mTypeName);
}

void CodeNetPacket::generateCppPacketReadWrite(const PacketInfo& packetInfo, myVector<string>& generateCodes)
{
	if (packetInfo.mMemberList.size() > 0)
	{
		myVector<myVector<PacketMember>> memberGroupList;
		generateMemberGroup(packetInfo.mMemberList, memberGroupList);

		// readFromBuffer
		generateCodes.push_back("\tbool readFromBuffer(SerializerBitRead* reader) override");
		generateCodes.push_back("\t{");
		generateCodes.push_back("\t\tbool success = true;");
		FOR_VECTOR(memberGroupList)
		{
			const myVector<PacketMember>& memberList = memberGroupList[i];
			// ������ֻ��һ����Ա,���п�����optional
			if (memberList.size() == 1)
			{
				const PacketMember& item = memberList[0];
				if (item.mOptional)
				{
					generateCodes.push_back("\t\tif (isFieldValid(Field::" + item.mMemberNameNoPrefix + "))");
					generateCodes.push_back("\t\t{");
					generateCodes.push_back("\t\t\t" + singleMemberReadLine(item.mMemberName, item.mTypeName, true));
					generateCodes.push_back("\t\t}");
				}
				else
				{
					generateCodes.push_back("\t\t" + singleMemberReadLine(item.mMemberName, item.mTypeName, true));
				}
			}
			else
			{
				myVector<string> nameList;
				string groupTypeName = expandMembersInGroup(memberList, nameList);
				myVector<string> list = multiMemberReadLine(nameList, groupTypeName, true);
				FOR_VECTOR(list)
				{
					generateCodes.push_back("\t\t" + list[i]);
				}
			}
		}
		generateCodes.push_back("\t\treturn success;");
		generateCodes.push_back("\t}");

		// writeToBuffer
		generateCodes.push_back("\tbool writeToBuffer(SerializerBitWrite* serializer) const override");
		generateCodes.push_back("\t{");
		generateCodes.push_back("\t\tbool success = true;");
		FOR_VECTOR(memberGroupList)
		{
			const myVector<PacketMember>& memberList = memberGroupList[i];
			// ������ֻ��һ����Ա,���п�����optional
			if (memberList.size() == 1)
			{
				const PacketMember& item = memberList[0];
				if (item.mOptional)
				{
					generateCodes.push_back("\t\tif (isFieldValid(Field::" + item.mMemberNameNoPrefix + "))");
					generateCodes.push_back("\t\t{");
					generateCodes.push_back("\t\t\t" + singleMemberWriteLine(item.mMemberName, item.mTypeName, true));
					generateCodes.push_back("\t\t}");
				}
				else
				{
					generateCodes.push_back("\t\t" + singleMemberWriteLine(item.mMemberName, item.mTypeName, true));
				}
			}
			else
			{
				myVector<string> nameList;
				string groupTypeName = expandMembersInGroup(memberList, nameList);
				myVector<string> list = multiMemberWriteLine(nameList, groupTypeName, true);
				FOR_VECTOR(list)
				{
					generateCodes.push_back("\t\t" + list[i]);
				}
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
				item.mTypeName == "Vector2UInt" || 
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

// SCPacket.h�ļ�
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
	generateCodes.push_back("class " + packetName + " : public Packet");
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
					ERROR("��ѡ�ֶε��±겻�ܳ���63");
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
	generateCodes.push_back("\t\tmType = PACKET_TYPE::" + packetName + ";");
	generateCodes.push_back("\t\tmShowInfo = " + boolToString(packetInfo.mShowInfo) + ";");
	generateCodes.push_back("\t}");
	generateCodes.push_back("\tstatic " + packetName + "& get()");
	generateCodes.push_back("\t{");
	generateCodes.push_back("\t\tmStaticObject.resetProperty();");
	generateCodes.push_back("\t\treturn mStaticObject;");
	generateCodes.push_back("\t}");
	generateCodes.push_back("\tstatic constexpr ushort getStaticType() { return PACKET_TYPE::" + packetName + "; }");
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
		replaceFileLines(headerFullPath,
			[](const string& codeLine) { return codeLine == "// auto generate start"; },
			[](const string& codeLine) { return endWith(codeLine, "// auto generate end"); }, generateCodes);
	}
	else
	{
		myVector<string> codeList;
		codeList.push_back("#pragma once");
		codeList.push_back("");
		codeList.push_back("#include \"Packet.h\"");
		codeList.push_back("#include \"GamePacketDefine.h\"");
		codeList.push_back("");
		codeList.push_back("// auto generate start");
		codeList.addRange(generateCodes);
		codeList.push_back("\t// auto generate end");
		codeList.push_back("\tvoid debugInfo(MyString<1024>& buffer) override");
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
		myVector<string> newLines;
		newLines.push_back(packetName + " " + packetName + "::mStaticObject;");
		replaceFileLines(cppFullPath,
			[](const string& codeLine) { return codeLine == "// auto generate start"; },
			[](const string& codeLine) { return endWith(codeLine, "// auto generate end"); }, newLines);
	}
	else
	{
		myVector<string> codeList;
		codeList.push_back("#include \"GameHeader.h\"");
		codeList.push_back("");
		codeList.push_back("// auto generate start");
		codeList.push_back(packetName + " " + packetName + "::mStaticObject;");
		codeList.push_back("// auto generate end");
		writeFile(cppFullPath, ANSIToUTF8(codeListToString(codeList).c_str(), true));
	}
}

// PacketDefine.cs�ļ�
void CodeNetPacket::generateCSharpPacketDefineFile(const myVector<PacketInfo>& gamePacketList, const string& filePath)
{
	string str;
	line(str, "using System;");
	
	line(str, "");
	line(str, "public class PACKET_TYPE");
	line(str, "{");
	line(str, "\tpublic static ushort MIN = 0;");
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

// PacketRegister.cs�ļ�
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
		if (mUseILRuntime)
		{
			line(str, "\t\tregistePacket(typeof(" + packetList[i].mPacketName + "), PACKET_TYPE." + packetNameToUpper(packetList[i].mPacketName) + ");");
		}
		else
		{
			line(str, "\t\tregistePacket<" + packetList[i].mPacketName + ">(PACKET_TYPE." + packetNameToUpper(packetList[i].mPacketName) + ");");
		}
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
		if (mUseILRuntime)
		{
			line(str, "\t\tregistePacket(typeof(" + packetList[i].mPacketName + "), PACKET_TYPE." + packetNameToUpper(packetList[i].mPacketName) + ");");
		}
		else
		{
			line(str, "\t\tregistePacket<" + packetList[i].mPacketName + ">(PACKET_TYPE." + packetNameToUpper(packetList[i].mPacketName) + ");");
		}
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
	line(str, "\tprotected static void registePacket<T>(ushort type) where T : NetPacketFrame");
	line(str, "\t{");
	line(str, "\t\tmNetPacketTypeManager.registePacket(typeof(T), type);");
	line(str, "\t}");
	line(str, "\tprotected static void registeUDP(ushort type, string packetName)");
	line(str, "\t{");
	line(str, "\t\tmNetPacketTypeManager.registeUDPPacketName(type, packetName);");
	line(str, "\t}");
	line(str, "}", false);

	writeFile(filePath + "PacketRegister.cs", ANSIToUTF8(str.c_str(), true));
}

// CSPacket.cs�ļ�
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

	myVector<myVector<PacketMember>> memberGroupList;
	generateMemberGroup(packetInfo.mMemberList, memberGroupList);

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

		// get
		if (startWith(packetName, "CS"))
		{
			generateCodes.push_back("\tpublic static " + packetName + " get() { return PACKET<" + packetName + ">(); }");
		}
		if (packetName == "SCPKSettlement")
		{
			int a = 0;
		}

		// read
		generateCodes.push_back("\tpublic override bool read(SerializerBitRead reader, ulong fieldFlag)");
		generateCodes.push_back("\t{");
		generateCodes.push_back("\t\tbool success = true;");
		FOR_VECTOR(memberGroupList)
		{
			const myVector<PacketMember>& memberGroup = memberGroupList[i];
			if (memberGroup.size() == 1)
			{
				const PacketMember& item = memberGroup[0];
				// ��ѡ�ֶ���Ҫ�ر��ж�һ��
				if (item.mOptional)
				{
					generateCodes.push_back("\t\tif (hasBit(fieldFlag, " + intToString(item.mIndex) + "))");
					generateCodes.push_back("\t\t{");
					generateCodes.push_back("\t\t\tsuccess = success && " + item.mMemberName + ".read(reader);");
					generateCodes.push_back("\t\t}");
				}
				else
				{
					generateCodes.push_back("\t\tsuccess = success && " + item.mMemberName + ".read(reader);");
				}
			}
			else
			{
				myVector<string> nameList;
				string groupTypeName = expandMembersInGroupCSharp(memberGroup, nameList, false);
				myVector<string> list = multiMemberReadLineCSharp(nameList, groupTypeName, true);
				FOR_VECTOR(list)
				{
					generateCodes.push_back("\t\t" + list[i]);
				}
			}
		}
		generateCodes.push_back("\t\treturn success;");
		generateCodes.push_back("\t}");

		// write
		generateCodes.push_back("\tpublic override void write(SerializerBitWrite writer, out ulong fieldFlag)");
		generateCodes.push_back("\t{");
		generateCodes.push_back("\t\tbase.write(writer, out fieldFlag);");
		FOR_VECTOR(memberGroupList)
		{
			const myVector<PacketMember>& memberGroup = memberGroupList[i];
			if (memberGroup.size() == 1)
			{
				const PacketMember& item = memberGroup[0];
				// ��ѡ�ֶ���Ҫ�ر��ж�һ��
				if (item.mOptional)
				{
					generateCodes.push_back("\t\tif (" + item.mMemberName + ".mValid)");
					generateCodes.push_back("\t\t{");
					generateCodes.push_back("\t\t\tsetBitOne(ref fieldFlag, " + intToString(item.mIndex) + ");");
					generateCodes.push_back("\t\t\t" + item.mMemberName + ".write(writer);");
					generateCodes.push_back("\t\t}");
				}
				else
				{
					generateCodes.push_back("\t\t" + item.mMemberName + ".write(writer);");
				}
			}
			else
			{
				myVector<string> nameList;
				string groupTypeName = expandMembersInGroupCSharp(memberGroup, nameList, true);
				myVector<string> list = multiMemberWriteLineCSharp(nameList, groupTypeName, true);
				FOR_VECTOR(list)
				{
					generateCodes.push_back("\t\t" + list[i]);
				}
			}
		}
		generateCodes.push_back("\t}");
	}
	else
	{
		// get
		if (startWith(packetName, "CS"))
		{
			generateCodes.push_back("\tpublic static " + packetName + " get() { return PACKET<" + packetName + ">(); }");
		}
	}
	if (isFileExist(fullPath))
	{
		replaceFileLines(fullPath, 
			[](const string& codeLine) { return codeLine == "// auto generate start"; },
			[](const string& codeLine) { return endWith(codeLine, "// auto generate end"); }, generateCodes);
	}
	else
	{
		myVector<string> codeList;
		codeList.push_back("using static FrameUtility;");
		codeList.push_back("using static GU;");
		codeList.push_back("");
		codeList.push_back("// auto generate start");
		codeList.addRange(generateCodes);
		codeList.push_back("\t// auto generate end");
		if (startWith(packetName, "SC"))
		{
			codeList.push_back("\tpublic override void execute()");
			codeList.push_back("\t{}");
		}
		else if (startWith(packetName, "CS"))
		{
			codeList.push_back("\tpublic static void send()");
			codeList.push_back("\t{");
			codeList.push_back("\t\tsendPacket(get());");
			codeList.push_back("\t}");
		}
		codeList.push_back("}");
		writeFile(fullPath, ANSIToUTF8(codeListToString(codeList).c_str(), true));
	}
}

void CodeNetPacket::generateCSharpStruct(const PacketStruct& structInfo, const string& gamePath, const string& hotFixPath)
{
	bool hasOptional = false;
	for (const PacketMember& item : structInfo.mMemberList)
	{
		hasOptional |= item.mOptional;
	}
	myVector<string> codeList;
	codeList.push_back("using System;");
	codeList.push_back("using UnityEngine;");
	codeList.push_back("using System.Collections;");
	codeList.push_back("using System.Collections.Generic;");
	codeList.push_back("using static FrameUtility;");
	if (hasOptional)
	{
		codeList.push_back("using static BinaryUtility;");
	}
	codeList.push_back("");
	codeList.push_back("public class " + structInfo.mStructName + " : NetStruct");
	codeList.push_back("{");
	for (const PacketMember& item : structInfo.mMemberList)
	{
		codeList.push_back("\t" + cSharpMemberDeclareString(item));
	}

	// ����
	codeList.push_back("\tpublic " + structInfo.mStructName + "()");
	codeList.push_back("\t{");
	for (const PacketMember& item : structInfo.mMemberList)
	{
		codeList.push_back("\t\taddParam(" + item.mMemberName + ", " + (item.mOptional ? "true" : "false") + ");");
	}
	codeList.push_back("\t}");
	
	myVector<myVector<PacketMember>> memberGroupList;
	generateMemberGroup(structInfo.mMemberList, memberGroupList);

	// readInternal
	codeList.push_back("\tprotected override bool readInternal(ulong fieldFlag, SerializerBitRead reader)");
	codeList.push_back("\t{");
	codeList.push_back("\t\tbool success = true;");
	FOR_VECTOR(memberGroupList)
	{
		const myVector<PacketMember>& memberGroup = memberGroupList[i];
		if (memberGroup.size() == 1)
		{
			const PacketMember& member = memberGroup[0];
			if (member.mOptional)
			{
				codeList.push_back("\t\tif (" + member.mMemberName + ".mValid = hasBit(fieldFlag, " + intToString(member.mIndex) + "))");
				codeList.push_back("\t\t{");
				codeList.push_back("\t\t\t" + singleMemberReadLineCSharp(member.mMemberName, member.mTypeName));
				codeList.push_back("\t\t}");
			}
			else
			{
				codeList.push_back("\t\t" + singleMemberReadLineCSharp(member.mMemberName, member.mTypeName));
			}
		}
		else
		{
			myVector<string> nameList;
			string groupTypeName = expandMembersInGroupCSharp(memberGroup, nameList, false);
			myVector<string> list = multiMemberReadLineCSharp(nameList, groupTypeName, false);
			FOR_VECTOR(list)
			{
				codeList.push_back("\t\t" + list[i]);
			}
		}
	}
	codeList.push_back("\t\treturn success;");
	codeList.push_back("\t}");

	// write
	codeList.push_back("\tpublic override void write(SerializerBitWrite writer)");
	codeList.push_back("\t{");
	codeList.push_back("\t\tbase.write(writer);");
	FOR_VECTOR(memberGroupList)
	{
		const myVector<PacketMember>& memberGroup = memberGroupList[i];
		if (memberGroup.size() == 1)
		{
			const PacketMember& member = memberGroup[0];
			if (member.mOptional)
			{
				codeList.push_back("\t\tif (" + member.mMemberName + ".mValid)");
				codeList.push_back("\t\t{");
				codeList.push_back("\t\t\t" + singleMemberWriteLineCSharp(member.mMemberName, member.mTypeName));
				codeList.push_back("\t\t}");
			}
			else
			{
				codeList.push_back("\t\t" + singleMemberWriteLineCSharp(member.mMemberName, member.mTypeName));
			}
		}
		else
		{
			myVector<string> nameList;
			string groupTypeName = expandMembersInGroupCSharp(memberGroup, nameList, true);
			myVector<string> list = multiMemberWriteLineCSharp(nameList, groupTypeName, false);
			FOR_VECTOR(list)
			{
				codeList.push_back("\t\t" + list[i]);
			}
		}
	}
	codeList.push_back("\t}");

	// resetProperty
	codeList.push_back("\tpublic override void resetProperty()");
	codeList.push_back("\t{");
	codeList.push_back("\t\tbase.resetProperty();");
	for (const PacketMember& item : structInfo.mMemberList)
	{
		codeList.push_back("\t\t" + item.mMemberName + ".resetProperty();");
	}
	codeList.push_back("\t}");

	codeList.push_back("}");
	codeList.push_back("");

	// StructList
	codeList.push_back("public class " + structInfo.mStructName + "_List : SerializableBit, IEnumerable<" + structInfo.mStructName + ">");
	codeList.push_back("{");
	if (mUseILRuntime)
	{
		codeList.push_back("\tpublic List<" + structInfo.mStructName + "> mList = new List<" + structInfo.mStructName + ">();");
	}
	else
	{
		codeList.push_back("\tpublic List<" + structInfo.mStructName + "> mList = new();");
	}
	codeList.push_back("\tpublic " + structInfo.mStructName + " this[int index]");
	codeList.push_back("\t{");
	codeList.push_back("\t\tget { return mList[index]; }");
	codeList.push_back("\t\tset { mList[index] = value; }");
	codeList.push_back("\t}");
	codeList.push_back("\tpublic int Count{ get { return mList.Count; } }");
	codeList.push_back("\tpublic override bool read(SerializerBitRead reader)");
	codeList.push_back("\t{");
	if (mUseILRuntime)
	{
		codeList.push_back("\t\treturn reader.readCustomList(mList, typeof(" + structInfo.mStructName + "));");
	}
	else
	{
		codeList.push_back("\t\treturn reader.readCustomList(mList);");
	}
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
	codeList.push_back("\tpublic IEnumerator<" + structInfo.mStructName + "> GetEnumerator(){ return mList.GetEnumerator(); }");
	codeList.push_back("\tIEnumerator IEnumerable.GetEnumerator() { return mList.GetEnumerator(); }");
	codeList.push_back("}");

	const string fullPath = (structInfo.mHotFix ? hotFixPath : gamePath) + structInfo.mStructName + ".cs";
	writeFile(fullPath, ANSIToUTF8(codeListToString(codeList).c_str(), true));
}