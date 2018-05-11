#ifndef _TX_TREMBLING_MANAGER_H_
#define _TX_TREMBLING_MANAGER_H_

#include "txEngineDefine.h"

class txTrembling;
class txTremblingManager
{
public:
	txTremblingManager() {}
	virtual ~txTremblingManager() { destroy(); }
	virtual void init() { loadAll(); }
	void destroy();
	void loadAll();
	txTrembling* createTrembling(const std::string& name);
	txTrembling* loadTrembling(const std::string& fullFileName);
	txTrembling* getTrembling(const std::string& name);
	txMap<std::string, txTrembling*>& getTremblingList() { return mTremblingList; }
protected:
	txMap<std::string, txTrembling*> mTremblingList;
	static int mTremblingNameSeed;
};

#endif