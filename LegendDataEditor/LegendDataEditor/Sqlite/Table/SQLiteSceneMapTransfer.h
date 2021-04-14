#ifndef _SQLITE_SCENE_MAP_TRANSFER_H_
#define _SQLITE_SCENE_MAP_TRANSFER_H_

#include "TDSceneMapTransfer.h"

class SQLiteSceneMapTransfer : public SQLiteTable<TDSceneMapTransfer>
{
public:
	const myVector<TDSceneMapTransfer*>& querySceneTransfer(int sceneID);
protected:
	myMap<int, myVector<TDSceneMapTransfer*>> mSceneTransferList;
private:
	static myVector<TDSceneMapTransfer*> mEmptyList;
};

#endif