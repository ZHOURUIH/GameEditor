#ifndef _SQLITE_N_P_C_H_
#define _SQLITE_N_P_C_H_

#include "TDNPC.h"

class SQLiteNPC : public SQLiteTable<TDNPC>
{
public:
	const myVector<TDNPC*>& querySceneNPC(int sceneID);
protected:
	myMap<int, myVector<TDNPC*>> mSceneNPCList;
private:
	static myVector<TDNPC*> mEmptyList;
};

#endif