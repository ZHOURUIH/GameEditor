#include "txActionTree.h"
#include "txAction.h"
#include "txActionTreeManager.h"
#include "txActionFactory.h"
#include "txActionFactoryManager.h"

void txActionTree::update(float elapsedTime)
{
	// 由行为树来决定当前执行哪个行为
	if (mTreeState == PS_PLAY)
	{
		// 根据时间线判断是否该执行哪个行为,开始执行后,再累计时间
		// 找到时间深度小于等于当前时间线,并且还没有执行的行为
		// first表示行为正向或者反向开始执行时的时间深度到当前时间线的距离
		txMap<float, txVector<txAction*> > actionList;
		mRootAction->getNeedExecuteAction(actionList, mTimeLine, mTimeDirction);
		if (actionList.size() > 0)
		{
			auto iterAction = actionList.begin();
			auto iterActionEnd = actionList.end();
			FOR_STL(actionList, ; iterAction != iterActionEnd; ++iterAction)
			{
				auto& actionLine = iterAction->second;
				int actionCount = actionLine.size();
				FOR_STL(actionLine, int i = 0; i < actionCount; ++i)
				{
					txAction* action = actionLine[i];
					action->execute(std::abs(mTimeLine - (mTimeDirction ? action->getTimeDepth() : action->getExecuteDoneTimeDepth())), mTimeDirction);
				}
				END_FOR_STL(actionLine);
			}
			END_FOR_STL(actionList);
		}
		// 更新所有正在执行的行为,此处复制一个列表,避免在更新过程中对mExecutingAction进行操作而使迭代器失效
		auto executingActionList = mExecutingAction;
		auto iterExecuting = executingActionList.begin();
		auto iterExecutingEnd = executingActionList.end();
		FOR_STL(executingActionList, ; iterExecuting != iterExecutingEnd; ++iterExecuting)
		{
			(*iterExecuting)->update(elapsedTime);
		}
		END_FOR_STL(executingActionList);
		mTimeLine += elapsedTime * (mTimeDirction ? 1.0f : -1.0f);
	}
}

void txActionTree::destroy()
{
	// 复制一份列表,避免在销毁时迭代器失效
	if (mRootAction != NULL)
	{
		mActionTreeManager->destroyAction(mRootAction);
		mRootAction = NULL;
	}
	mActionList.clear();
	mExecutingAction.clear();
}

txAction* txActionTree::getAction(const std::string& name)
{
	auto iterAction = mActionList.find(name);
	if (iterAction != mActionList.end())
	{
		return iterAction->second;
	}
	return NULL;
}

txAction* txActionTree::addRoot()
{
	if (mRootAction != NULL)
	{
		ENGINE_ERROR("error : there is a root action! can not add again!");
		return mRootAction;
	}
	txAction* action = addAction(TOSTRING(txAction), "root", NULL);
	return action;
}

void txActionTree::setTreeState(const PLAY_STATE& state)
{
	if (state == PS_PLAY)
	{
		play();
	}
	else if (state == PS_PAUSE)
	{
		pause();
	}
}

void txActionTree::play(const bool& direction)
{
	if (mTreeState == PS_PLAY)
	{
		return;
	}
	if (mRootAction == NULL)
	{
		ENGINE_ERROR("error : no root action, can not play action tree!");
		return;
	}
	mTimeDirction = direction;
	// 如果是从暂停状态开始播放,则需要将所有在执行列表中的被暂停的行为恢复执行
	if (mTreeState == PS_PAUSE)
	{
		auto iterAction = mExecutingAction.begin();
		auto iterActionEnd = mExecutingAction.end();
		FOR_STL(mExecutingAction, ; iterAction != iterActionEnd; ++iterAction)
		{
			(*iterAction)->resume(mTimeDirction);
		}
		END_FOR_STL(mExecutingAction);
	}
	// 将位于时间线前方的行为全部设置为未执行状态
	mRootAction->resetStateToNotExecute(mTimeLine, mTimeDirction);
	mTreeState = PS_PLAY;

	// 得到最后一个执行完毕的行为,通知该行为在执行完毕时应该通知行为树,只有正向执行时才会有,反向执行时,0时间深度为终止
	if (direction)
	{
		mDeepestAction = mRootAction;
		mRootAction->getDeepestExecuteDoneAction(mDeepestAction);
	}
	else
	{
		mDeepestAction = mRootAction;
	}
}

void txActionTree::pause()
{
	mTreeState = PS_PAUSE;
	// 需要把所有正在执行的行为都暂停
	auto iterAction = mExecutingAction.begin();
	auto iterActionEnd = mExecutingAction.end();
	FOR_STL(mExecutingAction, ; iterAction != iterActionEnd; ++iterAction)
	{
		(*iterAction)->pause();
	}
	END_FOR_STL(mExecutingAction);
}

void txActionTree::stop()
{
	mTreeState = PS_STOP;
	mTimeLine = 0.0f;
	// 从根行为开始把所有行为停止,此处有问题需要修改!在行为树正在执行过程中停止行为树,然后再次开始执行时,执行过程会有错误
	if (mRootAction != NULL)
	{
		mRootAction->stop();
	}
}

void txActionTree::complete()
{
	// 时间线校正为最后一个行为执行完毕的时间深度,状态恢复初始状态
	if (mDeepestAction != NULL)
	{
		mTimeLine = mDeepestAction->getExecuteDoneTimeDepth();
	}
	else
	{
		mTimeLine = 0.0f;
	}
	mTreeState = PS_STOP;
}

txAction* txActionTree::createIndependentAction(const std::string& type, const std::string& name)
{
	txActionFactoryBase* factory = mActionFactoryManager->getFactory(type);
	if (factory != NULL)
	{
		return factory->createAction(name);
	}
	return NULL;
}

txAction* txActionTree::addAction(const std::string& type, const std::string& name, txAction* parentAction)
{
	if (parentAction == NULL && mRootAction != NULL)
	{
		return NULL;
	}
	txAction* action = createIndependentAction(type, name);
	if (action == NULL)
	{
		return NULL;
	}
	action->init(this);
	action->setDefaultParam();
	addAction(action, parentAction);
	return action;
}

void txActionTree::addAction(txAction* action, txAction* parent)
{
	if (mTreeState != PS_STOP)
	{
		ENGINE_ERROR("error : can not add action during playing actions!");
		return;
	}
	// 如果想要将当前添加的行为作为根行为
	if (parent == NULL)
	{
		if (mRootAction == NULL)
		{
			mRootAction = action;
		}
		else
		{
			ENGINE_ERROR("error : there is a root action! can not set again!");
		}
	}
	else
	{
		parent->addChild(action);
	}
	mActionList.insert(action->getName(), action);
}

bool txActionTree::deleteAction(const std::string& name)
{
	if (mTreeState != PS_STOP)
	{
		ENGINE_ERROR("error : can not destroy action during playing actions!");
		return false;
	}
	txAction* action = getAction(name);
	if (action == NULL)
	{
		return false;
	}
	auto iterAction = mActionList.find(name);
	if (iterAction != mActionList.end())
	{
		mActionTreeManager->destroyAction(action);
		mActionList.erase(iterAction);
		return true;
	}
	return false;
}

void txActionTree::notifyActionDestroied(txAction* action)
{
	if (action == NULL)
	{
		return;
	}
	if (mRootAction == action)
	{
		mRootAction = NULL;
	}
	auto iterAction = mActionList.find(action->getName());
	if (iterAction != mActionList.end())
	{
		mActionList.erase(iterAction);
	}
	auto iterExecutingAction = mExecutingAction.find(action);
	if (iterExecutingAction != mExecutingAction.end())
	{
		mExecutingAction.erase(iterExecutingAction);
	}
}