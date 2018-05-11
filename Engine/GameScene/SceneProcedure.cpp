#include "txCamera.h"
#include "txCommandHeader.h"
#include "SceneProcedure.h"
#include "GameScene.h"
#include "GameCamera.h"
#include "CameraManager.h"

SceneProcedure::SceneProcedure(const int& type, GameScene* gameScene)
{
	mInited = false;
	mProcedureType = type;
	mGameScene = gameScene;
	mParentProcedure = NULL;
	mCurChildProcedure = NULL;
	mCurPrepareTime = -1.0f;
	mExitTime = -1.0f;
	mPrepareNext = NULL;
	mPrepareIntent = EMPTY_STRING;
}
void SceneProcedure::init(SceneProcedure* lastProcedure, const std::string& intent)
{
	// 如果父节点还没有初始化,则先初始化父节点
	if (mParentProcedure != NULL && !mParentProcedure->mInited)
	{
		mParentProcedure->init(lastProcedure, intent);
	}
	// 再初始化自己,如果是从子节点返回到父节点,则需要调用另外一个初始化函数
	if (lastProcedure != NULL && lastProcedure->isThisOrParent(mProcedureType))
	{
		onInitFromChild(lastProcedure, intent);
	}
	else
	{
		onInit(lastProcedure, intent);
	}
	mInited = true;
}
void SceneProcedure::update(const float& elapsedTime)
{
	// 先更新父节点
	if (mParentProcedure != NULL)
	{
		mParentProcedure->update(elapsedTime);
	}
	// 再更新自己
	onUpdate(elapsedTime);

	// 正在准备退出流程时,累计时间,
	if (mCurPrepareTime >= 0.0f)
	{
		mCurPrepareTime += elapsedTime;
		if (mCurPrepareTime >= mExitTime)
		{
			mExitTime = -1.0f;
			mCurPrepareTime = -1.0f;
			// 超过了准备时间,强制跳转流程
			CommandGameSceneChangeProcedure* cmd = NEW_CMD(cmd);
			cmd->mProcedure = mPrepareNext->getProcedureType();
			cmd->mIntent = mPrepareIntent;
			mCommandSystem->pushCommand(cmd, mGameScene);
		}
	}
}
void SceneProcedure::exit(SceneProcedure* exitTo, SceneProcedure* nextPro)
{
	// 中断自己所有未执行的命令
	auto iter = mDelayCmdList.begin();
	auto iterEnd = mDelayCmdList.end();
	FOR_STL(mDelayCmdList, ; iter != iterEnd; ++iter)
	{
		mCommandSystem->interruptCommand(*iter);
	}
	END_FOR_STL(mDelayCmdList);
	mDelayCmdList.clear();

	// 当停止目标为自己时,则不再退出,此时需要判断当前将要进入的流程是否为当前流程的子流程
	// 如果是,则需要调用onExitToChild,执行退出当前并且进入子流程的操作
	// 如果不是则不需要调用,不需要执行任何退出操作
	if (this == exitTo)
	{
		if (nextPro != NULL && nextPro->isThisOrParent(mProcedureType))
		{
			onExitToChild(nextPro);
			onExitSelf();
		}
		return;
	}
	// 先退出自己
	onExit(nextPro);
	onExitSelf();
	mInited = false;
	// 再退出父节点
	if (mParentProcedure != NULL)
	{
		mParentProcedure->exit(exitTo, nextPro);
	}
	// 退出完毕后就修改标记
	mExitTime = -1.0f;
	mCurPrepareTime = -1.0f;
	mPrepareNext = NULL;
	mPrepareIntent = "";
}
void SceneProcedure::prepareExit(SceneProcedure* next, const float& time, const std::string& intent)
{
	mCurPrepareTime = 0.0f;
	mExitTime = time;
	mPrepareNext = next;
	mPrepareIntent = intent;
	// 通知自己准备退出
	onPrepareExit(next);
}
void SceneProcedure::keyProcess(const float& elapsedTime)
{
	// 先处理父节点按键响应
	if (mParentProcedure != NULL)
	{
		mParentProcedure->keyProcess(elapsedTime);
	}
	// 然后再处理自己的按键响应
	onKeyProcess(elapsedTime);
}
void SceneProcedure::addDelayCmd(txCommand* cmd)
{
	mDelayCmdList.insert(cmd->getAssignID());
	cmd->addStartCommandCallback(onCmdStarted, this);
}
void SceneProcedure::getParentList(txVector<SceneProcedure*>& parentList)
{
	// 由于父节点列表中需要包含自己,所以先加入自己
	parentList.push_back(this);
	// 再加入父节点的所有父节点
	if (mParentProcedure != NULL)
	{
		mParentProcedure->getParentList(parentList);
	}
}
// 获得自己和otherProcedure的共同的父节点
SceneProcedure* SceneProcedure::getSameParent(SceneProcedure* otherProcedure)
{
	// 获得两个流程的父节点列表
	txVector<SceneProcedure*> thisParentList;
	txVector<SceneProcedure*> otherParentList;
	getParentList(thisParentList);
	otherProcedure->getParentList(otherParentList);
	// 从前往后判断,找到第一个相同的父节点
	SceneProcedure* sameParent = NULL;
	int thisCount = thisParentList.size();
	int otherCount = otherParentList.size();
	FOR_STL(thisParentList, int i = 0; i < thisCount; ++i)
	{
		FOR_STL(otherParentList, int j = 0; j < otherCount; ++j)
		{
			if (thisParentList[i] == otherParentList[j])
			{
				sameParent = thisParentList[i];
				i = thisCount;
				break;
			}
		}
		END_FOR_STL(otherParentList);
	}
	END_FOR_STL(thisParentList);
	return sameParent;
}
bool SceneProcedure::isThisOrParent(const int& type)
{
	// 判断是否是自己的类型
	if (mProcedureType == type)
	{
		return true;
	}
	// 判断是否为父节点的类型
	if (mParentProcedure != NULL)
	{
		return mParentProcedure->isThisOrParent(type);
	}
	// 没有父节点,返回false
	return false;
}
SceneProcedure* SceneProcedure::getParent(const int& type)
{
	// 没有父节点,返回null
	if (mParentProcedure == NULL)
	{
		return NULL;
	}
	// 有父节点,则判断类型是否匹配,匹配则返回父节点
	if (mParentProcedure->getProcedureType() == type)
	{
		return mParentProcedure;
	}
	// 不匹配,则继续向上查找
	else
	{
		return mParentProcedure->getParent(type);
	}
}
SceneProcedure* SceneProcedure::getChildProcedure(const int& type)
{
	auto iter = mChildProcedureList.find(type);
	if (iter != mChildProcedureList.end())
	{
		return iter->second;
	}
	return NULL;
}
bool SceneProcedure::addChildProcedure(SceneProcedure* child)
{
	if (child == NULL)
	{
		return false;
	}
	if (mChildProcedureList.contains(child->getProcedureType()))
	{
		return false;
	}
	child->setParent(this);
	mChildProcedureList.insert(child->getProcedureType(), child);
	return true;
}
//---------------------------------------------------------------------------------------------------------
bool SceneProcedure::setParent(SceneProcedure* parent)
{
	if (mParentProcedure != NULL)
	{
		return false;
	}
	mParentProcedure = parent;
	return true;
}
void SceneProcedure::onCmdStarted(txCommand* cmd, void* userdata)
{
	SceneProcedure* procedure = static_cast<SceneProcedure*>(userdata);
	if (!procedure->mDelayCmdList.erase(cmd->getAssignID()))
	{
		ENGINE_ERROR("命令执行后移除流程命令失败");
	}
}