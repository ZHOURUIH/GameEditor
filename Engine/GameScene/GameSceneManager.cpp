#include "GameScene.h"
#include "GameSceneManager.h"
#include "GameSceneFactory.h"
#include "GameSceneFactoryManager.h"
#include "SceneProcedure.h"

GameSceneManager::GameSceneManager()
:
txCommandReceiver("GameSceneManager"),
mCurScene(NULL)
{
	mGameSceneFactoryManager = TRACE_NEW(GameSceneFactoryManager, mGameSceneFactoryManager);
}

void GameSceneManager::init()
{
	mGameSceneFactoryManager->init();
}

bool GameSceneManager::enterScene(const int& type)
{
	// 查找工厂实例
	GameSceneFactoryBase* factory = mGameSceneFactoryManager->getFactory(type);
	if(factory == NULL)
	{
		return false;
	}
	GameScene* pScene = factory->createScene();
	// 如果有上一个场景,则先销毁上一个场景,只是暂时保存下上个场景的指针,然后在更新中将场景销毁
	if(mCurScene != NULL)
	{
		// 先退出当前场景的流程
		mCurScene->exit();
		mLastSceneList.push_back(mCurScene);
		mCurScene = NULL;
	}
	mCurScene = pScene;
	mCurScene->init();
	return true;
}

void GameSceneManager::update(float elapsedTime)
{
	// 如果上一个场景不为空,则将上一个场景销毁
	if (mLastSceneList.size() != 0)
	{
		// 查找场景的工厂
		int listSize = mLastSceneList.size();
		FOR_STL(mLastSceneList, int i = 0; i < listSize; ++i)
		{
			TRACE_DELETE(mLastSceneList[i]);
		}
		END_FOR_STL(mLastSceneList);
		mLastSceneList.clear();
	}
	if(mCurScene != NULL)
	{
		mCurScene->update(elapsedTime);
	}
}

void GameSceneManager::destroy()
{
	// 如果上一个场景不为空,则将上一个场景销毁
	if (mLastSceneList.size() != 0)
	{
		// 查找场景的工厂
		int listSize = mLastSceneList.size();
		FOR_STL(mLastSceneList, int i = 0; i < listSize; ++i)
		{
			TRACE_DELETE(mLastSceneList[i]);
		}
		END_FOR_STL(mLastSceneList);
		mLastSceneList.clear();
	}
	TRACE_DELETE(mCurScene);
	TRACE_DELETE(mGameSceneFactoryManager);
}