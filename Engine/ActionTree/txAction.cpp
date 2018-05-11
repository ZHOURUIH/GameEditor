#include "txAction.h"
#include "txActionTree.h"
#include "txMathUtility.h"
#include "Utility.h"

void txAction::update(float elapsedTime)
{
	if (mActionState == AES_EXECUTING)
	{
		mTimeCount += elapsedTime;
		if (mTimeCount >= mExecuteTime)
		{
			mTimeCount = 0.0f;
			setActionState(AES_EXECUTE_DONE);
		}
	}
}

void txAction::destroy()
{
	if (mParentActionTree != NULL)
	{
		mParentActionTree->notifyActionDestroied(this);
	}
	if (mParentAction != NULL)
	{
		mParentAction->notifyChildDestroied(this);
	}
	int childCount = mChildActionList.size();
	FOR_STL(mChildActionList, int i = 0; i < childCount; ++i)
	{
		mChildActionList[i]->notifyParentDestroied();
	}
	END_FOR_STL(mChildActionList);
	mChildActionList.clear();
	mChildActionSet.clear();
	mParentAction = NULL;
	mParentActionTree = NULL;
	mActionState = AES_NOT_EXECUTE;
	mStartCallback = NULL;
	mStartUserData = NULL;
	mDoneCallback = NULL;
	mDoneUserData = NULL;
	mInverseStartCallback = NULL;
	mInverseStartUserData = NULL;
	mInverseDoneCallback = NULL;
	mInverseDoneUserData = NULL;
}

void txAction::pause()
{
	setActionState(AES_PAUSE);
	pauseAction();
}

void txAction::resume(const bool& direciton)
{
	mDirection = direciton;
	setActionState(AES_EXECUTING);
	resumeAction(direciton);
}

void txAction::recover()
{
	setActionState(AES_NOT_EXECUTE);
	recoverAction();
}

void txAction::stop()
{
	// 后序遍历所有行为,从最后正在执行的行为开始往上停止恢复执行前的状态
	// 遍历子行为
	int childCount = mChildActionList.size();
	FOR_STL(mChildActionList, int i = 0; i < childCount; ++i)
	{
		mChildActionList[i]->stop();
	}
	END_FOR_STL(mChildActionList);
	// 无论行为处于什么状态,都需要恢复并停止,确保整个行为树的状态是正确的
	recover();
	stopAction();
	if (mParentActionTree)
	{
		mParentActionTree->notifyActionStoped(this);
	}
}

void txAction::execute(const float& timeOffset, const bool& direction)
{
	mTimeCount = timeOffset;
	mDirection = direction;
	setActionState(AES_EXECUTING);
}

void txAction::setActionState(const ACTION_EXECUTE_STATE& state)
{
	mActionState = state;
	if (mActionState == AES_EXECUTE_DONE)
	{
		// 通知行为树,当前行为已经执行完毕
		if (mParentActionTree != NULL)
		{
			mParentActionTree->notifyActionExecutingDone(this);
		}
		// 调用行为执行完的回调
		if (mDirection)
		{
			if (mDoneCallback != NULL)
			{
				mDoneCallback(this, mDoneUserData);
			}
		}
		else
		{
			if (mInverseDoneCallback != NULL)
			{
				mInverseDoneCallback(this, mInverseDoneUserData);
			}
		}
	}
	else if (mActionState == AES_EXECUTING)
	{
		// 调用行为开始执行的回调
		if (mDirection)
		{
			if (mStartCallback != NULL)
			{
				mStartCallback(this, mStartUserData);
			}
		}
		else
		{
			if (mInverseStartCallback != NULL)
			{
				mInverseStartCallback(this, mInverseStartUserData);
			}
		}
		// 通知行为树,当前行为正在执行
		if (mParentActionTree != NULL)
		{
			mParentActionTree->notifyActionExecuting(this);
		}
	}
}

void txAction::addChild(txAction* child)
{
	if (child->getParent() != NULL)
	{
		ENGINE_ERROR("error : action has parent! can not attach to other action!");
		return;
	}
	auto iterAction = mChildActionSet.find(child);
	if (iterAction == mChildActionSet.end())
	{
		// 设置父行为
		child->setParent(this);
		mChildActionSet.insert(child);
		mChildActionList.push_back(child);
	}
}

void txAction::removeChild(txAction* child)
{
	auto iterAction = mChildActionSet.find(child);
	if (iterAction == mChildActionSet.end())
	{
		return;
	}
	mChildActionSet.erase(iterAction);
	int childPos = -1;
	auto iterChild = mChildActionList.begin();
	auto iterChildEnd = mChildActionList.end();
	FOR_STL(mChildActionList, int i = 0; iterChild != iterChildEnd; ++iterChild)
	{
		if (*iterChild == child)
		{
			// 清空父行为
			child->setParent(NULL);
			childPos = i;
			break;
		}
		++i;
	}
	END_FOR_STL(mChildActionList);
	if (childPos >= 0)
	{
		mChildActionList.erase(mChildActionList.begin() + childPos);
	}
}

float txAction::getMaxExecuteDoneTimeDepth()
{
	int childCount = mChildActionList.size();
	// 有子行为,则返回子行为中最大执行完毕时的时间深度
	if (childCount > 0)
	{
		float maxDepth = 0.0f;
		FOR_STL(mChildActionList, int i = 0; i < childCount; ++i)
		{
			float doneTimeDepth = mChildActionList[i]->getMaxExecuteDoneTimeDepth();
			maxDepth = txMath::getMax(doneTimeDepth, maxDepth);
		}
		END_FOR_STL(mChildActionList);
		return maxDepth;
	}
	// 没有子行为,返回自己的执行完毕时的时间深度
	else
	{
		return mExecuteDoneTimeDepth;
	}
}

void txAction::resetStateToNotExecute(const float& timeLine, const bool& direction)
{
	// 正向查找,将时间深度大于等于时间线的行为都设置为未执行的
	if (direction)
	{
		int childCount = mChildActionList.size();
		FOR_STL(mChildActionList, int i = 0; i < childCount; ++i)
		{
			mChildActionList[i]->resetStateToNotExecute(timeLine, direction);
		}
		END_FOR_STL(mChildActionList);
		if (mTimeDepth >= timeLine)
		{
			setActionState(AES_NOT_EXECUTE);
		}
	}
	// 反向查找,将执行完毕的时间深度小于等于时间线的行为都设置为未执行
	else
	{
		// 执行完毕的时间深度大于了时间线,则不用再遍历了
		if (mExecuteDoneTimeDepth > timeLine)
		{
			return;
		}
		int childCount = mChildActionList.size();
		FOR_STL(mChildActionList, int i = 0; i < childCount; ++i)
		{
			mChildActionList[i]->resetStateToNotExecute(timeLine, direction);
		}
		END_FOR_STL(mChildActionList);
		// 直接设置即可,前面已经判断出了
		setActionState(AES_NOT_EXECUTE);
	}
}

void txAction::getNeedExecuteAction(txMap<float, txVector<txAction*> >& actionTimeMap, const float& timeLine, const bool& direction)
{
	// 正向查找时,当前行为的时间深度已经大于时间线,直接返回,不用再递归遍历了
	if ((direction && mTimeDepth > timeLine))
	{
		return;
	}
	if (!direction && mActionState != AES_NOT_EXECUTE)
	{
		return;
	}
	// 反向查找时,当前行为正常执行完的时间深度已经大于等于时间线则查找放入
	// 正向查找则直接查找放入(因为前面遇到不符合条件时已经退出递归了)
	if (direction || !direction && mExecuteDoneTimeDepth >= timeLine)
	{
		// 只放入未执行的行为
		if (mActionState == AES_NOT_EXECUTE)
		{
			// timeDistance表示行为正向或者反向开始执行时的时间深度到当前时间线的距离
			float timeDistance = direction ? std::abs(mTimeDepth - timeLine) : std::abs(mExecuteDoneTimeDepth - timeLine);
			auto iterTime = actionTimeMap.find(timeDistance);
			if (iterTime == actionTimeMap.end())
			{
				txVector<txAction*> actionList;
				actionList.push_back(this);
				actionTimeMap.insert(timeDistance, actionList);
			}
			else
			{
				iterTime->second.push_back(this);
			}
		}
	}
	// 遍历子行为
	int childCount = mChildActionList.size();
	FOR_STL(mChildActionList, int i = 0; i < childCount; ++i)
	{
		mChildActionList[i]->getNeedExecuteAction(actionTimeMap, timeLine, direction);
	}
	END_FOR_STL(mChildActionList);
}

void txAction::getDeepestExecuteDoneAction(txAction*& action)
{
	int childCount = mChildActionList.size();
	if (childCount == 0)
	{
		if (mExecuteDoneTimeDepth >= action->getExecuteDoneTimeDepth())
		{
			action = this;
		}
	}
	else
	{
		FOR_STL(mChildActionList, int i = 0; i < childCount; ++i)
		{
			mChildActionList[i]->getDeepestExecuteDoneAction(action);
		}
		END_FOR_STL(mChildActionList);
	}
}

// 由派生类调用设置行为完全执行所需要的时间
void txAction::setExecuteTime(const float& time)
{
	if (txMath::isFloatEqual(mExecuteTime, time))
	{
		return;
	}
	mExecuteTime = time;
	notifyTimeChanged();
}

// 通知该行为父行为的时间或者自己的时间有改变,需要更新自己的时间深度
void txAction::notifyTimeChanged()
{
	// 先刷新自己的时间深度
	refreshTimeDepth();
	// 然后通知自己的所有子节点时间深度改变
	int actionCount = mChildActionList.size();
	FOR_STL(mChildActionList, int i = 0; i < actionCount; ++i)
	{
		mChildActionList[i]->notifyTimeChanged();
	}
	END_FOR_STL(mChildActionList);
}