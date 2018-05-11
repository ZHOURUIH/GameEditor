#include "txSceneManager.h"
#include "txEngineRoot.h"
#include "Utility.h"
#include "txCommandHeader.h"
#include "GameScene.h"
#include "SceneProcedure.h"
#include "GameSceneManager.h"
#include "txComponentHeader.h"

GameScene::GameScene(const int& type, const std::string& sceneName)
:
txComponentOwner(sceneName)
{
	mSceneType = type;
	mCurProcedure = NULL;
	mLastProcedureType = -1;
}
// 进入场景时初始化
void GameScene::init()
{
	initComponents();
	// 创建出所有的场景流程
	createSceneProcedure();
	// 设置起始流程名
	assignStartExitProcedure();
	// 开始执行起始流程
	CommandGameSceneChangeProcedure* cmd = NEW_CMD(cmd);
	cmd->mProcedure = mStartProcedure;
	mCommandSystem->pushCommand(cmd, this);
}
void GameScene::initComponents()
{
	// 添加音效组件
	addComponent("audio", TOSTRING(txComponentAudio))->setActive(true);
}
void GameScene::destroy()
{
	// 销毁所有流程
	auto iter = mSceneProcedureList.begin();
	auto iterEnd = mSceneProcedureList.end();
	FOR_STL(mSceneProcedureList, ; iter != iterEnd; ++iter)
	{
		TRACE_DELETE(iter->second);
	}
	END_FOR_STL(mSceneProcedureList);
	mSceneProcedureList.clear();
	mCurProcedure = NULL;
	destroyAllComponents();
}

void GameScene::update(const float& elapsedTime)
{
	// 更新组件
	updateComponents(elapsedTime);

	// 更新当前流程
	keyProcess(elapsedTime);
	if (mCurProcedure != NULL)
	{
		mCurProcedure->update(elapsedTime);
	}
}
void GameScene::keyProcess(const float& elapsedTime)
{
	// 在准备退出当前流程时,不响应任何按键操作
	if (mCurProcedure != NULL && !mCurProcedure->isPreparingExit())
	{
		mCurProcedure->keyProcess(elapsedTime);
	}
}
// 退出场景
void GameScene::exit()
{
	// 首先进入退出流程,然后再退出最后的流程
	CommandGameSceneChangeProcedure* cmd = NEW_CMD(cmd);
	cmd->mProcedure = mExitProcedure;
	mCommandSystem->pushCommand(cmd, this);
	if (mCurProcedure != NULL)
	{
		mCurProcedure->exit(NULL, NULL);
		mCurProcedure = NULL;
	}
}
bool GameScene::atProcedure(const int& type)
{
	if (mCurProcedure == NULL)
	{
		return false;
	}
	return mCurProcedure->isThisOrParent(type);
}

bool GameScene::atSelfProcedure(const int& type)
{
	if (mCurProcedure == NULL)
	{
		return false;
	}
	return mCurProcedure->getProcedureType() == type;
}
void GameScene::prepareChangeProcedure(const int& procedure, const float& time, const std::string& intent)
{
	SceneProcedure* targetProcedure = mSceneProcedureList[procedure];
	mCurProcedure->prepareExit(targetProcedure, time, intent);
}
void GameScene::backToLastProcedure(const std::string& intend)
{
	if (mLastProcedureList.size() == 0)
	{
		return;
	}
	// 获得上一次进入的流程
	int lastType = getLastProcedureType();
	changeProcedure(lastType, intend, false);
	mLastProcedureList.erase(mLastProcedureList.size() - 1);
}
bool GameScene::changeProcedure(const int& procedure, const std::string& intent, const bool& addToLastList)
{
	if (mSceneProcedureList.contains(procedure))
	{
		// 将上一个流程记录到返回列表中
		if (mCurProcedure != NULL && addToLastList)
		{
			mLastProcedureList.push_back(mCurProcedure);
			if (mLastProcedureList.size() > mMaxLastProcedureCount)
			{
				mLastProcedureList.erase(0);
			}
		}
		if (mCurProcedure == NULL || mCurProcedure->getProcedureType() != procedure)
		{
			SceneProcedure* targetProcedure = mSceneProcedureList[procedure];
			// 如果当前已经在一个流程中了,则要先退出当前流程,但是不要销毁流程
			if (mCurProcedure != NULL)
			{
				// 需要找到共同的父节点,退到该父节点时则不再退出
				SceneProcedure* exitTo = mCurProcedure->getSameParent(targetProcedure);
				SceneProcedure* nextPro = targetProcedure;
				mCurProcedure->exit(exitTo, nextPro);
			}
			SceneProcedure* lastProcedure = mCurProcedure;
			mCurProcedure = targetProcedure;
			mCurProcedure->init(lastProcedure, intent);
		}
		return true;
	}
	else
	{
		ENGINE_ERROR("can not find scene procedure : %d", procedure);
	}
	return false;
}
// 流程调用,通知场景当前流程已经准备完毕
void GameScene::notifyProcedurePrepared()
{
	if (mLastProcedureList.size() > 0)
	{
		mLastProcedureList[mLastProcedureList.size() - 1]->onNextProcedurePrepared(mCurProcedure);
	}
}
//  获取上一个流程
int GameScene::getLastProcedureType()
{
	if (mLastProcedureList.size() > 0)
	{
		return mLastProcedureList[mLastProcedureList.size() - 1]->getProcedureType();
	}
	return -1;
}
SceneProcedure* GameScene::getSceneProcedure(const int& type)
{
	auto iter = mSceneProcedureList.find(type);
	if (iter != mSceneProcedureList.end())
	{
		return iter->second;
	}
	return NULL;
}