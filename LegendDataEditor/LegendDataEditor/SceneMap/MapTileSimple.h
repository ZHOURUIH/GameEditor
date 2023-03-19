#ifndef _MAP_TILE_SIMPLE_H_
#define _MAP_TILE_SIMPLE_H_

#include "ServerDefine.h"

class txSerializer;
class MapTileSimple
{
public:
	short mBngImgIdx = 0;		// 背景图索引
	short mMidImgIdx = 0;		// 补充背景图索引
	short mObjImgIdx = 0;		// 对象图索引
	byte mObjFileIdx = 0;		// 资源文件索引
	bool mHasBng = false;		// 是否有背景图(在热血传奇2地图中，背景图大小为4个地图块，具体到绘制地图时则表现在只有横纵坐标都为双数时才绘制)
	bool mCanWalk = false;		// 是否可行走(站立)
	bool mHasMid = false;		// 是否有补充图
	bool mHasObj = false;		// 是否有对象图
	myVector<int> mObjectContainsUnreachableIndexList;   // 如果该坐标包含对象物体,则表示该对象物体所覆盖的所有不可行走的坐标
	int mUnreachGroupID = -1;
	int mIndex = 0;				// 地砖下标
public:
	void parseTile(char* buffer, int bufferSize, int& offset);
	void saveTile(txSerializer* serializer);
};

#endif