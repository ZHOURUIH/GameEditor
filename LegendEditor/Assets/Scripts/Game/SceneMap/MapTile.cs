using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEngine;

public class MapTile : GameBase
{
	public int mIndex;
	public short mBngImgIdx;	// 背景图索引
	public short mMidImgIdx;	// 补充背景图索引
	public short mObjImgIdx;	// 对象图索引
	public byte mDoorIdx;		// 门索引
	public byte mDoorOffset;	// 门偏移
	public byte mAniFrame;		// 动画帧数
	public byte mAniTick;		// 动画跳帧数
	public byte mObjFileIdx;	// 资源文件索引
	public byte mLight;			// 亮度
	public bool mHasBng;		// 是否有背景图(在热血传奇2地图中，背景图大小为4个地图块，具体到绘制地图时则表现在只有横纵坐标都为双数时才绘制)
	public bool mCanWalk;		// 是否可行走(站立)
	public bool mHasMid;		// 是否有补充图
	public bool mHasObj;		// 是否有对象图
	public bool mCanFly;		// 是否可以飞越
	public bool mHasDoor;		// 是否有门
	public bool mDoorOpen;		// 门是否开启	 
	public bool mHasAni;		// 是否有动画
	public MapTile(int index)
	{
		mIndex = index;
	}
	public void parseTile(byte[] buffer, ref int offset)
	{
		Serializer serializer = new Serializer(buffer);
		serializer.setIndex(offset);
		serializer.read(ref mBngImgIdx);
		serializer.read(ref mMidImgIdx);
		serializer.read(ref mObjImgIdx);
		serializer.read(ref mDoorIdx);
		serializer.read(ref mDoorOffset);
		serializer.read(ref mAniFrame);
		serializer.read(ref mAniTick);
		serializer.read(ref mObjFileIdx);
		serializer.read(ref mLight);
		offset = serializer.getIndex();
		
		mHasBng = BinaryUtility.getHightestBit(mBngImgIdx) == 1;
		BinaryUtility.setHighestBit(ref mBngImgIdx, 0);
		mHasMid = BinaryUtility.getHightestBit(mMidImgIdx) == 1;
		BinaryUtility.setHighestBit(ref mMidImgIdx, 0);
		mHasObj = BinaryUtility.getHightestBit(mObjImgIdx) == 1;
		BinaryUtility.setHighestBit(ref mObjImgIdx, 0);
		mCanWalk = (!mHasBng && !mHasObj);
		mCanFly = !mHasObj;
		mDoorOpen = BinaryUtility.getHightestBit(mDoorOffset) == 1;
		mHasDoor = BinaryUtility.getHightestBit(mDoorIdx) == 1;
		mHasAni = BinaryUtility.getHightestBit(mAniFrame) == 1;
		BinaryUtility.setHighestBit(ref mAniFrame, 0);
	}
}