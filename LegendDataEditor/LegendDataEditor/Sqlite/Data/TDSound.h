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
	string mFileName;
	string mDesc;
	float mVolumeScale;
public:
	TDSound()
	{
		registeParam(mID, ID);
		registeParam(mFileName, FileName);
		registeParam(mDesc, Desc);
		registeParam(mVolumeScale, VolumeScale);
	}
};

#endif