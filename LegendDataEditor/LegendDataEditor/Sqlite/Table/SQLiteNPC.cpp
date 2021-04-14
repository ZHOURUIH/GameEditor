#include "SQLiteNPC.h"

myVector<TDNPC*> SQLiteNPC::mEmptyList;

const myVector<TDNPC*>& SQLiteNPC::querySceneNPC(int sceneID)
{
	auto curDataList = mSceneNPCList.get(sceneID);
	if (curDataList != NULL)
	{
		return *curDataList;
	}
	myVector<TDNPC*> dataList;
	auto& allData = queryAll();
	FOREACH_CONST(iter, allData)
	{
		if (iter->second->mMapID != sceneID)
		{
			continue;
		}
		dataList.push_back(iter->second);
	}
	return mSceneNPCList.tryInsert(sceneID, dataList);
}