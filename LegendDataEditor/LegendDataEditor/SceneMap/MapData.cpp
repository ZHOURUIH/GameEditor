#include "MapData.h"
#include "MapTile.h"
#include "txSerializer.h"
#include "MapHeader.h"
#include "UnreachTileGroup.h"
#include "ImageUtility.h"

MapData::MapData()
{
	NEW(MapHeader, mHeader);
	mTileList = NULL;
	mIDSeed = 0;
}

void MapData::destroy()
{
	FOREACH(iter, mUnreachTileGroupList)
	{
		DELETE(iter->second);
	}
	END(mUnreachTileGroupList);
	DELETE_ARRAY(mTileList);
	DELETE(mHeader);
}

void MapData::readFile(const string& fileName)
{
	mFileName = fileName;
	FileContent file;
	FileUtility::openBinaryFile(fileName, file);
	int offset = 0;
	mHeader->parseHeader(file.mBuffer, file.mFileSize, offset);
	int tileCount = mHeader->mWidth * mHeader->mHeight;
	NEW_ARRAY(MapTile, tileCount, mTileList);
	for (int i = 0; i < tileCount; ++i)
	{
		mTileList[i].mIndex = i;
		mTileList[i].parseTile(file.mBuffer, file.mFileSize, offset);
	}
	// 计算所有的不可行走区域组
	findAllUnreachGroupNoRecursive();
}

void MapData::writeUnreachFile()
{
	txSerializer serializer;
	serializer.write(mHeader->mWidth);
	serializer.write(mHeader->mHeight);
	// 组数量
	serializer.write((unsigned short)mUnreachTileGroupList.size());
	auto iter = mUnreachTileGroupList.begin();
	auto iterEnd = mUnreachTileGroupList.end();
	for (; iter != iterEnd; ++iter)
	{
		// 地砖数量
		UnreachTileGroup* group = iter->second;
		serializer.write((unsigned int)group->mTriangleList.size());
		auto iterTile = group->mTriangleList.begin();
		auto iterTileEnd = group->mTriangleList.end();
		for (; iterTile != iterTileEnd; ++iterTile)
		{
			// 地砖下标
			serializer.write((unsigned int)iterTile->first);
			// 三角形数量
			// 每个地砖最多有8个三角形,所以可以使用一个字节的8位表示是否有指定的三角形
			byte triangleFlag = 0;
			auto& triangleList = iterTile->second;
			FOR_I(triangleList.size())
			{
				triangleFlag |= 1 << triangleList[i];
			}
			serializer.write(triangleFlag);
		}
	}
	serializer.writeToFile(StringUtility::getFileNameNoSuffix(mFileName, false) + ".unreach");
}

//void MapData::findAllUnreachGroup()
//{
//	int tileCount = mHeader->mWidth * mHeader->mHeight;
//	for (int i = 0; i < tileCount; ++i)
//	{
//		MapTile& tile = mTileList[i];
//		if (!tile.mCanWalk && tile.mUnreachGroupID == -1)
//		{
//			int id = ++mIDSeed;
//			mUnreachTileGroupList.insert(id, new UnreachTileGroup(id, this));
//			int x = ImageUtility::tileIndexToTileX(tile.mIndex, mHeader->mHeight);
//			int y = ImageUtility::tileIndexToTileY(tile.mIndex, mHeader->mHeight);
//			findGroup(x, y, id);
//		}
//	}
//	// 优化阻挡区域
//	auto iter = mUnreachTileGroupList.begin();
//	auto iterEnd = mUnreachTileGroupList.end();
//	for (; iter != iterEnd; ++iter)
//	{
//		iter->second->optimizeUnreach();
//	}
//}

//void MapData::findGroup(int x, int y, int id)
//{
//	// 判断当前坐标是否允许行走
//	if (x < 0 || x >= mHeader->mWidth || y < 0 || y >= mHeader->mHeight)
//	{
//		return;
//	}
//	{
//		int tileIndex = ImageUtility::tilePosToTileIndex(x, y, mHeader->mHeight);
//		MapTile& tile = mTileList[tileIndex];
//		if (tile.mCanWalk || tile.mUnreachGroupID != -1)
//		{
//			return;
//		}
//		tile.mUnreachGroupID = id;
//		mUnreachTileGroupList[id]->addTile(&tile);
//	}
//	// 上
//	findGroup(x, y - 1, id);
//	// 下
//	findGroup(x, y + 1, id);
//	// 左
//	findGroup(x - 1, y, id);
//	// 右
//	findGroup(x + 1, y, id);
//}

int MapData::getTileUnreachIndex(int x, int y)
{
	if (x < 0 || x >= mHeader->mWidth || y < 0 || y >= mHeader->mHeight)
	{
		return -1;
	}
	// 地砖不可行走,并且没有被分配到任何组
	int index = ImageUtility::tilePosToTileIndex(x, y, mHeader->mHeight);
	if (!mTileList[index].mCanWalk && mTileList[index].mUnreachGroupID == -1)
	{
		return index;
	}
	return -1;
}

void MapData::findAllUnreachGroupNoRecursive()
{
	// 找出所有的不可行走的地砖
	mySet<int> tileNearList;
	int tileCount = mHeader->mWidth * mHeader->mHeight;
	for (int i = 0; i < tileCount; ++i)
	{
		MapTile& tile = mTileList[i];
		if (!tile.mCanWalk)
		{
			tileNearList.insert(i);
		}
	}
	while (true)
	{
		if (tileNearList.size() == 0)
		{
			break;
		}
		int id = ++mIDSeed;
		UnreachTileGroup* group = NEW(UnreachTileGroup, group, id, this);
		mUnreachTileGroupList.insert(id, group);
		myVector<int> waitForList;
		waitForList.push_back(*tileNearList.begin());
		while (true) 
		{
			if (waitForList.size() == 0)
			{
				break;
			}
			// 设置地砖组ID,并且将该地砖周围的地砖加入到待检查列表中
			myVector<int> nextWaitForList;
			int waitForCount = waitForList.size();
			for (int i = 0; i < waitForCount; ++i)
			{
				assignGroupID(&(mTileList[waitForList[i]]), group, nextWaitForList);
				tileNearList.erase(waitForList[i]);
			}
			waitForList = nextWaitForList;
		}
	}
	// 优化阻挡区域
	auto iter = mUnreachTileGroupList.begin();
	auto iterEnd = mUnreachTileGroupList.end();
	for (; iter != iterEnd; ++iter)
	{
		iter->second->optimizeUnreach();
	}
}

void MapData::assignGroupID(MapTile* tile, UnreachTileGroup* group, myVector<int>& waitForList)
{
	tile->mUnreachGroupID = group->mGroupID;
	group->addTile(tile);
	int x = ImageUtility::tileIndexToTileX(tile->mIndex, mHeader->mHeight);
	int y = ImageUtility::tileIndexToTileY(tile->mIndex, mHeader->mHeight);
	int top = getTileUnreachIndex(x, y - 1);
	int bottom = getTileUnreachIndex(x, y + 1);
	int left = getTileUnreachIndex(x - 1, y);
	int right = getTileUnreachIndex(x + 1, y);
	if (top >= 0 && !waitForList.contains(top))
	{
		waitForList.push_back(top);
	}
	if (bottom >= 0 && !waitForList.contains(bottom))
	{
		waitForList.push_back(bottom);
	}
	if (left >= 0 && !waitForList.contains(left))
	{
		waitForList.push_back(left);
	}
	if (right >= 0 && !waitForList.contains(right))
	{
		waitForList.push_back(right);
	}
}