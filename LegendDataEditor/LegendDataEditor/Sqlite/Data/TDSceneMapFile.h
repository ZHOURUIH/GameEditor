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
public:
	TDSceneMapFile()
	{
		registeParam(mID, ID);
		registeParam(mFileName, FileName);
		registeParamEmpty(MiniMap);
		registeParamEmpty(Width);
		registeParamEmpty(Height);
	}
};

#endif