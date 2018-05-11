#include "Utility.h"
#include "txTremblingManager.h"
#include "txTrembling.h"

int txTremblingManager::mTremblingNameSeed = 0;

void txTremblingManager::loadAll()
{
	txVector<std::string> fileList;
	txFileUtility::findFiles(P_TREMBLING_PATH, fileList, TREMBLING_SUFFIX);
	int size = fileList.size();
	FOR_STL(fileList, int i = 0; i < size; ++i)
	{
		loadTrembling(fileList[i]);
	}
	END_FOR_STL(fileList);
}

void txTremblingManager::destroy()
{
	auto itr = mTremblingList.begin();
	auto itrEnd = mTremblingList.end();
	FOR_STL(mTremblingList, ; itr != itrEnd; ++itr)
	{
		TRACE_DELETE(itr->second);
	}
	END_FOR_STL(mTremblingList);
	mTremblingList.clear();
}

txTrembling* txTremblingManager::createTrembling(const std::string& name)
{
	if (getTrembling(name) != NULL)
	{
		return NULL;
	}
	std::string newName = name;
	if (newName == EMPTY_STRING)
	{
		newName = "Trembling" + txStringUtility::intToString(mTremblingNameSeed++);
	}
	txTrembling* trembling = TRACE_NEW(txTrembling, trembling, newName);
	mTremblingList.insert(trembling->getName(), trembling);
	return trembling;
}

txTrembling* txTremblingManager::loadTrembling(const std::string& fullFileName)
{
	std::string tremblingName = txStringUtility::getFileNameNoSuffix(fullFileName);
	txTrembling* trembling = TRACE_NEW(txTrembling, trembling, tremblingName);
	if (!trembling->init(fullFileName))
	{
		TRACE_DELETE(trembling);
	}
	else
	{
		mTremblingList.insert(trembling->getName(), trembling);
	}
	++mTremblingNameSeed;
	return trembling;
}

txTrembling* txTremblingManager::getTrembling(const std::string& name)
{
	auto iter = mTremblingList.find(name);
	if (iter == mTremblingList.end())
	{
		return NULL;
	}
	return iter->second;
}