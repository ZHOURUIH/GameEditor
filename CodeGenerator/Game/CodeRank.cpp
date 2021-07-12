#include "CodeRank.h"

void CodeRank::generate()
{
	if (cppGamePath.length() == 0)
	{
		return;
	}

	string cppHeaderPath = cppGamePath + "RankSystem/";

	string file;
	openTxtFile("Rank.txt", file);
	if (file.length() == 0)
	{
		ERROR("未找文件Rank.txt");
		return;
	}
	myVector<string> lineList;
	split(file.c_str(), "\r\n", lineList);
	generateHeaderFile(lineList, cppHeaderPath);
	generateRegister(lineList, cppHeaderPath);
}

void CodeRank::generateHeaderFile(const myVector<string>& list, string headerPath)
{
	string str0;
	line(str0, "#ifndef _RANK_HEADER_H_");
	line(str0, "#define _RANK_HEADER_H_");
	line(str0, "");
	FOR_VECTOR_CONST(list)
	{
		line(str0, "#include \"" + list[i] + ".h\"");
	}
	line(str0, "");
	line(str0, "#endif", false);

	writeFile(headerPath + "RankHeader.h", ANSIToUTF8(str0.c_str(), true));
}

void CodeRank::generateRegister(const myVector<string>& list, string headerPath)
{
	// 源文件
	string source;
	line(source, "#include \"GameHeader.h\"");
	line(source, "");
	line(source, "#define REGISTE_RANK(classType, type) mRankFactoryManager->addFactory<classType>(type)");
	line(source, "");
	line(source, "void RankRegister::registeAll()");
	line(source, "{");
	FOR_VECTOR_CONST(list)
	{
		string type = nameToUpper(list[i]);
		type = type.substr(strlen("_RANK_"));
		line(source, "\tREGISTE_RANK(" + list[i] + ", RANK_TYPE::" + type + ");");
	}
	line(source, "}", false);

	writeFile(headerPath + "RankRegister.cpp", ANSIToUTF8(source.c_str(), true));
}