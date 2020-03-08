#ifndef _MAP_TILE_H_
#define _MAP_TILE_H_

#include "ServerDefine.h"

class MapTile
{
public:
	short mBngImgIdx;// 背景图索引
	short mMidImgIdx;// 补充背景图索引
	short mObjImgIdx;// 对象图索引
	unsigned char mDoorIdx;// 门索引
	unsigned char mDoorOffset;// 门偏移
	unsigned char mAniFrame;// 动画帧数
	unsigned char mAniTick;// 动画跳帧数
	unsigned char mObjFileIdx;// 资源文件索引
	unsigned char mLight;// 亮度
	bool mHasBng;// 是否有背景图(在热血传奇2地图中，背景图大小为4个地图块，具体到绘制地图时则表现在只有横纵坐标都为双数时才绘制)
	bool mCanWalk;// 是否可行走(站立)
	bool mHasMid;// 是否有补充图
	bool mHasObj;// 是否有对象图
	bool mCanFly;// 是否可以飞越
	bool mHasDoor;// 是否有门
	bool mDoorOpen;// 门是否开启
	bool mHasAni;// 是否有动画
	txVector<int> mObjectContainsUnreachableIndexList;   // 如果该坐标包含对象物体,则表示该对象物体所覆盖的所有不可行走的坐标
	int mUnreachGroupID;
	int mIndex;
	bool mAroundChecked;
public:
	MapTile();
	void parseTile(char* buffer, int bufferSize, int& offset);
};

#endif