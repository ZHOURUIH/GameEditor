#include "SQLiteMonsterRegion.h"

myVector<TDMonsterRegion*> SQLiteMonsterRegion::mEmptyList;

const myVector<TDMonsterRegion*>& SQLiteMonsterRegion::querySceneMonsterRegion(int sceneID)
{
	auto curDataList = mSceneMonsterRegionList.get(sceneID);
	if (curDataList != NULL)
	{
		return *curDataList;
	}
	myVector<TDMonsterRegion*> dataList;
	auto& allData = queryAll();
	FOREACH_CONST(iter, allData)
	{
		if (iter->second->mMapID != sceneID)
		{
			continue;
		}
		dataList.push_back(iter->second);
	}
	return mSceneMonsterRegionList.tryInsert(sceneID, dataList);
}