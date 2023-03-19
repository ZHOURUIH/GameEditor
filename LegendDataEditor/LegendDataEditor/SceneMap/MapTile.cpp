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
	bool hasBng = GET_HIGHEST_BIT(mBngImgIdx) == 1;
	SET_HIGHEST_BIT(mBngImgIdx, 0);
	SET_HIGHEST_BIT(mMidImgIdx, 0);
	bool hasObj = GET_HIGHEST_BIT(mObjImgIdx) == 1;
	SET_HIGHEST_BIT(mObjImgIdx, 0);
	mCanWalk = (!hasBng && !hasObj);
	mCanFly = !hasObj;
	mDoorOpen = GET_HIGHEST_BIT(mDoorOffset) == 1;
	mHasDoor = GET_HIGHEST_BIT(mDoorIdx) == 1;
	mHasAni = GET_HIGHEST_BIT(mAniFrame) == 1;
	SET_HIGHEST_BIT(mAniFrame, 0);
}

void MapTile::saveTile(txSerializer* serializer)
{
	serializer->write(mBngImgIdx);
	serializer->write(mMidImgIdx);
	serializer->write(mObjImgIdx);
	serializer->write(mDoorIdx);
	serializer->write(mDoorOffset);
	serializer->write(mAniFrame);
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