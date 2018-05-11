#include <time.h>
#include <stdlib.h>

#include "txConfig.h"
#include "txDataManager.h"

#include "Utility.h"
#include "GameFramework.h"
#include "GameConfig.h"
#include "txMemeryTrace.h"

template<>GameFramework *txSingleton<GameFramework>::ms_Singleton = 0;

GameFramework::GameFramework()
	:
	mGameConfig(NULL)
{
	// 设置随机数种子
	srand((unsigned int)time(0));
}

GameFramework::~GameFramework()
{
	destroy();
}

bool GameFramework::init()
{
	// 创建所有组件
	mGameConfig = TRACE_NEW(GameConfig, mGameConfig);
	mDataManager = TRACE_NEW(txDataManager, mDataManager);

	mDataManager->init();
	mGameConfig->init();
	return true;
}

void GameFramework::update(float elapsedTime)
{

}

void GameFramework::render()
{
}

void GameFramework::destroy()
{
	TRACE_DELETE(mGameConfig);
	TRACE_DELETE(mDataManager)
}