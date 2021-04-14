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
	string mName;
	string mDescription;
	float mBuffTime;
	string mParam0;
	string mParamDesc0;
	string mParam1;
	string mParamDesc1;
	string mParam2;
	string mParamDesc2;
	string mParam3;
	string mParamDesc3;
	string mParam4;
	string mParamDesc4;
public:
	TDBuffDetail()
	{
		registeParam(mID, ID);
		registeParam(mBuffID, BuffID);
		registeParam(mName, Name);
		registeParam(mDescription, Description);
		registeParam(mBuffTime, BuffTime);
		registeParam(mParam0, Param0);
		registeParam(mParamDesc0, ParamDesc0);
		registeParam(mParam1, Param1);
		registeParam(mParamDesc1, ParamDesc1);
		registeParam(mParam2, Param2);
		registeParam(mParamDesc2, ParamDesc2);
		registeParam(mParam3, Param3);
		registeParam(mParamDesc3, ParamDesc3);
		registeParam(mParam4, Param4);
		registeParam(mParamDesc4, ParamDesc4);
	}
};

#endif