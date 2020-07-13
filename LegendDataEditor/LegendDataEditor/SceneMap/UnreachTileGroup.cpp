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
	for (int i = 0; i < (int)TILE_TRIANGLE::MAX; ++i)
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
			mTriangleList[iter->first].erase(mTriangleList[iter->first].find((int)TILE_TRIANGLE::LEFT_TOP));
		}
		// 右边和上边都没有与组中的地砖,则去掉右上角的三角形
		if (rightTileIndex == -1 && topTileIndex == -1)
		{
			mTriangleList[iter->first].erase(mTriangleList[iter->first].find((int)TILE_TRIANGLE::RIGHT_TOP));
		}
		// 右边和下边都没有与组中的地砖,则去掉右下角的三角形
		if (rightTileIndex == -1 && bottomTileIndex == -1)
		{
			mTriangleList[iter->first].erase(mTriangleList[iter->first].find((int)TILE_TRIANGLE::RIGHT_BOTTOM));
		}
		// 左边和下边都没有与组中的地砖,则去掉左下角的三角形
		if (leftTileIndex == -1 && bottomTileIndex == -1)
		{
			mTriangleList[iter->first].erase(mTriangleList[iter->first].find((int)TILE_TRIANGLE::LEFT_BOTTOM));
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
		bool topTile_RightBottomTriangle = topTile != -1 && mTriangleList[topTile].contains((int)TILE_TRIANGLE::RIGHT_BOTTOM);
		bool topTile_LeftBottomTriangle = topTile != -1 && mTriangleList[topTile].contains((int)TILE_TRIANGLE::LEFT_BOTTOM);
		bool bottomTile_RightTopTriangle = bottomTile != -1 && mTriangleList[bottomTile].contains((int)TILE_TRIANGLE::RIGHT_TOP);
		bool bottomTile_LeftTopTriangle = bottomTile != -1 && mTriangleList[bottomTile].contains((int)TILE_TRIANGLE::LEFT_TOP);
		bool leftTile_RightTopTriangle = leftTile != -1 && mTriangleList[leftTile].contains((int)TILE_TRIANGLE::RIGHT_TOP);
		bool leftTile_RightBottomTriangle = leftTile != -1 && mTriangleList[leftTile].contains((int)TILE_TRIANGLE::RIGHT_BOTTOM);
		bool rightTile_LeftTopTriangle = rightTile != -1 && mTriangleList[rightTile].contains((int)TILE_TRIANGLE::LEFT_TOP);
		bool rightTile_LeftBottomTriangle = rightTile != -1 && mTriangleList[rightTile].contains((int)TILE_TRIANGLE::LEFT_BOTTOM);
		// 补充左上角的三角形
		if (topTile_LeftBottomTriangle && leftTile_RightTopTriangle)
		{
			mTempAddTriangles.push_back((int)TILE_TRIANGLE::LEFT_TOP);
		}
		// 补充右上角的三角形
		if (topTile_RightBottomTriangle && rightTile_LeftTopTriangle)
		{
			mTempAddTriangles.push_back((int)TILE_TRIANGLE::RIGHT_TOP);
		}
		// 补充左下角的三角形
		if (bottomTile_LeftTopTriangle && leftTile_RightBottomTriangle)
		{
			mTempAddTriangles.push_back((int)TILE_TRIANGLE::LEFT_BOTTOM);
		}
		// 补充右下角的三角形
		if (bottomTile_RightTopTriangle && rightTile_LeftBottomTriangle)
		{
			mTempAddTriangles.push_back((int)TILE_TRIANGLE::RIGHT_BOTTOM);
		}
		// 如果四个角的三角形都补充了,则把中间的三角形也填充满
		if (mTempAddTriangles.size() == 4)
		{
			mTempAddTriangles.push_back((int)TILE_TRIANGLE::INNER_LEFT_TOP);
			mTempAddTriangles.push_back((int)TILE_TRIANGLE::INNER_RIGHT_TOP);
			mTempAddTriangles.push_back((int)TILE_TRIANGLE::INNER_RIGHT_BOTTOM);
			mTempAddTriangles.push_back((int)TILE_TRIANGLE::INNER_LEFT_BOTTOM);
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