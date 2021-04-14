#ifndef _TD_SCENE_MAP_SUB_REGION_H_
#define _TD_SCENE_MAP_SUB_REGION_H_

#include "SQLiteData.h"

class TDSceneMapSubRegion : public SQLiteData
{
public:
	static const char* ID;
	static const char* Name;
	static const char* MapID;
	static const char* Position;
	static const char* Size;
public:
	int mID;
	string mName;
	ushort mMapID;
	Vector2UShort mPosition;
	Vector2UShort mSize;
public:
	TDSceneMapSubRegion()
	{
		registeParam(mID, ID);
		registeParam(mName, Name);
		registeParam(mMapID, MapID);
		registeParam(mPosition, Position);
		registeParam(mSize, Size);
	}
};

#endif