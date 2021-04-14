#ifndef _SQLITE_MONSTER_REGION_H_
#define _SQLITE_MONSTER_REGION_H_

#include "TDMonsterRegion.h"

class SQLiteMonsterRegion : public SQLiteTable<TDMonsterRegion>
{
public:
	const myVector<TDMonsterRegion*>& querySceneMonsterRegion(int sceneID);
protected:
	myMap<int, myVector<TDMonsterRegion*>> mSceneMonsterRegionList;
private:
	static myVector<TDMonsterRegion*> mEmptyList;
};

#endif