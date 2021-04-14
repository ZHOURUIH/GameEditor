#include "SQLiteSceneMapSubRegion.h"

myVector<TDSceneMapSubRegion*> SQLiteSceneMapSubRegion::mEmptyList;

const myVector<TDSceneMapSubRegion*>& SQLiteSceneMapSubRegion::querySceneSubRegion(int sceneID)
{
	auto curDataList = mSceneSubRegionList.get(sceneID);
	if (curDataList != NULL)
	{
		return *curDataList;
	}
	myVector<TDSceneMapSubRegion*> dataList;
	auto& allData = queryAll();
	FOREACH_CONST(iter, allData)
	{
		if (iter->second->mMapID != sceneID)
		{
			continue;
		}
		dataList.push_back(iter->second);
	}
	return mSceneSubRegionList.tryInsert(sceneID, dataList);
}