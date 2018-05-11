#include "txBillboardManager.h"
#include "txBillboard.h"

txBillboardManager::txBillboardManager()
{
	;
}

txBillboardManager::~txBillboardManager()
{
	ClearUselessBillboard();
	DestroyAllBillboard();
}

void txBillboardManager::update(float elapsedTime)
{
	ClearUselessBillboard();
}

txBillboard* txBillboardManager::CreateBillboard(const char* name, const VECTOR3& pos, const float& width, const float& height, 
	const char* textureName, const int& frameCount, txCamera* pCamera, txNode* parentNode, const float& duration)
{
	if(mBillboardMap.find(name) != mBillboardMap.end())
	{
		return NULL;
	}
	txBillboard* billboard = TRACE_NEW(txBillboard, billboard, name);
	billboard->init(pCamera);
	billboard->setParentNode(parentNode);
	billboard->setPosition(pos);
	billboard->setSize(VECTOR2(width, height));
	billboard->createVertex();
	billboard->setTextureName(textureName, frameCount);
	billboard->setDuration(duration);
	mBillboardMap.insert(std::string(name), billboard);
	return billboard;
}

txBillboard* txBillboardManager::GetBillboard(const char* name)
{
	if(mBillboardMap.size() == 0)
	{
		return NULL;
	}
	auto iter = mBillboardMap.find(name);
	if(iter == mBillboardMap.end())
	{
		return NULL;
	}
	return iter->second;
}

void txBillboardManager::StopAllBillboard()
{
	auto iter = mBillboardMap.begin();
	auto iterEnd = mBillboardMap.end();
	FOR_STL(mBillboardMap,; iter != iterEnd; ++iter)
	{
		txBillboard* bill = iter->second;
		if(bill != NULL)
		{
			bill->stop();
		}
	}
	END_FOR_STL(mBillboardMap);
}

void txBillboardManager::DestroyBillboard(const char* name)
{
	if(mBillboardMap.size() == 0)
	{
		return;
	}
	auto iter = mBillboardMap.find(name);
	if(iter == mBillboardMap.end())
	{
		return;
	}
	iter->second->stop();
	mDestroyBillboardList.push_back(iter->second);
	mBillboardMap.erase(iter);
}

void txBillboardManager::DestroyAllBillboard()
{
	if(mBillboardMap.size() == 0)
	{
		return;
	}
	auto iter = mBillboardMap.begin();
	auto iterEnd = mBillboardMap.end();
	FOR_STL(mBillboardMap,; iter != iterEnd; ++iter)
	{
		TRACE_DELETE(iter->second);
	}
	END_FOR_STL(mBillboardMap);
	mBillboardMap.clear();
}

void txBillboardManager::RenderAllBillboard()
{
	if(mBillboardMap.size() == 0)
	{
		return;
	}
	auto iter = mBillboardMap.begin();
	auto iterEnd = mBillboardMap.end();
	FOR_STL(mBillboardMap, ; iter != iterEnd; ++iter)
	{
		iter->second->render();
	}
	END_FOR_STL(mBillboardMap);
}

void txBillboardManager::ClearUselessBillboard()
{
	int listSize = mDestroyBillboardList.size();
	FOR_STL(mDestroyBillboardList, int i = 0; i < listSize; ++i)
	{
		TRACE_DELETE(mDestroyBillboardList[i]);
	}
	END_FOR_STL(mDestroyBillboardList);
	mDestroyBillboardList.clear();
}