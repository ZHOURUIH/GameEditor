#ifndef _TD_SCENE_MAP_FILE_H_
#define _TD_SCENE_MAP_FILE_H_

#include "SQLiteData.h"

class TDSceneMapFile : public SQLiteData
{
public:
	static const char* ID;
	static const char* FileName;
	static const char* MiniMap;
	static const char* Width;
	static const char* Height;
public:
	int mID;
	string mFileName;
	short mMiniMap;
	ushort mWidth;
	ushort mHeight;
public:
	TDSceneMapFile()
	{
		registeParam(mID, ID);
		registeParam(mFileName, FileName);
		registeParam(mMiniMap, MiniMap);
		registeParam(mWidth, Width);
		registeParam(mHeight, Height);
	}
};

#endif