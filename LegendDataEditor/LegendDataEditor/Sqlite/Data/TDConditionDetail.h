#ifndef _TD_CONDITION_DETAIL_H_
#define _TD_CONDITION_DETAIL_H_

#include "SQLiteData.h"

class TDConditionDetail : public SQLiteData
{
public:
	static const char* ID;
	static const char* Name;
	static const char* Type;
	static const char* Description;
	static const char* Param0;
	static const char* ParamDesc0;
	static const char* Param1;
	static const char* ParamDesc1;
	static const char* Param2;
	static const char* ParamDesc2;
	static const char* Param3;
	static const char* ParamDesc3;
public:
	int mID;
	byte mType;
	string mParam0;
	string mParam1;
	string mParam2;
	string mParam3;
public:
	TDConditionDetail()
	{
		registeParam(mID, ID);
		registeParamEmpty(Name);
		registeParam(mType, Type);
		registeParamEmpty(Description);
		registeParam(mParam0, Param0);
		registeParamEmpty(ParamDesc0);
		registeParam(mParam1, Param1);
		registeParamEmpty(ParamDesc1);
		registeParam(mParam2, Param2);
		registeParamEmpty(ParamDesc2);
		registeParam(mParam3, Param3);
		registeParamEmpty(ParamDesc3);
	}
};

#endif