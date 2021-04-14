#ifndef _TD_SOUND_H_
#define _TD_SOUND_H_

#include "SQLiteData.h"

class TDSound : public SQLiteData
{
public:
	static const char* ID;
	static const char* FileName;
	static const char* Desc;
	static const char* VolumeScale;
public:
	int mID;
public:
	TDSound()
	{
		registeParam(mID, ID);
		registeParamEmpty(FileName);
		registeParamEmpty(Desc);
		registeParamEmpty(VolumeScale);
	}
};

#endif