#include "txActionTreeManager.h"
#include "txActionTree.h"
#include "Utility.h"
#include "txEngineLog.h"

int txActionTreeManager::mActionNameSeed = 0;

txActionTreeManager::txActionTreeManager()
:
mActionCount(0)
{
	;
}

void txActionTreeManager::update(float elapsedTime)
{
	auto iter = mActionTreeList.begin();
	auto iterEnd = mActionTreeList.end();
	FOR_STL(mActionTreeList, ; iter != iterEnd; ++iter)
	{
		iter->second->update(elapsedTime);
	}
	END_FOR_STL(mActionTreeList);
}

void txActionTreeManager::destroy()
{
	auto iter = mActionTreeList.begin();
	auto iterEnd = mActionTreeList.end();
	FOR_STL(mActionTreeList, ; iter != iterEnd; ++iter)
	{
		TRACE_DELETE(iter->second);
	}
	END_FOR_STL(mActionTreeList);
	mActionTreeList.clear();
	if (mActionCount != 0)
	{
		ENGINE_ERROR("error : all actions have not been destroied completely!");
	}
}

void txActionTreeManager::destroyAction(txAction* action)
{
	auto& childList = action->getChildList();
	auto iter = childList.begin();
	auto iterEnd = childList.end();
	FOR_STL(childList, ; iter != iterEnd; ++iter)
	{
		destroyAction(*iter);
	}
	END_FOR_STL(childList);
	TRACE_DELETE(action);
	--mActionCount;
}

txActionTree* txActionTreeManager::createActionTree(const std::string& name, const bool& createRoot)
{
	std::string treeName = name;
	if (treeName == EMPTY_STRING)
	{
		treeName = "ActionTree" + txStringUtility::intToString(mActionNameSeed);
	}
	++mActionNameSeed;
	txActionTree* actionTree = newActionTree(treeName);
	actionTree->init(createRoot);
	mActionTreeList.insert(actionTree->getName(), actionTree);
	return actionTree;
}

txActionTree* txActionTreeManager::newActionTree(const std::string& name)
{
	txActionTree* actionTree = TRACE_NEW(txActionTree, actionTree, name);
	return actionTree;
}

bool txActionTreeManager::destroyActionTree(txActionTree* actionTree)
{
	if (actionTree == NULL)
	{
		return false;
	}
	auto iter = mActionTreeList.find(actionTree->getName());
	if (iter != mActionTreeList.end())
	{
		TRACE_DELETE(actionTree);
		mActionTreeList.erase(iter);
		return true;
	}
	ENGINE_ERROR("error : can not find action tree! can not destroy it!");
	return false;
}

bool txActionTreeManager::destroyActionTree(const std::string& name)
{
	return destroyActionTree(getActionTree(name));
}

txActionTree* txActionTreeManager::getActionTree(const std::string& name)
{
	auto iterTree = mActionTreeList.find(name);
	if (iterTree != mActionTreeList.end())
	{
		return iterTree->second;
	}
	return NULL;
}