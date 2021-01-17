#include "CodeAchivement.h"

void CodeAchivement::generateAchivementCode(string headerPath, string achivementPath)
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

	FOR_VECTOR(achivementList)
	{
		generateAchivementFile(achivementList[i], achivementPath);
	}
	END(achivementList);
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
	line(source, "};", false);

	source = ANSIToUTF8(source.c_str(), true);
	writeFile(headerPath + "AchivementRegister.cpp", source);
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
		line(header, "\t\tClassPooledObject::resetProperty();");
		line(header, "\t\tmTargetValue = 0;");
		line(header, "\t}");
		line(header, "protected:");
		line(header, "\tstatic void (GameEvent* event, IEventListener* listener);");
		line(header, "protected:");
		line(header, "\tint mTargetValue;");
		line(header, "};");
		line(header, "");
		line(header, "#endif", false);

		header = ANSIToUTF8(header.c_str(), true);
		writeFile(headerFullPath, header);
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

		source = ANSIToUTF8(source.c_str(), true);
		writeFile(cppFullPath, source);
	}
}