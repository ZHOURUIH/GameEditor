#include <stdlib.h>
#include <vector>
#include <map>

#include "Utility.h"
#include "GameConfig.h"
#include "CommonDefine.h"
#include "txMemeryTrace.h"

std::map<std::string, GAME_DEFINE_FLOAT> GameConfig::mFloatParamDefineList;
std::map<GAME_DEFINE_FLOAT, float> GameConfig::mFloatParamList;
std::map<std::string, GAME_DEFINE_STRING> GameConfig::mStringParamDefineList;
std::map<GAME_DEFINE_STRING, std::string> GameConfig::mStringParamList;

#define GAME_STEEING_FLOAT(t) mFloatParamDefineList.insert(std::pair<std::string, GAME_DEFINE_FLOAT>(TOSTRING(t), t));
#define GAME_STEEING_STRING(t) mStringParamDefineList.insert(std::pair<std::string, GAME_DEFINE_STRING>(TOSTRING(t), t));

GameConfig::GameConfig()
{
	if (mFloatParamDefineList.size() < GDF_MAX)
	{
		std::cout << "error : not all float params add to list! max count : " << GDF_MAX << ", added count : " << (int)mFloatParamDefineList.size();
	}

	GAME_STEEING_STRING(GDS_EXCEPT_FILE);
	GAME_STEEING_STRING(GDS_COPY_FILE_DIR);
	GAME_STEEING_STRING(GDS_COPY_FILE_DEST_DIR);
	if (mStringParamDefineList.size() < GDS_MAX)
	{
		std::cout << "error : not all string params add to list! max count : " << GDS_MAX << ", added count : " << (int)mStringParamDefineList.size();
	}
}

#undef GAME_STEEING_FLOAT
#undef GAME_STEEING_STRING

GameConfig::~GameConfig()
{
	destory();
}

void GameConfig::readConfig(const std::string& fileName, const bool& floatParam)
{
	std::string dataString = Utility::openTxtFile(fileName);
	// 将文件的每一行处理后放入lineList中
	std::vector<std::string> lineList;
	while (true)
	{
		int returnPos = dataString.find_first_of('\r');
		if (returnPos == -1)
		{
			std::string lineString = dataString;
			if (lineString.length() != 0)
			{
				lineList.push_back(lineString);
			}
			break;
		}
		std::string lineString = dataString.substr(0, returnPos);
		if (lineString.length() != 0)
		{
			lineList.push_back(lineString);
		}
		dataString = dataString.substr(returnPos + strlen("\r\n"), dataString.length() - returnPos - strlen("\r\n"));
	}

	// 将数据字符串拆分出来,放入valueList中
	std::map<std::string, std::string> valueList;
	int lineCount = lineList.size();
	for (int i = 0; i < lineCount; ++i)
	{
		std::string lineString = lineList[i];
		// 首先去掉所有的空格和制表符
		char* newStringBuffer = TRACE_NEW_ARRAY(char, lineString.length() + 1, newStringBuffer);
		memset(newStringBuffer, 0, lineString.length() + 1);
		int curLen = 0;
		int strLength = lineString.length();
		for (int j = 0; j < strLength; ++j)
		{
			if (lineString[j] != ' ' && lineString[j] != '\t')
			{
				newStringBuffer[curLen] = lineString[j];
				++curLen;
			}
		}
		std::string newString(newStringBuffer);
		TRACE_DELETE_ARRAY(newStringBuffer);
		// 如果该行是空的,或者是注释,则不进行处理
		if (newString.length() > 0 && newString.substr(0, 2) != std::string("//"))
		{
			std::vector<std::string> valueVector;
			Utility::split(newString, "=", &valueVector);
			if (valueVector.size() == 2)
			{
				valueList.insert(std::pair<std::string, std::string>(valueVector[0], valueVector[1]));
			}
		}
	}

	// 解析valueList中的数据字符串
	auto iterValue = valueList.begin();
	auto iterValueEnd = valueList.end();
	for (; iterValue != iterValueEnd; ++iterValue)
	{
		if (floatParam)
		{
			auto iterDefine = mFloatParamDefineList.find(iterValue->first);
			if (iterDefine != mFloatParamDefineList.end())
			{
				setFloatParam(iterDefine->second, (float)atof(iterValue->second.c_str()));
			}
		}
		else
		{
			auto iterDefine = mStringParamDefineList.find(iterValue->first);
			if (iterDefine != mStringParamDefineList.end())
			{
				setStringParam(iterDefine->second, iterValue->second.c_str());
			}
		}
	}
}

void GameConfig::setFloatParam(const GAME_DEFINE_FLOAT& param, const float& value)
{
	auto iterParam = mFloatParamList.find(param);
	if (iterParam == mFloatParamList.end())
	{
		mFloatParamList.insert(std::make_pair(param, value));
	}
	else
	{
		iterParam->second = value;
	}
}

void GameConfig::setStringParam(const GAME_DEFINE_STRING& param, const std::string& value)
{
	auto iterParam = mStringParamList.find(param);
	if (iterParam == mStringParamList.end())
	{
		mStringParamList.insert(std::make_pair(param, value));
	}
	else
	{
		iterParam->second = value;
	}
}