#ifndef _SQLITE_SCENE_MAP_SUB_REGION_H_
#define _SQLITE_SCENE_MAP_SUB_REGION_H_

#include "TDSceneMapSubRegion.h"

class SQLiteSceneMapSubRegion : public SQLiteTable<TDSceneMapSubRegion>
{
public:
	const myVector<TDSceneMapSubRegion*>& querySceneSubRegion(int sceneID);
protected:
	myMap<int, myVector<TDSceneMapSubRegion*>> mSceneSubRegionList;
private:
	static myVector<TDSceneMapSubRegion*> mEmptyList;
};

#endif