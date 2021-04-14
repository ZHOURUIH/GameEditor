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
	string mName;
	byte mType;
	string mDescription;
	string mParam0;
	string mParamDesc0;
	string mParam1;
	string mParamDesc1;
	string mParam2;
	string mParamDesc2;
	string mParam3;
	string mParamDesc3;
public:
	TDConditionDetail()
	{
		registeParam(mID, ID);
		registeParam(mName, Name);
		registeParam(mType, Type);
		registeParam(mDescription, Description);
		registeParam(mParam0, Param0);
		registeParam(mParamDesc0, ParamDesc0);
		registeParam(mParam1, Param1);
		registeParam(mParamDesc1, ParamDesc1);
		registeParam(mParam2, Param2);
		registeParam(mParamDesc2, ParamDesc2);
		registeParam(mParam3, Param3);
		registeParam(mParamDesc3, ParamDesc3);
	}
};

#endif