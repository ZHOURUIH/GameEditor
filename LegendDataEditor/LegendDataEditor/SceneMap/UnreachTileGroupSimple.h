#ifndef _UNREACH_TILE_GROUP_SIMPLE_H_
#define _UNREACH_TILE_GROUP_SIMPLE_H_

#include "ServerDefine.h"

class MapDataSimple;
class MapTileSimple;
class UnreachTileGroupSimple
{
public:
	UnreachTileGroupSimple(int id, MapDataSimple* mapData);
	void addTile(MapTileSimple* tile);
	void optimizeUnreach();
	bool checkPositionValid(const Vector2& pos, int tileIndex = -1);
protected:
	int getTileIndexInList(int x, int y);
	bool generateAddTriangle(int x, int y);
public:
	MapDataSimple* mMapData = nullptr;
	int mGroupID = -1;
	myMap<int, MapTileSimple*> mTileList;
	myMap<int, myVector<int>> mTriangleList;    // key是地砖下标,value是该地砖中的三角形下标
	int mMapHeight = 0;
	int mMapWidth = 0;
	static myVector<int> mTempAddTriangles;
};

#endif