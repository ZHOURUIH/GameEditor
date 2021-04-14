#ifndef _TD_SCENE_MAP_PEACE_AREA_H_
#define _TD_SCENE_MAP_PEACE_AREA_H_

#include "SQLiteData.h"

class TDSceneMapPeaceArea : public SQLiteData
{
public:
	static const char* ID;
	static const char* MapID;
	static const char* MapName;
	static const char* Position;
	static const char* Size;
public:
	int mID;
	ushort mMapID;
	string mMapName;
	Vector2UShort mPosition;
	Vector2UShort mSize;
public:
	TDSceneMapPeaceArea()
	{
		registeParam(mID, ID);
		registeParam(mMapID, MapID);
		registeParam(mMapName, MapName);
		registeParam(mPosition, Position);
		registeParam(mSize, Size);
	}
};

#endif