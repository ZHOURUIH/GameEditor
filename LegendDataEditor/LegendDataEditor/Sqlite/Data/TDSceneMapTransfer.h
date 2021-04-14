#ifndef _TD_SCENE_MAP_TRANSFER_H_
#define _TD_SCENE_MAP_TRANSFER_H_

#include "SQLiteData.h"

class TDSceneMapTransfer : public SQLiteData
{
public:
	static const char* ID;
	static const char* SourceMap;
	static const char* SourceMapName;
	static const char* Position;
	static const char* TargetMap;
	static const char* TargetMapName;
	static const char* TargetPosition;
public:
	int mID;
	ushort mSourceMap;
	string mSourceMapName;
	Vector2UShort mPosition;
	ushort mTargetMap;
	string mTargetMapName;
	Vector2UShort mTargetPosition;
public:
	TDSceneMapTransfer()
	{
		registeParam(mID, ID);
		registeParam(mSourceMap, SourceMap);
		registeParam(mSourceMapName, SourceMapName);
		registeParam(mPosition, Position);
		registeParam(mTargetMap, TargetMap);
		registeParam(mTargetMapName, TargetMapName);
		registeParam(mTargetPosition, TargetPosition);
	}
};

#endif