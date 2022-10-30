#ifndef _MAP_TILE_H_
#define _MAP_TILE_H_

#include "ServerDefine.h"

class MapTileSimple;
class MapTile
{
public:
	short mBngImgIdx = 0;			// 背景图索引
	short mMidImgIdx = 0;			// 补充背景图索引
	short mObjImgIdx = 0;			// 对象图索引
	unsigned char mDoorIdx = 0;		// 门索引
	unsigned char mDoorOffset = 0;	// 门偏移
	unsigned char mAniFrame = 0;	// 动画帧数
	unsigned char mAniTick = 0;		// 动画跳帧数
	unsigned char mObjFileIdx = 0;	// 资源文件索引
	unsigned char mLight = 0;		// 亮度
	bool mCanWalk = false;			// 是否可行走(站立)
	bool mCanFly = false;			// 是否可以飞越
	bool mHasDoor = false;			// 是否有门
	bool mDoorOpen = false;			// 门是否开启
	bool mHasAni = false;			// 是否有动画
	myVector<int> mObjectContainsUnreachableIndexList;   // 如果该坐标包含对象物体,则表示该对象物体所覆盖的所有不可行走的坐标
	int mUnreachGroupID = -1;
	int mIndex = 0;					// 地砖下标
	bool mAroundChecked = false;
public:
	void parseTile(char* buffer, int bufferSize, int& offset);
	void toSimple(MapTileSimple* simpleTile);
};

#endif