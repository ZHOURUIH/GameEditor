#include "Utility.h"
#include "GameConfig.h"

txMap<std::string, GAME_DEFINE_FLOAT> GameConfig::mFloatParamDefineList;
txMap<std::string, GAME_DEFINE_STRING> GameConfig::mStringParamDefineList;
txMap<GAME_DEFINE_FLOAT, float> GameConfig::mFloatParamList;
txMap<GAME_DEFINE_STRING, std::string> GameConfig::mStringParamList;

#define ADD_FLOAT_PARAM(t) mFloatParamDefineList.insert(TOSTRING(t), t);
#define ADD_STRING_PARAM(t) mStringParamDefineList.insert(TOSTRING(t), t);

GameConfig::GameConfig()
{
	ADD_FLOAT_PARAM(GDF_OUTPUT_PACKET_LOG);
	if (mFloatParamDefineList.size() != GDF_MAX)
	{
		TOOL_CORE_ERROR("error : not all float parameters init! init count : %d, , max count : %d", (int)mFloatParamDefineList.size(), GDF_MAX);
	}

	if (mStringParamDefineList.size() != GDS_MAX)
	{
		TOOL_CORE_ERROR("error : not all string parameters init! init count : %d, , max count : %d", (int)mStringParamDefineList.size(), GDS_MAX);
	}
}

void GameConfig::init()
{
	readConfig("./Config/GameFloatConfig.txt", true); 
	readConfig("./Config/GameStringConfig.txt", false);
}

void GameConfig::readConfig(const std::string& fileName, bool floatParam)
{
	std::string dataString = FileUtility::openTxtFile(fileName);
	if (dataString == "")
	{
		return;
	}
#if RUN_PLATFORM == PLATFORM_WINDOWS
	char returnChar = '\r';
	const char* returnNextLine = "\r\n";
#elif RUN_PLATFORM == PLATFORM_LINUX
	char returnChar = '\n';
	const char* returnNextLine = "\n";
#endif
	// 将文件的每一行处理后放入lineList中
	txVector<std::string> lineList;
	while(true)
	{
		int returnPos = dataString.find_first_of(returnChar);
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
		dataString = dataString.substr(returnPos + strlen(returnNextLine), dataString.length() - returnPos - strlen(returnNextLine));
	}

	// 每行最多只允许2048个字节
	const int LINE_MAX_LENGTH = 2048;
	char* tempBuffer = TRACE_NEW_ARRAY(char, LINE_MAX_LENGTH, tempBuffer);
	// 将数据字符串拆分出来,放入valueList中
	txMap<std::string, std::string> valueList;
	int lineCount = lineList.size();
	FOR_STL(lineList, int i = 0; i < lineCount; ++i)
	{
		const std::string& lineString = lineList[i];
		// 首先去掉所有的空格和制表符
		int curLen = 0;
		int strLength = lineString.length();
		for (int j = 0; j < strLength; ++j)
		{
			if (lineString[j] != ' ' && lineString[j] != '\t')
			{
				if (curLen >= LINE_MAX_LENGTH)
				{
					break;
				}
				tempBuffer[curLen] = lineString[j];
				++curLen;
			}
		}
		tempBuffer[curLen] = 0;
		std::string newString(tempBuffer);
		// 如果该行是空的,或者是注释,则不进行处理
		if (newString.length() > 0 && !StringUtility::startWith(newString, "//"))
		{
			txVector<std::string> valueVector;
			StringUtility::split(newString, "=", valueVector);
			if (valueVector.size() == 2)
			{
				valueList.insert(valueVector[0], valueVector[1]);
			}
		}
	}
	END_FOR_STL(lineList);
	TRACE_DELETE_ARRAY(tempBuffer);
	// 解析valueList中的数据字符串
	auto iterValue = valueList.begin();
	auto iterValueEnd = valueList.end();
	FOR_STL(valueList, ; iterValue != iterValueEnd; ++iterValue)
	{
		if (floatParam)
		{
			auto iterDefine = mFloatParamDefineList.find(iterValue->first);
			if (iterDefine != mFloatParamDefineList.end())
			{
				setParam(iterDefine->second, (float)atof(iterValue->second.c_str()));
			}
		}
		else
		{
			auto iterDefine = mStringParamDefineList.find(iterValue->first);
			if (iterDefine != mStringParamDefineList.end())
			{
				setParam(iterDefine->second, iterValue->second.c_str());
			}
		}
	}
	END_FOR_STL(valueList);
}
