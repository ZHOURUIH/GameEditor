#ifndef _SQLITE_SCENE_MAP_PEACE_AREA_H_
#define _SQLITE_SCENE_MAP_PEACE_AREA_H_

#include "TDSceneMapPeaceArea.h"

class SQLiteSceneMapPeaceArea : public SQLiteTable<TDSceneMapPeaceArea>
{
public:
	const myVector<TDSceneMapPeaceArea*>& queryScenePeachArea(int sceneID);
protected:
	myMap<int, myVector<TDSceneMapPeaceArea*>> mScenePeaceAreaList;
private:
	static myVector<TDSceneMapPeaceArea*> mEmptyList;
};

#endif