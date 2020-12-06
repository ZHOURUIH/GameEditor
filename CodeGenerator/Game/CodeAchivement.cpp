#include "CodeAchivement.h"

void CodeAchivement::generateAchivementCode(string headerPath)
{
	string achviementFile;
	openTxtFile("Achivement.txt", achviementFile);
	if (achviementFile.length() == 0)
	{
		ERROR("未找文件Achivement.txt");
		return;
	}
	myVector<string> achivementList;
	split(achviementFile.c_str(), "\r\n", achivementList);
	// 生成AchivementHeader.h文件
	generateHeaderFile(achivementList, headerPath);
	// 生成AcvehimentRegister.cpp文件
	generateAchivementRegister(achivementList, headerPath);
}

// AchivementHeader.h文件
void CodeAchivement::generateHeaderFile(const myVector<string>& achivementList, string headerPath)
{
	string str0;
	line(str0, "#ifndef _ACHIVEMENT_HEADER_H_");
	line(str0, "#define _ACHIVEMENT_HEADER_H_");
	line(str0, "");
	uint count = achivementList.size();
	FOR_I(count)
	{
		line(str0, "#include \"" + achivementList[i] + ".h\"");
	}
	line(str0, "");
	line(str0, "#endif", false);
	validPath(headerPath);
	str0 = ANSIToUTF8(str0.c_str(), true);
	writeFile(headerPath + "AchivementHeader.h", str0);
}

// AcvehimentRegister.cpp
void CodeAchivement::generateAchivementRegister(const myVector<string>& achivementList, string headerPath)
{
	// 源文件
	string source;
	line(source, "#include \"AchivementRegister.h\"");
	line(source, "#include \"AchivementHeader.h\"");
	line(source, "#include \"AchivementFactoryManager.h\"");
	line(source, "");
	line(source, "#define ACHIVEMENT_FACTORY(classType, type) mAchivementFactoryManager->addFactory<classType>(ACHIVEMENT_TYPE::type);\n");
	line(source, "void AchivementRegister::registeAll()");
	line(source, "{");
	FOR_VECTOR_CONST(achivementList)
	{
		string type = nameToUpper(achivementList[i]);
		type = type.substr(strlen("_ACHIVEMENT_"));
		line(source, "\tACHIVEMENT_FACTORY(" + achivementList[i] + ", " + type + ");");
	}
	line(source, "};");
	validPath(headerPath);
	source = ANSIToUTF8(source.c_str(), true);
	writeFile(headerPath + "AchivementRegister.cpp", source);
}