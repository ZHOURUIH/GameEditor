#include "MapTile.h"
#include "txSerializer.h"
#include "MapTileSimple.h"

void MapTile::parseTile(char* buffer, int bufferSize, int& offset)
{
	txSerializer serializer(buffer, bufferSize);
	serializer.setIndex(offset);
	serializer.read(mBngImgIdx);
	serializer.read(mMidImgIdx);
	serializer.read(mObjImgIdx);
	serializer.read(mDoorIdx);
	serializer.read(mDoorOffset);
	serializer.read(mAniFrame);
	serializer.read(mAniTick);
	serializer.read(mObjFileIdx);
	serializer.read(mLight);
	offset = serializer.getIndex();

	mOriginBngImgIdx = mBngImgIdx;
	mOriginMidImgIdx = mMidImgIdx;
	mOriginObjImgIdx = mObjImgIdx;
	mOriginAniFrame = mAniFrame;
	bool hasBng = GET_HIGHEST_BIT(mBngImgIdx) == 1;
	bool hasObj = GET_HIGHEST_BIT(mObjImgIdx) == 1;
	mCanWalk = (!hasBng && !hasObj);
	mCanFly = !hasObj;
	mDoorOpen = GET_HIGHEST_BIT(mDoorOffset) == 1;
	mHasDoor = GET_HIGHEST_BIT(mDoorIdx) == 1;
	mHasAni = GET_HIGHEST_BIT(mAniFrame) == 1;
	SET_HIGHEST_BIT(mBngImgIdx, 0);
	SET_HIGHEST_BIT(mMidImgIdx, 0);
	SET_HIGHEST_BIT(mObjImgIdx, 0);
	SET_HIGHEST_BIT(mAniFrame, 0);
}

void MapTile::saveTile(txSerializer* serializer)
{
	serializer->write(mOriginBngImgIdx);
	serializer->write(mOriginMidImgIdx);
	serializer->write(mOriginObjImgIdx);
	serializer->write(mDoorIdx);
	serializer->write(mDoorOffset);
	serializer->write(mOriginAniFrame);
	serializer->write(mAniTick);
	serializer->write(mObjFileIdx);
	serializer->write(mLight);
}

void MapTile::toSimple(MapTileSimple* simpleTile)
{
	simpleTile->mBngImgIdx = mBngImgIdx;
	simpleTile->mMidImgIdx = mMidImgIdx;
	simpleTile->mObjImgIdx = mObjImgIdx;
	simpleTile->mObjFileIdx = mObjFileIdx;
	simpleTile->mCanWalk = mCanWalk;
}

string MapTile::toString()
{
	string str;
	str += StringUtility::intToString(mOriginBngImgIdx);
	str += " ";
	str += StringUtility::intToString(mOriginMidImgIdx);
	str += " ";
	str += StringUtility::intToString(mOriginObjImgIdx);
	str += " ";
	str += StringUtility::intToString(mDoorIdx);
	str += " ";
	str += StringUtility::intToString(mDoorOffset);
	str += " ";
	str += StringUtility::intToString(mOriginAniFrame);
	str += " ";
	str += StringUtility::intToString(mAniTick);
	str += " ";
	str += StringUtility::intToString(mObjFileIdx);
	str += " ";
	str += StringUtility::intToString(mLight);
	return str;
}

void MapTile::parseString(const string& str)
{
	myVector<string> vec;
	StringUtility::split(str.c_str(), " ", vec);
	if (vec.size() != 9)
	{
		cout << "错误:地砖的文本数据错误,下标:" << mIndex << endl;
		return;
	}
	mBngImgIdx = StringUtility::stringToInt(vec[0]);
	mMidImgIdx = StringUtility::stringToInt(vec[1]);
	mObjImgIdx = StringUtility::stringToInt(vec[2]);
	mDoorIdx = StringUtility::stringToInt(vec[3]);
	mDoorOffset = StringUtility::stringToInt(vec[4]);
	mAniFrame = StringUtility::stringToInt(vec[5]);
	mAniTick = StringUtility::stringToInt(vec[6]);
	mObjFileIdx = StringUtility::stringToInt(vec[7]);
	mLight = StringUtility::stringToInt(vec[8]);

	mOriginBngImgIdx = mBngImgIdx;
	mOriginMidImgIdx = mMidImgIdx;
	mOriginObjImgIdx = mObjImgIdx;
	mOriginAniFrame = mAniFrame;
	bool hasBng = GET_HIGHEST_BIT(mBngImgIdx) == 1;
	bool hasObj = GET_HIGHEST_BIT(mObjImgIdx) == 1;
	mCanWalk = (!hasBng && !hasObj);
	mCanFly = !hasObj;
	mDoorOpen = GET_HIGHEST_BIT(mDoorOffset) == 1;
	mHasDoor = GET_HIGHEST_BIT(mDoorIdx) == 1;
	mHasAni = GET_HIGHEST_BIT(mAniFrame) == 1;
	SET_HIGHEST_BIT(mBngImgIdx, 0);
	SET_HIGHEST_BIT(mMidImgIdx, 0);
	SET_HIGHEST_BIT(mObjImgIdx, 0);
	SET_HIGHEST_BIT(mAniFrame, 0);
}