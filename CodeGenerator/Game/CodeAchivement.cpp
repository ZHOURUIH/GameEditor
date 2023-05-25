#include "CodeAchivement.h"

void CodeAchivement::generate()
{
	generateAchivement(cppGamePath, cppGamePath + "Achivement/");
}

void CodeAchivement::generateAchivement(const string& path, const string& cppHeaderPath)
{
	myVector<string> files = findTargetHeaderFile(path,
		[](const string& fileName) { return startWith(fileName, "Achivement"); },
		[](const string& line) { return findSubstr(line, " : public Achivement"); });
	// 生成AcvehimentRegister.cpp文件
	generateRegister(files, cppHeaderPath);
}

// AcvehimentRegister.cpp
void CodeAchivement::generateRegister(const myVector<string>& list, const string& headerPath)
{
	// 源文件
	string source;
	line(source, "#include \"GameHeader.h\"");
	line(source, "");
	line(source, "void AchivementRegister::registeAll()");
	line(source, "{");
	FOR_VECTOR_CONST(list)
	{
		string type = nameToUpper(list[i]).substr(strlen("_ACHIVEMENT_"));
		line(source, "\tGameBase::mAchivementFactoryManager->addFactory<" + list[i] + ">(ACHIVEMENT_TYPE::" + type + ");");
	}
	line(source, "}", false);

	writeFile(headerPath + "AchivementRegister.cpp", ANSIToUTF8(source.c_str(), true));
}