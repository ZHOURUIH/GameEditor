#include "MapTile.h"
#include "txSerializer.h"

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
	if (mHasAni || mAniFrame > 0)
	{
		int a = 0;
	}
	SET_HIGHEST_BIT(mAniFrame, 0);
}