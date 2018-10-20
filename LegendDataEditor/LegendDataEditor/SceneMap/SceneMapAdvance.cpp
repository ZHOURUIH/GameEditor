#include "SceneMapAdvance.h"
#include "MapTileAdvance.h"
#include "txSerializer.h"
#include "MapHeader.h"
#include "SceneMap.h"
#include "MapTile.h"

SceneMapAdvance::SceneMapAdvance()
{
	TRACE_NEW(MapHeader, mHeader);
	mTileList = NULL;
}

void SceneMapAdvance::destroy()
{
	TRACE_DELETE(mTileList);
	TRACE_DELETE(mHeader);
}

void SceneMapAdvance::readFile(const std::string& fileName)
{
	int fileSize = 0;
	char* fileBuffer = FileUtility::openBinaryFile(fileName, &fileSize);
	int offset = 0;
	mHeader->parseHeader(fileBuffer, fileSize, offset);
	int tileCount = mHeader->mWidth * mHeader->mHeight;
	TRACE_NEW_ARRAY(MapTileAdvance, tileCount, mTileList);
	for (int i = 0; i < tileCount; ++i)
	{
		mTileList[i].parseTile(fileBuffer, fileSize, offset);
	}
	TRACE_DELETE_ARRAY(fileBuffer);
}

void SceneMapAdvance::initFromMap(SceneMap* oldMap, txMap<int, txMap<int, int>>& objAtlasIndexMap, txMap<int, int>& bngAltasIndexMap)
{
	// 文件头直接赋值
	*mHeader = *(oldMap->mHeader);
	int tileCount = mHeader->mWidth * mHeader->mHeight;
	TRACE_NEW_ARRAY(MapTileAdvance, tileCount, mTileList);
	for (int i = 0; i < tileCount; ++i)
	{
		int fileIndex = oldMap->mTileList[i].mObjFileIdx + 1;
		int imageIndex = oldMap->mTileList[i].mObjImgIdx - 1;
		int objAtlasIndex = 0xFF;
		int bngAtlasIndex = 0xFF;
		if (objAtlasIndexMap.contains(fileIndex))
		{
			objAtlasIndex = objAtlasIndexMap[fileIndex].tryGet(imageIndex, objAtlasIndex);
		}
		bngAtlasIndex = bngAltasIndexMap.tryGet(oldMap->mTileList[i].mBngImgIdx - 1, bngAtlasIndex);
		mTileList[i].initFromOldTile(oldMap->mTileList[i], objAtlasIndex, bngAtlasIndex);
	}
}

void SceneMapAdvance::saveAdvanceMap(const std::string& fileName)
{
	txSerializer serializer;
	mHeader->saveHeader(&serializer);
	int tileCount = mHeader->mWidth * mHeader->mHeight;
	for (int i = 0; i < tileCount; ++i)
	{
		mTileList[i].saveTile(&serializer);
	}
	serializer.writeToFile(fileName);
}