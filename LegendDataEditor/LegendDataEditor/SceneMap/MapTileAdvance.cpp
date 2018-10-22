#include "MapTileAdvance.h"
#include "txSerializer.h"
#include "MapTile.h"

void MapTileAdvance::parseTile(char* buffer, int bufferSize, int& offset)
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
	serializer.read(mObjAtlasIndex);
	serializer.read(mBngAtlasIndex);
	serializer.read(mLight);
	offset = serializer.getIndex();
	mHasBng = GET_HIGHEST_BIT(mBngImgIdx) == 1;
	SET_HIGHEST_BIT(mBngImgIdx, 0);
	mHasMid = GET_HIGHEST_BIT(mMidImgIdx) == 1;
	SET_HIGHEST_BIT(mMidImgIdx, 0);
	mHasObj = GET_HIGHEST_BIT(mObjImgIdx) == 1;
	SET_HIGHEST_BIT(mObjImgIdx, 0);
	mCanWalk = (!mHasBng && !mHasObj);
	mCanFly = !mHasObj;
	mDoorOpen = GET_HIGHEST_BIT(mDoorOffset) == 1;
	mHasDoor = GET_HIGHEST_BIT(mDoorIdx) == 1;
	mHasAni = GET_HIGHEST_BIT(mAniFrame) == 1;
	SET_HIGHEST_BIT(mAniFrame, 0);
}

void MapTileAdvance::initFromOldTile(MapTile& tile, unsigned char objAtlasIndex, unsigned char bngAtlasIndex)
{
	mBngImgIdx = tile.mBngImgIdx;
	mMidImgIdx = tile.mMidImgIdx;
	mObjImgIdx = tile.mObjImgIdx;
	mDoorIdx = tile.mDoorIdx;
	mDoorOffset = tile.mDoorOffset;
	mAniFrame = tile.mAniFrame;
	mAniTick = tile.mAniTick;
	mObjFileIdx = tile.mObjFileIdx;
	mObjAtlasIndex = objAtlasIndex;
	mBngAtlasIndex = bngAtlasIndex;
	mLight = tile.mLight;
	mHasBng = tile.mHasBng;
	mCanWalk = tile.mCanWalk;
	mHasMid = tile.mHasMid;
	mHasObj = tile.mHasObj;
	mCanFly = tile.mCanFly;
	mHasDoor = tile.mHasDoor;
	mDoorOpen = tile.mDoorOpen;
	mHasAni = tile.mHasAni;
}

void MapTileAdvance::saveTile(txSerializer* serializer)
{
	short bngImgIdx = mBngImgIdx;
	if (mHasBng)
	{
		SET_HIGHEST_BIT(bngImgIdx, 1);
	}
	short midImgIdx = mMidImgIdx;
	if (mHasMid)
	{
		SET_HIGHEST_BIT(midImgIdx, 1);
	}
	short objImgIdx = mObjImgIdx;
	if (mHasObj)
	{
		SET_HIGHEST_BIT(objImgIdx, 1);
	}
	unsigned char aniFrame = mAniFrame;
	if (mHasAni)
	{
		SET_HIGHEST_BIT(aniFrame, 1);
	}
	serializer->write(bngImgIdx);
	serializer->write(midImgIdx);
	serializer->write(objImgIdx);
	serializer->write(mDoorIdx);
	serializer->write(mDoorOffset);
	serializer->write(aniFrame);
	serializer->write(mAniTick);
	serializer->write(mObjFileIdx);
	serializer->write(mObjAtlasIndex);
	serializer->write(mBngAtlasIndex);
	serializer->write(mLight);
}