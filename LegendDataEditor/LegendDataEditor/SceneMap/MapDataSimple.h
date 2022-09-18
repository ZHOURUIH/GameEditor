#ifndef _MAP_DATA_SIMPLE_H_
#define _MAP_DATA_SIMPLE_H_

#include "ServerDefine.h"

class MapTileSimple;
class MapHeader;
class UnreachTileGroupSimple;
class MapDataSimple
{
public:
	string mFileName;
	MapHeader* mHeader = nullptr;
	MapTileSimple* mTileList = nullptr;
	myMap<int, UnreachTileGroupSimple*> mUnreachTileGroupList;    // key是组ID,value是该组中所有的地砖
	int mIDSeed = 0;
	int mTileCount = 0;
public:
	MapDataSimple();
	~MapDataSimple() { destroy(); }
	void destroy();
	void readFile(const string& fileName);
	void writeUnreachFile();
protected:
	// 递归方式查找不可行走区域组,但是如果地砖太多,就会因为递归太深而堆栈溢出
	//void findAllUnreachGroup();
	//void findGroup(int x, int y, int id);
	// 非递归方式查找不可行走区域组
	void findAllUnreachGroupNoRecursive();
	void assignGroupID(MapTileSimple* tile, UnreachTileGroupSimple* group, myVector<int>& waitForList);
	int getTileUnreachIndex(int x, int y);
};

#endif