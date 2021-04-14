#ifndef _TD_TIME_TASK_H_
#define _TD_TIME_TASK_H_

#include "SQLiteData.h"

class TDTimeTask : public SQLiteData
{
public:
	static const char* ID;
	static const char* Type;
	static const char* Param0;
	static const char* ParamDesc0;
	static const char* Param1;
	static const char* ParamDesc1;
	static const char* Param2;
	static const char* ParamDesc2;
	static const char* Param3;
	static const char* ParamDesc3;
	static const char* Param4;
	static const char* ParamDesc4;
	static const char* Param5;
	static const char* ParamDesc5;
	static const char* Param6;
	static const char* ParamDesc6;
	static const char* Param7;
	static const char* ParamDesc7;
public:
	int mID;
	int mType;
	float mParam0;
	float mParam1;
	float mParam2;
	float mParam3;
	float mParam4;
	float mParam5;
	float mParam6;
	float mParam7;
public:
	TDTimeTask()
	{
		registeParam(mID, ID);
		registeParam(mType, Type);
		registeParam(mParam0, Param0);
		registeParamEmpty(ParamDesc0);
		registeParam(mParam1, Param1);
		registeParamEmpty(ParamDesc1);
		registeParam(mParam2, Param2);
		registeParamEmpty(ParamDesc2);
		registeParam(mParam3, Param3);
		registeParamEmpty(ParamDesc3);
		registeParam(mParam4, Param4);
		registeParamEmpty(ParamDesc4);
		registeParam(mParam5, Param5);
		registeParamEmpty(ParamDesc5);
		registeParam(mParam6, Param6);
		registeParamEmpty(ParamDesc6);
		registeParam(mParam7, Param7);
		registeParamEmpty(ParamDesc7);
	}
};

#endif