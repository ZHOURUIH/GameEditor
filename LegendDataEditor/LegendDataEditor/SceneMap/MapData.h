#ifndef _MAP_DATA_H_
#define _MAP_DATA_H_

#include "ServerDefine.h"

class MapTile;
class MapHeader;
class UnreachTileGroup;
class MapData
{
public:
	string mFileName;
	MapHeader* mHeader = nullptr;
	MapTile* mTileList = nullptr;
	myMap<int, UnreachTileGroup*> mUnreachTileGroupList;    // key是组ID,value是该组中所有的地砖
	int mTileCount = 0;
public:
	MapData();
	~MapData() { destroy(); }
	void destroy();
	bool readFile(const string& fileName);
	void writeFile(const string& fileName);
	void writeUnreachFile();
	void convertToSimple(const string& writeFile);
protected:
	// 递归方式查找不可行走区域组,但是如果地砖太多,就会因为递归太深而堆栈溢出
	//void findAllUnreachGroup();
	//void findGroup(int x, int y, int id);
	// 非递归方式查找不可行走区域组
	void findAllUnreachGroupNoRecursive();
	void assignGroupID(MapTile* tile, UnreachTileGroup* group, myVector<int>& waitForList);
	int getTileUnreachIndex(int x, int y);
};

#endif