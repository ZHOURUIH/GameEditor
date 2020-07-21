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
	deleteFolder(cppHeaderFilePath);
	// c#的只删除代码文件,不删除meta文件
	myVector<string> csFileList;
	findFiles(csFilePath, csFileList, ".cs");
	FOR_VECTOR_CONST(csFileList)
	{
		deleteFile(csFileList[i]);
	}
	FOR_VECTOR_CONST(packetInfoList)
	{
		// 生成代码文件
		// .h代码
		generateCppPacketHeaderFile(packetInfoList[i], cppHeaderFilePath);
		// .cs代码
		generateCSharpFile(packetInfoList[i], csFilePath);
	}
	// c++
	generateCppPacketDefineFile(packetInfoList, cppPacketDefineFilePath);
	generateCppPacketRegisteFile(packetInfoList, cppPacketDefineFilePath);
	generateCppPacketTotalHeaderFile(packetInfoList, cppPacketDefineFilePath);
	// c#
	generateCSharpPacketDefineFile(packetInfoList, csPacketDefineFilePath);
	generateCSharpPacketRegisteFile(packetInfoList, csPacketDefineFilePath);
}

void CodeGenerator::generateSQLiteCode(string cppDataPath, string csDataPath)
{
	// 解析模板文件
	string fileContent;
	openTxtFile("SQLite.txt", fileContent);
	if (fileContent.length() == 0)
	{
		ERROR("未找到表格格式文件SQLite.txt");
		return;
	}
	fileContent = UTF8ToANSI(fileContent.c_str(), true);
	myVector<string> lines;
	split(fileContent.c_str(), "\r\n", lines);
	bool packetStart = false;
	myVector<SQLiteInfo> sqliteInfoList;
	SQLiteInfo tempInfo;
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
		// 成员变量列表起始
		if (line == "{")
		{
			packetStart = true;
			string lastLine = lines[i - 1];
			int startIndex = -1;
			int endIndex = -1;
			findString(lastLine.c_str(), "[", &startIndex);
			findString(lastLine.c_str(), "]", &endIndex, startIndex);
			if (startIndex >= 0 && endIndex >= 0)
			{
				tempInfo.mSQLiteName = lastLine.substr(0, startIndex);
				string owner = lastLine.substr(startIndex, endIndex - startIndex + 1);
				if (owner == "[Client]")
				{
					tempInfo.mOwner = SQLITE_OWNER::CLIENT_ONLY;
				}
				else if (owner == "[Server]")
				{
					tempInfo.mOwner = SQLITE_OWNER::SERVER_ONLY;
				}
				else
				{
					tempInfo.mOwner = SQLITE_OWNER::BOTH;
				}
			}
			else
			{
				tempInfo.mSQLiteName = lastLine;
				tempInfo.mOwner = SQLITE_OWNER::BOTH;
			}
			tempInfo.mMemberList.clear();
			// 添加默认的ID字段
			SQLiteMember idMember;
			idMember.mMemberName = "ID";
			idMember.mOwner = SQLITE_OWNER::BOTH;
			idMember.mTypeName = "int";
			tempInfo.mMemberList.push_back(idMember);
			continue;
		}
		// 成员变量列表结束
		if (line == "}")
		{
			sqliteInfoList.push_back(tempInfo);
			packetStart = false;
			continue;
		}
		if (packetStart)
		{
			tempInfo.mMemberList.push_back(parseSQLiteMemberLine(line));
		}
	}
	deleteFolder(cppDataPath);
	// c#的只删除代码文件,不删除meta文件
	myVector<string> csFileList;
	findFiles(csDataPath, csFileList, ".cs");
	FOR_VECTOR_CONST(csFileList)
	{
		deleteFile(csFileList[i]);
	}
	FOR_VECTOR_CONST(sqliteInfoList)
	{
		// 生成代码文件
		// .h代码
		generateCppSQLiteDataFile(sqliteInfoList[i], cppDataPath);
		// .cs代码
		generateCSharpSQLiteDataFile(sqliteInfoList[i], csDataPath);
	}
	// 在上一层目录生成SQLiteHeader.h文件
	string headerPath = cppDataPath;
	if (headerPath[headerPath.length() - 1] == '/' || headerPath[headerPath.length() - 1] == '\\')
	{
		headerPath = headerPath.substr(0, headerPath.length() - 1);
	}
	headerPath = getFilePath(headerPath);
	generateCppSQLiteTotalHeaderFile(sqliteInfoList, headerPath);
}

void CodeGenerator::generateMySQLCode(string cppDataPath)
{
	// 解析模板文件
	string fileContent;
	openTxtFile("MySQL.txt", fileContent);
	if (fileContent.length() == 0)
	{
		ERROR("未找到表格格式文件MySQL.txt");
		return;
	}
	fileContent = UTF8ToANSI(fileContent.c_str(), true);
	myVector<string> lines;
	split(fileContent.c_str(), "\r\n", lines);
	bool packetStart = false;
	myVector<MySQLInfo> mysqlInfoList;
	MySQLInfo tempInfo;
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
		// 成员变量列表起始
		if (line == "{")
		{
			packetStart = true;
			tempInfo.mSQLiteName = lines[i - 1];
			tempInfo.mMemberList.clear();
			continue;
		}
		// 成员变量列表结束
		if (line == "}")
		{
			mysqlInfoList.push_back(tempInfo);
			packetStart = false;
			continue;
		}
		if (packetStart)
		{
			tempInfo.mMemberList.push_back(parseMySQLMemberLine(line));
		}
	}
	deleteFolder(cppDataPath);
	FOR_VECTOR_CONST(mysqlInfoList)
	{
		// 生成代码文件
		generateCppMySQLDataFile(mysqlInfoList[i], cppDataPath);
	}
}

// 生成MySQLData.h和MySQLData.cpp文件
void CodeGenerator::generateCppMySQLDataFile(const MySQLInfo& mysqlInfo, string filePath)
{
	// 头文件
	string headerFileContent;
	string className = "MySQLData" + mysqlInfo.mSQLiteName;
	string headerMacro = "_MYSQL_DATA" + nameToUpper(mysqlInfo.mSQLiteName) + "_H_";
	headerFileContent += "#ifndef " + headerMacro + "\r\n";
	headerFileContent += "#define " + headerMacro + "\r\n";
	headerFileContent += "\r\n";
	headerFileContent += "#include \"MySQLData.h\"\r\n";
	headerFileContent += "\r\n";
	headerFileContent += "class MySQLTable;\r\n";
	headerFileContent += "class " + className + " : public MySQLData\r\n";
	headerFileContent += "{\r\n";
	headerFileContent += "public:\r\n";
	uint memberCount = mysqlInfo.mMemberList.size();
	FOR_I(memberCount)
	{
		headerFileContent += "\tCOL(" + mysqlInfo.mMemberList[i].mTypeName + ", " + mysqlInfo.mMemberList[i].mMemberName + ");\r\n";
	}
	headerFileContent += "public:\r\n";
	headerFileContent += "\tstatic void fillColName(MySQLTable* table);\r\n";
	headerFileContent += "\tvoid resultRowToTableData(myMap<const char*, char*>& resultRow) override;\r\n";
	headerFileContent += "\tvoid paramList(char* params, uint size) override;\r\n";
	headerFileContent += "\tvoid resetProperty() override;\r\n";
	headerFileContent += "};\r\n";
	headerFileContent += "\r\n";
	headerFileContent += "#endif";

	// 源文件
	string sourceFileContent;
	sourceFileContent += "#include \"" + className + ".h\"\r\n";
	sourceFileContent += "#include \"Utility.h\"\r\n";
	sourceFileContent += "#include \"MySQLTable.h\"\r\n";
	sourceFileContent += "\r\n";
	// 字段静态变量定义
	FOR_I(memberCount)
	{
		sourceFileContent += "COL_DEFINE(" + className + ", " + mysqlInfo.mMemberList[i].mMemberName + ");\r\n";
	}
	// fillColName函数
	sourceFileContent += "\r\n";
	sourceFileContent += "void " + className + "::fillColName(MySQLTable* table)\r\n";
	sourceFileContent += "{\r\n";
	FOR_I(memberCount)
	{
		sourceFileContent += "\ttable->addColName(" + mysqlInfo.mMemberList[i].mMemberName + ");\r\n";
	}
	sourceFileContent += "}\r\n";
	sourceFileContent += "\r\n";
	// resultRowToTableData函数
	sourceFileContent += "void " + className + "::resultRowToTableData(myMap<const char*, char*>& resultRow)\r\n";
	sourceFileContent += "{\r\n";
	FOR_I(memberCount)
	{
		sourceFileContent += "\tPARSE(" + mysqlInfo.mMemberList[i].mMemberName + ");\r\n";
	}
	sourceFileContent += "}\r\n";
	sourceFileContent += "\r\n";
	// paramList函数
	sourceFileContent += "void " + className + "::paramList(char* params, uint size)\r\n";
	sourceFileContent += "{\r\n";
	FOR_I(memberCount)
	{
		if (i != memberCount - 1)
		{
			if (mysqlInfo.mMemberList[i].mTypeName == "string")
			{
				string isUTF8Str = mysqlInfo.mMemberList[i].mUTF8 ? "true" : "false";
				sourceFileContent += "\tAPPEND_STRING(" + mysqlInfo.mMemberList[i].mMemberName + ", " + isUTF8Str + ");\r\n";
			}
			else
			{
				sourceFileContent += "\tAPPEND_VALUE(" + mysqlInfo.mMemberList[i].mMemberName + ");\r\n";
			}
		}
		else
		{
			if (mysqlInfo.mMemberList[i].mTypeName == "string")
			{
				string isUTF8Str = mysqlInfo.mMemberList[i].mUTF8 ? "true" : "false";
				sourceFileContent += "\tAPPEND_STRING_END(" + mysqlInfo.mMemberList[i].mMemberName + ", " + isUTF8Str + ");\r\n";
			}
			else
			{
				sourceFileContent += "\tAPPEND_VALUE_END(" + mysqlInfo.mMemberList[i].mMemberName + ");\r\n";
			}
		}
	}
	sourceFileContent += "}\r\n";
	sourceFileContent += "\r\n";
	// resetProperty函数
	sourceFileContent += "void " + className + "::resetProperty()\r\n";
	sourceFileContent += "{\r\n";
	sourceFileContent += "\tMySQLData::resetProperty();\r\n";
	FOR_I(memberCount)
	{
		if (mysqlInfo.mMemberList[i].mTypeName == "string")
		{
			sourceFileContent += "\tm" + mysqlInfo.mMemberList[i].mMemberName + ".clear();\r\n";
		}
		else if (mysqlInfo.mMemberList[i].mTypeName == "float")
		{
			sourceFileContent += "\tm" + mysqlInfo.mMemberList[i].mMemberName + " = 0.0f;\r\n";
		}
		else
		{
			sourceFileContent += "\tm" + mysqlInfo.mMemberList[i].mMemberName + " = 0;\r\n";
		}
	}
	sourceFileContent += "}";
	validPath(filePath);
	headerFileContent = ANSIToUTF8(headerFileContent.c_str(), true);
	sourceFileContent = ANSIToUTF8(sourceFileContent.c_str(), true);
	writeFile(filePath + className + ".h", headerFileContent);
	writeFile(filePath + className + ".cpp", sourceFileContent);
}

// TDSQLite.h和TDSQLite.cpp文件
void CodeGenerator::generateCppSQLiteDataFile(const SQLiteInfo& sqliteInfo, string filePath)
{
	if (sqliteInfo.mOwner == SQLITE_OWNER::CLIENT_ONLY)
	{
		return;
	}
	string headerFileContent;
	string className = "TD" + sqliteInfo.mSQLiteName;
	string headerMacro = "_TD" + nameToUpper(sqliteInfo.mSQLiteName) + "_H_";
	headerFileContent += "#ifndef " + headerMacro + "\r\n";
	headerFileContent += "#define " + headerMacro + "\r\n"; 
	headerFileContent += "\r\n";
	headerFileContent += "#include \"SQLiteData.h\"\r\n";
	headerFileContent += "\r\n";
	headerFileContent += "class " + className + " : public SQLiteData\r\n";
	headerFileContent += "{\r\n";
	headerFileContent += "public:\r\n";
	uint memberCount = sqliteInfo.mMemberList.size();
	FOR_I(memberCount)
	{
		if (sqliteInfo.mMemberList[i].mOwner == SQLITE_OWNER::CLIENT_ONLY)
		{
			headerFileContent += "\tCOL_EMPTY(" + sqliteInfo.mMemberList[i].mTypeName + ", " + sqliteInfo.mMemberList[i].mMemberName + ");\r\n";
		}
		else
		{
			headerFileContent += "\tCOL(" + sqliteInfo.mMemberList[i].mTypeName + ", " + sqliteInfo.mMemberList[i].mMemberName + ");\r\n";
		}
	}
	headerFileContent += "public:\r\n";
	headerFileContent += "\t" + className + "()\r\n";
	headerFileContent += "\t{\r\n";
	FOR_I(memberCount)
	{
		if (sqliteInfo.mMemberList[i].mOwner == SQLITE_OWNER::CLIENT_ONLY)
		{
			headerFileContent += "\t\tREGISTE_PARAM_EMPTY(" + sqliteInfo.mMemberList[i].mMemberName + ");\r\n";
		}
		else
		{
			headerFileContent += "\t\tREGISTE_PARAM(" + sqliteInfo.mMemberList[i].mMemberName + ");\r\n";
		}
	}
	headerFileContent += "\t}\r\n";
	headerFileContent += "};\r\n";
	headerFileContent += "\r\n";
	headerFileContent += "#endif";

	string sourceFileContent;
	sourceFileContent += "#include \"" + className + ".h\"\r\n";
	sourceFileContent += "\r\n";
	FOR_I(memberCount)
	{
		sourceFileContent += "COL_DEFINE(" + className + ", " + sqliteInfo.mMemberList[i].mMemberName + ");";
		if (i != memberCount - 1)
		{
			sourceFileContent += "\r\n";
		}
	}
	validPath(filePath);
	headerFileContent = ANSIToUTF8(headerFileContent.c_str(), true);
	sourceFileContent = ANSIToUTF8(sourceFileContent.c_str(), true);
	writeFile(filePath + className + ".h", headerFileContent);
	writeFile(filePath + className + ".cpp", sourceFileContent);
}

// SQLiteHeader.h文件
void CodeGenerator::generateCppSQLiteTotalHeaderFile(const myVector<SQLiteInfo>& sqliteList, string filePath)
{
	string str0;
	str0 += "#ifndef _SQLITE_HEADER_H_\r\n";
	str0 += "#define _SQLITE_HEADER_H_\r\n";
	str0 += "\r\n";
	str0 += "#include \"SQLite.h\"\r\n";
	uint packetCount = sqliteList.size();
	FOR_I(packetCount)
	{
		if (sqliteList[i].mOwner != SQLITE_OWNER::CLIENT_ONLY)
		{
			str0 += "#include \"SQLite" + sqliteList[i].mSQLiteName + ".h\"\r\n";
		}
	}
	str0 += "\r\n";
	str0 += "#endif";
	validPath(filePath);
	str0 = ANSIToUTF8(str0.c_str(), true);
	writeFile(filePath + "SQLiteHeader.h", str0);
}

// PacketHeader.h和PacketDeclareHeader.h文件
void CodeGenerator::generateCppPacketTotalHeaderFile(const myVector<PacketInfo>& packetList, string filePath)
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
void CodeGenerator::generateCppPacketHeaderFile(const PacketInfo& packetInfo, string filePath)
{
	string headerMacro = "_" + packetNameToUpper(packetInfo.mPacketName) + "_DECLARE_H_";
	string fileString;
	fileString += "#ifndef " + headerMacro + "\r\n";
	fileString += "#define " + headerMacro + "\r\n";
	fileString += "\r\n";
	fileString += "#define " + packetInfo.mPacketName + "_Declare \\\r\n";
	fileString += "public:\\\r\n";
	// 注册成员变量
	uint memberCount = packetInfo.mMemberList.size();
	if (memberCount != 0)
	{
		fileString += "\tvirtual void fillParams()\\\r\n";
		fileString += "\t{\\\r\n";
		FOR_I(memberCount)
		{
			fileString += cppPushParamString(packetInfo.mMemberList[i]);
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
		fileString += cppMemberDeclareString(packetInfo.mMemberList[i]);
	}
	removeLast(fileString, '\\');
	fileString += "\r\n";
	fileString += "#endif";
	validPath(filePath);
	fileString = ANSIToUTF8(fileString.c_str(), true);
	writeFile(filePath + packetInfo.mPacketName + "_Declare.h", fileString);
}

// TDSQLite.cs文件
void CodeGenerator::generateCSharpSQLiteDataFile(const SQLiteInfo& sqliteInfo, string filePath)
{
	if (sqliteInfo.mOwner == SQLITE_OWNER::SERVER_ONLY)
	{
		return;
	}
	string fileContent;
	string className = "TD" + sqliteInfo.mSQLiteName;
	fileContent += "using Mono.Data.Sqlite;\r\n";
	fileContent += "using System;\r\n";
	fileContent += "using System.Collections.Generic;\r\n";
	fileContent += "using UnityEngine;\r\n";
	fileContent += "\r\n";
	fileContent += "public class " + className + " : TableData\r\n";
	fileContent += "{\r\n";
	uint memberCount = sqliteInfo.mMemberList.size();
	FOR_I(memberCount)
	{
		if (sqliteInfo.mMemberList[i].mMemberName == "ID")
		{
			continue;
		}
		fileContent += "\tpublic static string " + sqliteInfo.mMemberList[i].mMemberName + " = \"" + sqliteInfo.mMemberList[i].mMemberName + "\";\r\n";
	}
	FOR_I(memberCount)
	{
		if (sqliteInfo.mMemberList[i].mMemberName == "ID")
		{
			continue;
		}
		string typeName = sqliteInfo.mMemberList[i].mTypeName;
		// 将myVector替换为List,Vector2UShort替换为Vector2Int
		if (startWith(typeName, "myVector"))
		{
			strReplaceAll(typeName, "myVector", "List");
		}
		else if (typeName == "Vector2UShort")
		{
			typeName = "Vector2Int";
		}
		if (findString(typeName.c_str(), "List", NULL))
		{
			fileContent += "\tpublic " + typeName + " m" + sqliteInfo.mMemberList[i].mMemberName + " = new " + typeName + "();\r\n";
		}
		else
		{
			fileContent += "\tpublic " + typeName + " m" + sqliteInfo.mMemberList[i].mMemberName + ";\r\n";
		}
	}
	fileContent += "\tpublic override void parse(SqliteDataReader reader)\r\n";
	fileContent += "\t{\r\n";
	fileContent += "\t\tbase.parse(reader);\r\n";
	FOR_I(memberCount)
	{
		if (sqliteInfo.mMemberList[i].mMemberName == "ID")
		{
			continue;
		}
		fileContent += "\t\tparseParam(reader, ref m" + sqliteInfo.mMemberList[i].mMemberName + ", " + sqliteInfo.mMemberList[i].mMemberName + ");\r\n";
	}
	fileContent += "\t}\r\n";
	fileContent += "\tpublic static void link(SQLiteTable table)\r\n";
	fileContent += "\t{\r\n";
	FOR_I(memberCount)
	{
		if (sqliteInfo.mMemberList[i].mLinkTable.length() > 0)
		{
			fileContent += "\t\ttable.link(" + sqliteInfo.mMemberList[i].mMemberName + ", mSQLite" + sqliteInfo.mMemberList[i].mLinkTable + ");\r\n";
		}
	}
	fileContent += "\t}\r\n";
	fileContent += "}";
	validPath(filePath);
	fileContent = ANSIToUTF8(fileContent.c_str(), true);
	writeFile(filePath + className + ".cs", fileContent);
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
void CodeGenerator::generateCSharpFile(const PacketInfo& packetInfo, string filePath)
{
	const int prefixLength = 2;
	if (packetInfo.mPacketName.substr(0, prefixLength) != "CS" && packetInfo.mPacketName.substr(0, prefixLength) != "SC")
	{
		ERROR("包名前缀错误");
		return;
	}
	string fileString = "using System;\r\n";
	fileString += "using System.Collections;\r\n";
	fileString += "using System.Collections.Generic;\r\n";
	fileString += "\r\n";
	fileString += "public partial class " + packetInfo.mPacketName + " : SocketPacket\r\n";
	fileString += "{\r\n";
	uint memberCount = packetInfo.mMemberList.size();
	FOR_I(memberCount)
	{
		fileString += cSharpMemberDeclareString(packetInfo.mMemberList[i]);
	}
	if (memberCount > 0)
	{
		fileString += "\tprotected override void fillParams()\r\n";
		fileString += "\t{\r\n";
		FOR_I(memberCount)
		{
			fileString += cSharpPushParamString(packetInfo.mMemberList[i]);
		}
		fileString += "\t}\r\n";
	}
	fileString += "}";
	validPath(filePath);
	fileString = ANSIToUTF8(fileString.c_str(), true);
	writeFile(filePath + packetInfo.mPacketName + "_Declare.cs", fileString);
}

MySQLMember CodeGenerator::parseMySQLMemberLine(string line)
{
	MySQLMember memberInfo;
	// 字段类型和字段名
	myVector<string> memberStrList;
	split(line.c_str(), " ", memberStrList);
	if (findString(memberStrList[0].c_str(), "(utf8)", NULL))
	{
		strReplaceAll(memberStrList[0], "(utf8)", "");
		memberInfo.mUTF8 = true;
	}
	else
	{
		memberInfo.mUTF8 = false;
	}
	memberInfo.mTypeName = memberStrList[0];
	memberInfo.mMemberName = memberStrList[1];
	return memberInfo;
}

SQLiteMember CodeGenerator::parseSQLiteMemberLine(string line)
{
	SQLiteMember memberInfo;
	// 该字段属于客户端还是服务器
	int rectStartIndex = line.find_first_of('[');
	int rectEndIndex = line.find_first_of(']', rectStartIndex);
	if (rectStartIndex >= 0 && rectEndIndex >= 0)
	{
		string owner = line.substr(rectStartIndex, rectEndIndex - rectStartIndex + 1);
		if (owner == "[Client]")
		{
			memberInfo.mOwner = SQLITE_OWNER::CLIENT_ONLY;
		}
		else if (owner == "[Server]")
		{
			memberInfo.mOwner = SQLITE_OWNER::SERVER_ONLY;
		}
		else
		{
			memberInfo.mOwner = SQLITE_OWNER::BOTH;
		}
		line.erase(rectStartIndex, rectEndIndex - rectStartIndex + 1);
	}
	else
	{
		memberInfo.mOwner = SQLITE_OWNER::BOTH;
	}
	// 该字段索引的表格
	int roundStartIndex = line.find_first_of('(');
	int roundEndIndex = line.find_first_of(')');
	if (roundStartIndex >= 0 && roundEndIndex >= 0)
	{
		memberInfo.mLinkTable = line.substr(roundStartIndex + 1, roundEndIndex - roundStartIndex - 1);
		line.erase(roundStartIndex, roundEndIndex - roundStartIndex + 1);
	}
	// 字段类型和字段名
	myVector<string> memberStrList;
	split(line.c_str(), " ", memberStrList);
	memberInfo.mTypeName = memberStrList[0];
	memberInfo.mMemberName = memberStrList[1];
	return memberInfo;
}

PacketMember CodeGenerator::parseMemberLine(const string& line)
{
	PacketMember memberInfo;
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
			return PacketMember();
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

string CodeGenerator::nameToUpper(const string& sqliteName)
{
	// 根据大写字母拆分
	myVector<string> macroList;
	int length = sqliteName.length();
	int lastIndex = 0;
	// 从1开始,因为第0个始终都是大写,会截取出空字符串
	for (int i = 1; i < length; ++i)
	{
		// 已大写字母为分隔符
		if (sqliteName[i] >= 'A' && sqliteName[i] <= 'Z')
		{
			macroList.push_back(sqliteName.substr(lastIndex, i - lastIndex));
			lastIndex = i;
		}
	}
	macroList.push_back(sqliteName.substr(lastIndex, length - lastIndex));
	string headerMacro;
	FOR_VECTOR_CONST(macroList)
	{
		headerMacro += "_" + toUpper(macroList[i]);
	}
	return headerMacro;
}

string CodeGenerator::cppPushParamString(const PacketMember& memberInfo)
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

string CodeGenerator::cppMemberDeclareString(const PacketMember& memberInfo)
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

string CodeGenerator::cSharpPushParamString(const PacketMember& memberInfo)
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

string CodeGenerator::cSharpMemberDeclareString(const PacketMember& memberInfo)
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