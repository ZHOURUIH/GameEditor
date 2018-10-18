#ifndef _MAP_TILE_H_
#define _MAP_TILE_H_

#include "ServerDefine.h"

class MapTile
{
public:
	// 背景图索引
	short mBngImgIdx;
	// 补充背景图索引
	short mMidImgIdx;
	// 对象图索引
	short mObjImgIdx;
	// 门索引
	unsigned char mDoorIdx;
	// 门偏移
	unsigned char mDoorOffset;
	// 动画帧数
	unsigned char mAniFrame;
	// 动画跳帧数
	unsigned char mAniTick;
	// 资源文件索引
	unsigned char mObjFileIdx;
	// 亮度
	unsigned char mLight;
	// 是否有背景图(在热血传奇2地图中，背景图大小为4个地图块，具体到绘制地图时则表现在只有横纵坐标都为双数时才绘制)
	bool mHasBng;
	// 是否可行走(站立)
	bool mCanWalk;
	// 是否有补充图
	bool mHasMid;
	// 是否有对象图
	bool mHasObj;
	// 是否可以飞越
	bool mCanFly;
	// 是否有门
	bool mHasDoor;
	// 门是否开启
	bool mDoorOpen;
	// 是否有动画
	bool mHasAni;
public:
	void parseTile(char* buffer, int bufferSize, int& offset);
};

#endif