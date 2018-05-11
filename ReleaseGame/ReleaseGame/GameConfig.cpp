#include <stdlib.h>
#include <vector>
#include <map>

#include "Utility.h"
#include "GameConfig.h"
#include "CommonDefine.h"
#include "txDataManager.h"

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
		std::cout << "error : not all float params add to list! max count : %d, added count : " << (int)mFloatParamDefineList.size();
		system("pause");
	}

	GAME_STEEING_STRING(GDS_COPY_RESOUCE_DIR);
	GAME_STEEING_STRING(GDS_COPY_DEST_DIR);
	GAME_STEEING_STRING(GDS_COPY_DEST_NAME);
	GAME_STEEING_STRING(GDS_COPY_RESOUCE_MEDIA_DIR);
	GAME_STEEING_STRING(GDS_COPY_DEST_MEDIA_DIR);
	GAME_STEEING_STRING(GDS_COPY_DEST_GAME_NAME);
	GAME_STEEING_STRING(GDS_EXCEPT_FILE);
	if (mStringParamDefineList.size() < GDS_MAX)
	{
		std::cout << "error : not all string params add to list! max count : %d, added count : " << (int)mStringParamDefineList.size();
		system("pause");
	}
}

#undef GAME_STEEING_FLOAT
#undef GAME_STEEING_STRING

GameConfig::~GameConfig()
{
	destory();
}

void GameConfig::readConfig(std::string fileName, bool floatParam)
{
	int fileIndex = txDataManager::getSingletonPtr()->LoadData(fileName.c_str(), true);
	if (fileIndex == -1)
	{
		return;
	}
	txDataElem* dataElem = txDataManager::getSingletonPtr()->GetData(fileIndex);
	char* data = dataElem->getValuePtr();

	// 将文件的每一行处理后放入lineList中
	std::vector<std::string> lineList;
	std::string dataString(data);
	while(true)
	{
		int returnPos = dataString.find_first_of('\r');
		if(returnPos == -1)
		{
			std::string lineString = dataString;
			if (lineString.length() != 0)
			{
				lineList.push_back(lineString);
			}
			break;
		}
		std::string lineString = dataString.substr(0, returnPos);
		if(lineString.length() != 0)
		{
			lineList.push_back(lineString);
		}
		dataString = dataString.substr(returnPos + strlen("\r\n"), dataString.length() - returnPos - strlen("\r\n"));
	}

	txDataManager::getSingletonPtr()->DestroyData(fileIndex);

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
			if(valueVector.size() == 2)
			{
				valueList.insert(std::pair<std::string, std::string>(valueVector[0], valueVector[1]));
			}
		}
	}

	// 解析valueList中的数据字符串
	std::map<std::string, std::string>::iterator iterValue = valueList.begin();
	std::map<std::string, std::string>::iterator iterValueEnd = valueList.end();
	for (; iterValue != iterValueEnd; ++iterValue)
	{
		if (floatParam)
		{
			std::map<std::string, GAME_DEFINE_FLOAT>::iterator iterDefine = mFloatParamDefineList.find(iterValue->first);
			if (iterDefine != mFloatParamDefineList.end())
			{
				setFloatParam(iterDefine->second, (float)atof(iterValue->second.c_str()));
			}
		}
		else
		{
			std::map<std::string, GAME_DEFINE_STRING>::iterator iterDefine = mStringParamDefineList.find(iterValue->first);
			if (iterDefine != mStringParamDefineList.end())
			{
				setStringParam(iterDefine->second, iterValue->second.c_str());
			}
		}
	}
}

void GameConfig::setFloatParam(GAME_DEFINE_FLOAT param, float value)
{
	std::map<GAME_DEFINE_FLOAT, float>::iterator iterParam = mFloatParamList.find(param);
	if (iterParam == mFloatParamList.end())
	{
		mFloatParamList.insert(std::make_pair(param, value));
	}
	else
	{
		iterParam->second = value;
	}
}

void GameConfig::setStringParam(GAME_DEFINE_STRING param, std::string value)
{
	std::map<GAME_DEFINE_STRING, std::string>::iterator iterParam = mStringParamList.find(param);
	if (iterParam == mStringParamList.end())
	{
		mStringParamList.insert(std::make_pair(param, value));
	}
	else
	{
		iterParam->second = value;
	}
}