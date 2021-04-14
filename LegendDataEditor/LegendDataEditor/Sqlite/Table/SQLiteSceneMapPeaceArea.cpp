#include "SQLiteSceneMapPeaceArea.h"

const myVector<TDSceneMapPeaceArea*>& SQLiteSceneMapPeaceArea::queryScenePeachArea(int sceneID)
{
	auto curDataList = mScenePeaceAreaList.get(sceneID);
	if (curDataList != NULL)
	{
		return *curDataList;
	}
	myVector<TDSceneMapPeaceArea*> dataList;
	auto& allData = queryAll();
	FOREACH_CONST(iter, allData)
	{
		if (iter->second->mMapID != sceneID)
		{
			continue;
		}
		dataList.push_back(iter->second);
	}
	return mScenePeaceAreaList.tryInsert(sceneID, dataList);
}