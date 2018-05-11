#ifndef _TX_ACTION_TREE_MANAGER_H_
#define _TX_ACTION_TREE_MANAGER_H_

#include "txAction.h"

class txActionTree;
class txActionTreeManager
{
public:
	txActionTreeManager();
	virtual ~txActionTreeManager(){ destroy(); }
	virtual void init(){}
	virtual void update(float elapsedTime);
	void destroy();
	void destroyAction(txAction* action);
	virtual txActionTree* createActionTree(const std::string& name = EMPTY_STRING, const bool& createRoot = true);
	bool destroyActionTree(txActionTree* actionTree);
	bool destroyActionTree(const std::string& name);
	txActionTree* getActionTree(const std::string& name);
	txMap<std::string, txActionTree*>& getActionTreeList(){ return mActionTreeList; }
protected:
	virtual txActionTree* newActionTree(const std::string& name = EMPTY_STRING);
protected:
	txMap<std::string, txActionTree*> mActionTreeList;
	int mActionCount;		// 行为计数
	static int mActionNameSeed;
};

#endif