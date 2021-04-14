#ifndef _TD_TRIGGER_EVENT_H_
#define _TD_TRIGGER_EVENT_H_

#include "SQLiteData.h"

class TDTriggerEvent : public SQLiteData
{
public:
	static const char* ID;
	static const char* Type;
	static const char* Description;
	static const char* CD;
	static const char* Modifier;
	static const char* Param0;
	static const char* ParamDesc0;
	static const char* Param1;
	static const char* ParamDesc1;
	static const char* Param2;
	static const char* ParamDesc2;
public:
	int mID;
	int mType;
	int mCD;
	int mModifier;
	string mParam0;
	string mParam1;
	string mParam2;
public:
	TDTriggerEvent()
	{
		registeParam(mID, ID);
		registeParam(mType, Type);
		registeParamEmpty(Description);
		registeParam(mCD, CD);
		registeParam(mModifier, Modifier);
		registeParam(mParam0, Param0);
		registeParamEmpty(ParamDesc0);
		registeParam(mParam1, Param1);
		registeParamEmpty(ParamDesc1);
		registeParam(mParam2, Param2);
		registeParamEmpty(ParamDesc2);
	}
};

#endif