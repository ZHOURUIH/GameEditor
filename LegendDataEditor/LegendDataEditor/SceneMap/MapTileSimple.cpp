#include "MapTileSimple.h"
#include "txSerializer.h"

MapTileSimple::MapTileSimple()
{
	;
}

void MapTileSimple::parseTile(char* buffer, int bufferSize, int& offset)
{
	txSerializer serializer(buffer, bufferSize);
	serializer.setIndex(offset);
	serializer.read(mBngImgIdx);
	serializer.read(mMidImgIdx);
	serializer.read(mObjImgIdx);
	serializer.read(mObjFileIdx);
	offset = serializer.getIndex();
	mHasBng = GET_HIGHEST_BIT(mBngImgIdx) == 1;
	SET_HIGHEST_BIT(mBngImgIdx, 0);
	mHasMid = GET_HIGHEST_BIT(mMidImgIdx) == 1;
	SET_HIGHEST_BIT(mMidImgIdx, 0);
	mHasObj = GET_HIGHEST_BIT(mObjImgIdx) == 1;
	SET_HIGHEST_BIT(mObjImgIdx, 0);
	mCanWalk = (!mHasBng && !mHasObj);
}

void MapTileSimple::saveTile(txSerializer* serializer)
{
	serializer->write(mBngImgIdx);
	serializer->write(mMidImgIdx);
	serializer->write(mObjImgIdx);
	serializer->write(mObjFileIdx);
}