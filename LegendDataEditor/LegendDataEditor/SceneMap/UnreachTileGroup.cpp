#include "UnreachTileGroup.h"
#include "MapData.h"
#include "MapHeader.h"
#include "MapTile.h"
#include "ImageUtility.h"

txVector<int> UnreachTileGroup::mTempAddTriangles;

UnreachTileGroup::UnreachTileGroup(int id, MapData* mapData)
{
	mGroupID = id;
	mMapData = mapData;
	mMapHeight = mMapData->mHeader->mHeight;
	mMapWidth = mMapData->mHeader->mWidth;
}
void UnreachTileGroup::addTile(MapTile* tile)
{
	if (mTileList.contains(tile->mIndex))
	{
		return;
	}
	mTileList.insert(tile->mIndex, tile);
	if (!mTriangleList.contains(tile->mIndex))
	{
		mTriangleList.insert(tile->mIndex, txVector<int>());
	}
	// 一个地砖中右8个三角形,左上角0,右上角1,右下角2,左下角3,中心左上角4,中心右上角5,中心右下角6,中心左下角7
	for (int i = 0; i < TT_MAX; ++i)
	{
		if (!mTriangleList[tile->mIndex].contains(i))
		{
			mTriangleList[tile->mIndex].push_back(i);
		}
	}
}
void UnreachTileGroup::optimizeUnreach()
{
	// 移除多余的三角形
	auto iter = mTileList.begin();
	auto iterEnd = mTileList.end();
	for(; iter != iterEnd; ++iter)
	{
		// 如果一个地砖四个角上的任意一个三角形没有与该组中其他的地砖相邻,则去掉该三角形
		int x = ImageUtility::tileIndexToTileX(iter->first, mMapHeight);
		int y = ImageUtility::tileIndexToTileY(iter->first, mMapHeight);
		int topTileIndex = getTileIndexInList(x, y - 1);
		int bottomTileIndex = getTileIndexInList(x, y + 1);
		int leftTileIndex = getTileIndexInList(x - 1, y);
		int rightTileIndex = getTileIndexInList(x + 1, y);
		// 左边和上边都没有与组中的地砖,则去掉左上角的三角形
		if (leftTileIndex == -1 && topTileIndex == -1)
		{
			mTriangleList[iter->first].erase(mTriangleList[iter->first].find((int)TT_LEFT_TOP));
		}
		// 右边和上边都没有与组中的地砖,则去掉右上角的三角形
		if (rightTileIndex == -1 && topTileIndex == -1)
		{
			mTriangleList[iter->first].erase(mTriangleList[iter->first].find((int)TT_RIGHT_TOP));
		}
		// 右边和下边都没有与组中的地砖,则去掉右下角的三角形
		if (rightTileIndex == -1 && bottomTileIndex == -1)
		{
			mTriangleList[iter->first].erase(mTriangleList[iter->first].find((int)TT_RIGHT_BOTTOM));
		}
		// 左边和下边都没有与组中的地砖,则去掉左下角的三角形
		if (leftTileIndex == -1 && bottomTileIndex == -1)
		{
			mTriangleList[iter->first].erase(mTriangleList[iter->first].find((int)TT_LEFT_BOTTOM));
		}
	}
	// 添加补充的三角形
	iter = mTileList.begin();
	for (; iter != iterEnd; ++iter)
	{
		// 一个地砖相邻的上下左右四个可行走的地砖,其中的任意一个地砖与组中两个地砖相邻,则补充一个三角形
		// 与三个或四个地砖相邻,则补充8个三角形,只与一个地砖相邻则不补充
		// 如果一个地砖四个角上的任意一个三角形没有与该组中其他的地砖相邻,则去掉该三角形
		int x = ImageUtility::tileIndexToTileX(iter->first, mMapHeight);
		int y = ImageUtility::tileIndexToTileY(iter->first, mMapHeight);
		// 上
		generateAddTriangle(x, y - 1);
		// 下
		generateAddTriangle(x, y + 1);
		// 左
		generateAddTriangle(x - 1, y);
		// 右
		generateAddTriangle(x + 1, y);
	}
}
// 判断指定坐标是否在不可行走区域内,传入tileIndex是为了避免重复计算,如果外部已经计算过了,则直接通过参数传进来
bool UnreachTileGroup::checkPositionValid(const Vector2& pos, int tileIndex)
{
	if (tileIndex == -1)
	{
		tileIndex = ImageUtility::pixelPosToTileIndex(pos, mMapHeight, mMapWidth);
		if (tileIndex < 0)
		{
			return false;
		}
	}
	if (mTriangleList.contains(tileIndex))
	{
		Vector2 tilePixelPos = ImageUtility::tileIndexToPixelPos(tileIndex, mMapHeight);
		TILE_TRIANGLE triangleIndex = ImageUtility::pixelPosToTriangleIndex(pos - tilePixelPos);
		return !mTriangleList[tileIndex].contains((int)triangleIndex);
	}
	return true;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------
// 在不可行走列表中获得指定坐标上地砖的下标
int UnreachTileGroup::getTileIndexInList(int x, int y)
{
	int tileIndex = -1;
	if (x >= 0 && x < mMapWidth && y >= 0 && y < mMapHeight)
	{
		tileIndex = ImageUtility::tilePosToTileIndex(x, y, mMapHeight);
		if (!mTileList.contains(tileIndex))
		{
			tileIndex = -1;
		}
	}
	return tileIndex;
}
// 坐标在x,y的地砖周围四个方向是否有不可行走地砖,如果有,则需要在该地砖上补充哪些三角形为不可行走区域
bool UnreachTileGroup::generateAddTriangle(int x, int y)
{
	if (x < 0 || x >= mMapWidth || y < 0 || y >= mMapHeight)
	{
		return false;
	}
	mTempAddTriangles.clear();
	int tileIndex = ImageUtility::tilePosToTileIndex(x, y, mMapHeight);
	if (mMapData->mTileList[tileIndex].mCanWalk)
	{
		int topTile = getTileIndexInList(x, y - 1);
		int bottomTile = getTileIndexInList(x, y + 1);
		int leftTile = getTileIndexInList(x - 1, y);
		int rightTile = getTileIndexInList(x + 1, y);
		int blockCount = 0;
		if (topTile != -1)
		{
			++blockCount;
		}
		if (bottomTile != -1)
		{
			++blockCount;
		}
		if (leftTile != -1)
		{
			++blockCount;
		}
		if (rightTile != -1)
		{
			++blockCount;
		}
		// 周围只有一个不可行走的地砖,则不补充
		if (blockCount == 1)
		{
			return false;
		}
		// 周围有两个不可行走的地砖,补充1个三角形
		else if (blockCount == 2)
		{
			if (leftTile != -1 && topTile != -1)
			{
				mTempAddTriangles.push_back((int)TT_LEFT_TOP);
			}
			else if (rightTile != -1 && topTile != -1)
			{
				mTempAddTriangles.push_back((int)TT_RIGHT_TOP);
			}
			else if (rightTile != -1 && bottomTile != -1)
			{
				mTempAddTriangles.push_back((int)TT_RIGHT_BOTTOM);
			}
			else if (leftTile != -1 && bottomTile != -1)
			{
				mTempAddTriangles.push_back((int)TT_LEFT_BOTTOM);
			}
		}
		// 有三个地砖,补充2个三角形
		else if (blockCount == 3)
		{
			// 只缺左边的,则补充右边的两个三角形
			if (leftTile == -1)
			{
				mTempAddTriangles.push_back((int)TT_RIGHT_BOTTOM);
				mTempAddTriangles.push_back((int)TT_RIGHT_TOP);
			}
			else if (rightTile == -1)
			{
				mTempAddTriangles.push_back((int)TT_LEFT_BOTTOM);
				mTempAddTriangles.push_back((int)TT_LEFT_TOP);
			}
			else if (topTile == -1)
			{
				mTempAddTriangles.push_back((int)TT_LEFT_BOTTOM);
				mTempAddTriangles.push_back((int)TT_RIGHT_BOTTOM);
			}
			else if (bottomTile == -1)
			{
				mTempAddTriangles.push_back((int)TT_LEFT_TOP);
				mTempAddTriangles.push_back((int)TT_RIGHT_TOP);
			}
		}
		// 有三个以上地砖,补充8个三角形
		else
		{
			for (int i = 0; i < (int)TT_MAX; ++i)
			{
				mTempAddTriangles.push_back(i);
			}
		}
		if (!mTriangleList.contains(tileIndex))
		{
			mTriangleList.insert(tileIndex, txVector<int>());
		}
		// 设置组ID,将三角形加入到不可行走区域中
		mMapData->mTileList[tileIndex].mUnreachGroupID = mGroupID;
		for (int i = 0; i < mTempAddTriangles.size(); ++i)
		{
			if (!mTriangleList[tileIndex].contains(mTempAddTriangles[i]))
			{
				mTriangleList[tileIndex].push_back(mTempAddTriangles[i]);
			}
		}
		return true;
	}
	return false;
}