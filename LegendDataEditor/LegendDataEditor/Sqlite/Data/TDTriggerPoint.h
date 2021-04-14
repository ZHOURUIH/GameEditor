#ifndef _TD_TRIGGER_POINT_H_
#define _TD_TRIGGER_POINT_H_

#include "SQLiteData.h"

class TDTriggerPoint : public SQLiteData
{
public:
	static const char* ID;
	static const char* MapID;
	static const char* MapName;
	static const char* TilePosition;
	static const char* Event;
public:
	int mID;
	int mMapID;
	Vector2UShort mTilePosition;
	byte mEvent;
public:
	TDTriggerPoint()
	{
		registeParam(mID, ID);
		registeParam(mMapID, MapID);
		registeParamEmpty(MapName);
		registeParam(mTilePosition, TilePosition);
		registeParam(mEvent, Event);
	}
};

#endif