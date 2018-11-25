#ifndef _SCENE_MAP_ADVANCE_H_
#define _SCENE_MAP_ADVANCE_H_

#include "ServerDefine.h"

class MapTileAdvance;
class MapHeader;
class SceneMap;
class SceneMapAdvance
{
public:
	MapHeader* mHeader;
	MapTileAdvance* mTileList;
public:
	SceneMapAdvance();
	~SceneMapAdvance() { destroy(); }
	void destroy();
	void readFile(const string& fileName, bool parseTile = true);
	// atlasIndexMap的第一个key表示文件夹下标,比如Objects1,Objects2的最后的数字
	// 第二个key表示在该文件夹下的文件名,不含后缀名,因为文件名都是数字
	// 最后一个value表示该文件所在的图集下标
	void initFromMap(SceneMap* oldMap, txMap<int, txMap<int, int>>& objAtlasIndexMap, txMap<int, int>& bngAltasIndex);
	void saveAdvanceMap(const string& fileName);
};

#endif