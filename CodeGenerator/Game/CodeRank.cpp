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
	generateRegister(lineList, cppHeaderPath);
}

void CodeRank::generateRegister(const myVector<string>& list, const string& headerPath)
{
	// 源文件
	string source;
	line(source, "#include \"GameHeader.h\"");
	line(source, "");
	line(source, "void RankRegister::registeAll()");
	line(source, "{");
	FOR_VECTOR_CONST(list)
	{
		string type = nameToUpper(list[i]).substr(strlen("_RANK_"));
		line(source, "\tGameBase::mRankFactoryManager->addFactory<" + list[i] + ">(RANK_TYPE::" + type + ");");
	}
	line(source, "}", false);

	writeFile(headerPath + "RankRegister.cpp", ANSIToUTF8(source.c_str(), true));
}