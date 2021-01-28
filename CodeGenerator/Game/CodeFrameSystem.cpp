#include "CodeFrameSystem.h"

const string CodeFrameSystem::cppHeaderPath = cppGamePath + "Common/";

void CodeFrameSystem::generate()
{
	string frameSystemFile;
	openTxtFile("FrameSystem.txt", frameSystemFile);
	if (frameSystemFile.length() == 0)
	{
		ERROR("未找文件FrameSystem.txt");
		return;
	}
	frameSystemFile = UTF8ToANSI(frameSystemFile.c_str(), true);
	myVector<string> lineList;
	split(frameSystemFile.c_str(), "\r\n", lineList);
	myVector<string> frameSystemList;
	myVector<string> factoryManagerList;
	myVector<string> classPoolList;
	int typeIndex = -1;
	FOR_VECTOR(lineList)
	{
		if (lineList[i] == "{")
		{
			if (lineList[i - 1] == "FrameSystem")
			{
				typeIndex = 0;
			}
			else if (lineList[i - 1] == "FactoryManager")
			{
				typeIndex = 1;
			}
			else if (lineList[i - 1] == "ClassPool")
			{
				typeIndex = 2;
			}
			continue;
		}
		if (lineList[i] == "}")
		{
			typeIndex = -1;
			continue;
		}
		removeAll(lineList[i], '\t');
		if (typeIndex == 0)
		{
			frameSystemList.push_back(lineList[i]);
		}
		else if (typeIndex == 1)
		{
			factoryManagerList.push_back(lineList[i]);
		}
		else if (typeIndex == 2)
		{
			classPoolList.push_back(lineList[i]);
		}
	}
	END(lineList);
	generateHeaderFile(frameSystemList, factoryManagerList, classPoolList, cppHeaderPath);
	generateSystemDefineFile(frameSystemList, factoryManagerList, classPoolList, cppHeaderPath);
}

// FrameSystemHeader.h
void CodeFrameSystem::generateHeaderFile(const myVector<string>& frameSystemList,
										const myVector<string>& factoryManagerList, 
										const myVector<string>& classPoolList, 
										string filePath)
{
	string str0;
	line(str0, "#ifndef _FRAME_SYSTEM_HEADER_H_");
	line(str0, "#define _FRAME_SYSTEM_HEADER_H_");
	line(str0, "");
	line(str0, "// 常规系统组件");
	FOR_VECTOR_CONST(frameSystemList)
	{
		line(str0, "#include \"" + frameSystemList[i] + ".h\"");
	}
	line(str0, "// 工厂管理器");
	FOR_VECTOR_CONST(factoryManagerList)
	{
		line(str0, "#include \"" + factoryManagerList[i] + ".h\"");
	}
	line(str0, "// 对象池");
	FOR_VECTOR_CONST(classPoolList)
	{
		line(str0, "#include \"" + classPoolList[i] + ".h\"");
	}
	line(str0, "");
	line(str0, "#endif");
	str0 = ANSIToUTF8(str0.c_str(), true);
	writeFile(filePath + "FrameSystemHeader.h", str0);
}

// FrameSystemDefine.h
void CodeFrameSystem::generateSystemDefineFile(const myVector<string>& frameSystemList,
												const myVector<string>& factoryManagerList,
												const myVector<string>& classPoolList,
												string filePath)
{
	string str0;
	line(str0, "#define REGISTE_ALL_SYSTEM \\");
	FOR_VECTOR_CONST(frameSystemList)
	{
		line(str0, "REGISTE_SYSTEM(" + frameSystemList[i] + ");\\");
	}
	FOR_VECTOR_CONST(factoryManagerList)
	{
		line(str0, "REGISTE_SYSTEM(" + factoryManagerList[i] + ");\\");
	}
	FOR_VECTOR_CONST(classPoolList)
	{
		line(str0, "REGISTE_SYSTEM(" + classPoolList[i] + ");\\");
	}
	removeLast(str0, '\\');

	line(str0, "");
	line(str0, "#define DECLARE_ALL_SYSTEM \\");
	FOR_I(frameSystemListCount)
	{
		line(str0, "DECALRE_SYSTEM(" + frameSystemList[i] + ");\\");
	}
	FOR_I(factoryManagerListCount)
	{
		line(str0, "DECALRE_SYSTEM(" + factoryManagerList[i] + ");\\");
	}
	FOR_I(classPoolListCount)
	{
		line(str0, "DECALRE_SYSTEM(" + classPoolList[i] + ");\\");
	}
	removeLast(str0, '\\');

	line(str0, "");
	line(str0, "#define DEFINE_ALL_GAME_SYSTEM \\");
	FOR_I(frameSystemListCount)
	{
		line(str0, "DEFINE_SYSTEM_GAME(" + frameSystemList[i] + ");\\");
	}
	FOR_I(factoryManagerListCount)
	{
		line(str0, "DEFINE_SYSTEM_GAME(" + factoryManagerList[i] + ");\\");
	}
	FOR_I(classPoolListCount)
	{
		line(str0, "DEFINE_SYSTEM_GAME(" + classPoolList[i] + ");\\");
	}
	removeLast(str0, '\\');

	line(str0, "");
	line(str0, "#define GET_ALL_SYSTEM \\");
	FOR_I(frameSystemListCount)
	{
		line(str0, "GET_SYSTEM(" + frameSystemList[i] + ");\\");
	}
	FOR_I(factoryManagerListCount)
	{
		line(str0, "GET_SYSTEM(" + factoryManagerList[i] + ");\\");
	}
	FOR_I(classPoolListCount)
	{
		line(str0, "GET_SYSTEM(" + classPoolList[i] + ");\\");
	}
	removeLast(str0, '\\');

	str0 = ANSIToUTF8(str0.c_str(), true);
	writeFile(filePath + "FrameSystemDefine.h", str0);
}