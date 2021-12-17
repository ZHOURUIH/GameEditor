#ifndef _MAP_TILE_SIMPLE_H_
#define _MAP_TILE_SIMPLE_H_

#include "ServerDefine.h"

class txSerializer;
class MapTileSimple
{
public:
	short mBngImgIdx;// 背景图索引
	short mMidImgIdx;// 补充背景图索引
	short mObjImgIdx;// 对象图索引
	unsigned char mObjFileIdx;// 资源文件索引
	bool mHasBng;// 是否有背景图(在热血传奇2地图中，背景图大小为4个地图块，具体到绘制地图时则表现在只有横纵坐标都为双数时才绘制)
	bool mCanWalk;// 是否可行走(站立)
	bool mHasMid;// 是否有补充图
	bool mHasObj;// 是否有对象图
public:
	MapTileSimple();
	void parseTile(char* buffer, int bufferSize, int& offset);
	void saveTile(txSerializer* serializer);
};

#endif