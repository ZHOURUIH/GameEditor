#include "SQLiteSceneMapTransfer.h"

myVector<TDSceneMapTransfer*> SQLiteSceneMapTransfer::mEmptyList;

const myVector<TDSceneMapTransfer*>& SQLiteSceneMapTransfer::querySceneTransfer(int sceneID)
{
	auto curDataList = mSceneTransferList.get(sceneID);
	if (curDataList != NULL)
	{
		return *curDataList;
	}
	myVector<TDSceneMapTransfer*> dataList;
	auto& allData = queryAll();
	FOREACH_CONST(iter, allData)
	{
		if (iter->second->mSourceMap != sceneID)
		{
			continue;
		}
		dataList.push_back(iter->second);
	}
	return mSceneTransferList.tryInsert(sceneID, dataList);
}