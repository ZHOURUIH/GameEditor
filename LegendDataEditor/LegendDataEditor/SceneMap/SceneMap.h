#ifndef _SCENE_MAP_H_
#define _SCENE_MAP_H_

#include "ServerDefine.h"

class MapTile;
class MapHeader;
class SceneMap
{
public:
	MapHeader* mHeader;
	MapTile* mTileList;
public:
	SceneMap();
	~SceneMap() { destroy(); }
	void destroy();
	void readFile(const std::string& fileName);
};

#endif