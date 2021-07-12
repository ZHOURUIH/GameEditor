#include "CodeAchivement.h"

void CodeAchivement::generate()
{
	if (cppGamePath.length() == 0)
	{
		return;
	}

	string cppHeaderPath = cppGamePath + "Achivement/";
	string cppAchivementFilePath = cppHeaderPath + "Achivement/";

	string file;
	openTxtFile("Achivement.txt", file);
	if (file.length() == 0)
	{
		ERROR("未找文件Achivement.txt");
		return;
	}

	myVector<string> lineList;
	split(file.c_str(), "\r\n", lineList);
	// 生成AchivementHeader.h文件
	generateHeaderFile(lineList, cppHeaderPath);
	// 生成AcvehimentRegister.cpp文件
	generateRegister(lineList, cppHeaderPath);

	FOR_VECTOR(lineList)
	{
		generateAchivementFile(lineList[i], cppAchivementFilePath);
	}
	END(lineList);
}

// AchivementHeader.h文件
void CodeAchivement::generateHeaderFile(const myVector<string>& list, string headerPath)
{
	string str0;
	line(str0, "#ifndef _ACHIVEMENT_HEADER_H_");
	line(str0, "#define _ACHIVEMENT_HEADER_H_");
	line(str0, "");
	FOR_VECTOR_CONST(list)
	{
		line(str0, "#include \"" + list[i] + ".h\"");
	}
	line(str0, "");
	line(str0, "#endif", false);

	writeFile(headerPath + "AchivementHeader.h", ANSIToUTF8(str0.c_str(), true));
}

// AcvehimentRegister.cpp
void CodeAchivement::generateRegister(const myVector<string>& list, string headerPath)
{
	// 源文件
	string source;
	line(source, "#include \"GameHeader.h\"");
	line(source, "");
	line(source, "#define ACHIVEMENT_FACTORY(classType, type) mAchivementFactoryManager->addFactory<classType>(type)");
	line(source, "");
	line(source, "void AchivementRegister::registeAll()");
	line(source, "{");
	FOR_VECTOR_CONST(list)
	{
		string type = nameToUpper(list[i]);
		type = type.substr(strlen("_ACHIVEMENT_"));
		line(source, "\tACHIVEMENT_FACTORY(" + list[i] + ", ACHIVEMENT_TYPE::" + type + ");");
	}
	line(source, "}", false);

	writeFile(headerPath + "AchivementRegister.cpp", ANSIToUTF8(source.c_str(), true));
}

// Achivement.h和Achivement.cpp
void CodeAchivement::generateAchivementFile(const string& achivement, string achivementPath)
{
	// Achivement.h
	string headerFullPath = achivementPath + achivement + ".h";
	if (!isFileExist(headerFullPath))
	{
		string header;
		string macro = nameToUpper(achivement) + "_H_";
		line(header, "#ifndef " + macro);
		line(header, "#define " + macro);
		line(header, "");
		line(header, "#include \"Achivement.h\"");
		line(header, "");
		line(header, "class GameEvent;");
		line(header, "class " + achivement + " : public Achivement");
		line(header, "{");
		line(header, "public:");
		line(header, "\t" + achivement + "()");
		line(header, "\t{");
		line(header, "\t\tmTargetValue = 0;");
		line(header, "\t}");
		line(header, "\tvoid init(TDAchivement* data) override;");
		line(header, "\tint getCurValue()const override;");
		line(header, "\tbool isFinish()const override;");
		line(header, "\tvoid resetProperty() override");
		line(header, "\t{");
		line(header, "\t\tbase::resetProperty();");
		line(header, "\t\tmTargetValue = 0;");
		line(header, "\t}");
		line(header, "protected:");
		line(header, "\tstatic void (GameEvent* event, IEventListener* listener);");
		line(header, "protected:");
		line(header, "\tint mTargetValue;");
		line(header, "};");
		line(header, "");
		line(header, "#endif", false);

		writeFile(headerFullPath, ANSIToUTF8(header.c_str(), true));
	}

	// Achivement.cpp
	string cppFullPath = achivementPath + achivement + ".cpp";
	if (!isFileExist(cppFullPath))
	{
		string source;
		line(source, "#include \"" + achivement + ".h\"");
		line(source, "#include \"SQLiteHeader.h\"");
		line(source, "#include \"EventSystem.h\"");
		line(source, "#include \"EventType.h\"");
		line(source, "#include \"CharacterPlayer.h\"");
		line(source, "#include \"CharacterPlayerData.h\"");
		line(source, "");
		line(source, "void " + achivement + "::init(TDAchivement* data)");
		line(source, "{");
		line(source, "\tAchivement::init(data);");
		line(source, "\tmTargetValue = data->mTargetValue;");
		line(source, "}");

		line(source, "void " + achivement + "::(GameEvent* event, IEventListener* listener)");
		line(source, "{");
		line(source, "\tauto thisEvent = CAST<*>(event);");
		line(source, "\tauto achivement = CAST<" + achivement + "*>(listener);");
		line(source, "\t// 如果成就已经完成,则不再处理");
		line(source, "\tif (achivement->isFinish())");
		line(source, "\t{");
		line(source, "\t\tachivement->removeListen();");
		line(source, "\t\treturn;");
		line(source, "\t}");
		line(source, "\tachivement->progressChanged();");
		line(source, "}");
		line(source, "");
		line(source, "int " + achivement + "::getCurValue() const");
		line(source, "{");
		line(source, "}");
		line(source, "");
		line(source, "bool " + achivement + "::isFinish() const");
		line(source, "{");
		line(source, "}", false);

		writeFile(cppFullPath, ANSIToUTF8(source.c_str(), true));
	}
}