#ifndef _SCENE_MAP_H_
#define _SCENE_MAP_H_

#include "ServerDefine.h"

class MapData;
class MapTile;
class UnreachTileGroup
{
public:
	UnreachTileGroup(int id, MapData* mapData);
	void addTile(MapTile* tile);
	void optimizeUnreach();
	bool checkPositionValid(const Vector2& pos, int tileIndex = -1);
protected:
	int getTileIndexInList(int x, int y);
	bool generateAddTriangle(int x, int y);
public:
	MapData* mMapData;
	int mGroupID;
	myMap<int, MapTile*> mTileList;
	myMap<int, myVector<int>> mTriangleList;    // key是地砖下标,value是该地砖中的三角形下标
	int mMapHeight;
	int mMapWidth;
	static myVector<int> mTempAddTriangles;
};

#endif