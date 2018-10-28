#ifndef _GAME_CONFIG_H_
#define _GAME_CONFIG_H_

#include "ToolCoreBase.h"
#include "CommonDefine.h"

enum GAME_DEFINE_FLOAT
{
	GDF_OUTPUT_PACKET_LOG,
	GDF_MAX,
};

enum GAME_DEFINE_STRING
{
	GDS_MAX,
};

class GameConfig : public ToolCoreBase
{
public:
	GameConfig();
	virtual ~GameConfig(){ destory(); }
	void init();
	void readConfig(const std::string& fileName, bool floatParam);
	void destory()
	{
		mFloatParamDefineList.clear();
		mStringParamDefineList.clear();
		mFloatParamList.clear();
		mStringParamList.clear();
	}
	static float getFloatParam(GAME_DEFINE_FLOAT param)
	{
		auto iterParam = mFloatParamList.find(param);
		if (iterParam != mFloatParamList.end())
		{
			return iterParam->second;
		}
		return 0.0f;
	}
	static const std::string& getStringParam(GAME_DEFINE_STRING param)
	{
		auto iterParam = mStringParamList.find(param);
		if (iterParam != mStringParamList.end())
		{
			return iterParam->second;
		}
		return EMPTY_STRING;
	}
protected:
	void setParam(GAME_DEFINE_FLOAT param, float value)
	{
		auto iterParam = mFloatParamList.find(param);
		if (iterParam == mFloatParamList.end())
		{
			mFloatParamList.insert(param, value);
		}
		else
		{
			iterParam->second = value;
		}
	}
	void setParam(GAME_DEFINE_STRING param, const std::string& value)
	{
		auto iterParam = mStringParamList.find(param);
		if (iterParam == mStringParamList.end())
		{
			mStringParamList.insert(param, value);
		}
		else
		{
			iterParam->second = value;
		}
	}
protected:
	static txMap<std::string, GAME_DEFINE_FLOAT> mFloatParamDefineList;
	static txMap<std::string, GAME_DEFINE_STRING> mStringParamDefineList;
	static txMap<GAME_DEFINE_FLOAT, float> mFloatParamList;
	static txMap<GAME_DEFINE_STRING, std::string> mStringParamList;
};

#endif