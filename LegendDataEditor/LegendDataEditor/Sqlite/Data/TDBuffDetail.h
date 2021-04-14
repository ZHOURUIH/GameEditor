#ifndef _TD_BUFF_DETAIL_H_
#define _TD_BUFF_DETAIL_H_

#include "SQLiteData.h"

class TDBuffDetail : public SQLiteData
{
public:
	static const char* ID;
	static const char* BuffID;
	static const char* Name;
	static const char* Description;
	static const char* BuffTime;
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
public:
	int mID;
	ushort mBuffID;
	float mBuffTime;
	string mParam0;
	string mParam1;
	string mParam2;
	string mParam3;
	string mParam4;
public:
	TDBuffDetail()
	{
		registeParam(mID, ID);
		registeParam(mBuffID, BuffID);
		registeParamEmpty(Name);
		registeParamEmpty(Description);
		registeParam(mBuffTime, BuffTime);
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
	}
};

#endif