#include "SceneMap.h"
#include "MapTile.h"
#include "txSerializer.h"
#include "MapHeader.h"

SceneMap::SceneMap()
{
	TRACE_NEW(MapHeader, mHeader);
	mTileList = NULL;
}

void SceneMap::destroy()
{
	TRACE_DELETE(mTileList);
	TRACE_DELETE(mHeader);
}

void SceneMap::readFile(const string& fileName)
{
	int fileSize = 0;
	char* fileBuffer = FileUtility::openBinaryFile(fileName, &fileSize);
	int offset = 0;
	mHeader->parseHeader(fileBuffer, fileSize, offset);
	int tileCount = mHeader->mWidth * mHeader->mHeight;
	TRACE_NEW_ARRAY(MapTile, tileCount, mTileList);
	for (int i = 0; i < tileCount; ++i)
	{
		mTileList[i].parseTile(fileBuffer, fileSize, offset);
	}
	TRACE_DELETE_ARRAY(fileBuffer);
}