#ifndef _TD_SCENE_MAP_H_
#define _TD_SCENE_MAP_H_

#include "SQLiteData.h"

class TDSceneMap : public SQLiteData
{
public:
	static const char* ID;
	static const char* Label;
	static const char* MapFile;
	static const char* MainCity;
public:
	int mID;
	ushort mMapFile;
	ushort mMainCity;
public:
	TDSceneMap()
	{
		registeParam(mID, ID);
		registeParamEmpty(Label);
		registeParam(mMapFile, MapFile);
		registeParam(mMainCity, MainCity);
	}
};

#endif