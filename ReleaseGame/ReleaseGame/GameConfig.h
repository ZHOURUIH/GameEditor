#ifndef _GAME_CONFIG_H_
#define _GAME_CONFIG_H_

#include <string>
#include <map>
#include <iostream>

#include "CommonDefine.h"

class GameConfig
{
public:
	GameConfig();
	virtual ~GameConfig();

	void init()
	{
		readConfig(CONFIG_PATH + "GameStringConfig.txt", false);
	}

	void readConfig(std::string fileName, bool floatParam);

	void destory()
	{
		mFloatParamList.clear(); 
		mStringParamList.clear();
		mFloatParamDefineList.clear();
		mStringParamDefineList.clear();
	}

	static float getFloatParam(GAME_DEFINE_FLOAT param)
	{
		std::map<GAME_DEFINE_FLOAT, float>::iterator iterParam = mFloatParamList.find(param);
		if (iterParam != mFloatParamList.end())
		{
			return iterParam->second;
		}
		std::cout << "can not find float game define : " <<param;
		return 0.0f;
	}

	static std::string getStringParam(GAME_DEFINE_STRING param)
	{
		std::map<GAME_DEFINE_STRING, std::string>::iterator iterParam = mStringParamList.find(param);
		if (iterParam != mStringParamList.end())
		{
			return iterParam->second;
		}
		std::cout << "can not find string game define : " << param;
		return "";
	}
protected:
	void setFloatParam(GAME_DEFINE_FLOAT param, float value);
	void setStringParam(GAME_DEFINE_STRING param, std::string value);
public:
	static std::map<std::string, GAME_DEFINE_FLOAT> mFloatParamDefineList;
	static std::map<GAME_DEFINE_FLOAT, float> mFloatParamList;
	static std::map<std::string, GAME_DEFINE_STRING> mStringParamDefineList;
	static std::map<GAME_DEFINE_STRING, std::string> mStringParamList;
	std::string mErrorInfo;
};

#endif