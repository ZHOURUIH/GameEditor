#include "CodeRank.h"

void CodeRank::generate()
{
	string cppHeaderPath = cppGamePath + "RankSystem/";

	myVector<string> list = findTargetHeaderFile(cppGamePath, [](const string& fileName) { return startWith(fileName, "Rank"); },
		[](const string& line) { return findSubstr(line, " : public Rank"); });
	generateRegister(list, cppHeaderPath);
}

void CodeRank::generateRegister(const myVector<string>& list, const string& headerPath)
{
	// Ô´ÎÄ¼þ
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