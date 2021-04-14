#ifndef _TD_TRIGGER_MODIFIER_H_
#define _TD_TRIGGER_MODIFIER_H_

#include "SQLiteData.h"

class TDTriggerModifier : public SQLiteData
{
public:
	static const char* ID;
	static const char* Type;
	static const char* Description;
	static const char* Param0;
	static const char* ParamDesc0;
	static const char* Param1;
	static const char* ParamDesc1;
	static const char* Param2;
	static const char* ParamDesc2;
public:
	int mID;
	int mType;
	string mParam0;
	string mParam1;
	string mParam2;
public:
	TDTriggerModifier()
	{
		registeParam(mID, ID);
		registeParam(mType, Type);
		registeParamEmpty(Description);
		registeParam(mParam0, Param0);
		registeParamEmpty(ParamDesc0);
		registeParam(mParam1, Param1);
		registeParamEmpty(ParamDesc1);
		registeParam(mParam2, Param2);
		registeParamEmpty(ParamDesc2);
	}
};

#endif