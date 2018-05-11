#ifndef _TX_ACTION_TREE_H_
#define _TX_ACTION_TREE_H_

#include "txEngineBase.h"
#include "txEngineDefine.h"

class txAction;
class txActionTree : public txEngineBase
{
public:
	txActionTree(const std::string& name)
		:
		mName(name),
		mTreeState(PS_STOP),
		mTimeLine(0.0f),
		mTimeDirction(true),
		mRootAction(NULL),
		mUserData(NULL)
	{}
	virtual ~txActionTree(){ destroy(); }
	virtual void init(const bool& createRoot = true)
	{
		if (createRoot)
		{
			addRoot();
		}
	}
	virtual void update(float elapsedTime);
	void destroy();
	const std::string& getName() { return mName; }
	txAction* getRootAction() { return mRootAction; }
	txAction* getAction(const std::string& name);
	txAction* addRoot();
	void setTreeState(const PLAY_STATE& state);
	void play(const bool& direction = true);
	void pause();
	void stop();	// 恢复所有行为都未执行时的状态
	static txAction* createIndependentAction(const std::string& type, const std::string& name);
	txAction* addAction(const std::string& type, const std::string& name, txAction* parentAction);
	void addAction(txAction* action, txAction* parent);
	bool deleteAction(const std::string& name);
	void notifyActionDestroied(txAction* action);
	void notifyActionExecuting(txAction* action)
	{
		if (mExecutingAction.find(action) == mExecutingAction.end())
		{
			mExecutingAction.insert(action);
		}
	}
	void notifyActionExecutingDone(txAction* action)
	{
		if (mDeepestAction == action)
		{
			complete();
		}
		auto iterAction = mExecutingAction.find(action);
		if (iterAction != mExecutingAction.end())
		{
			mExecutingAction.erase(iterAction);
		}
	}
	void notifyActionStoped(txAction* action)
	{
		auto iterAction = mExecutingAction.find(action);
		if (iterAction != mExecutingAction.end())
		{
			mExecutingAction.erase(iterAction);
		}
	}
	const float& getTimeLine() { return mTimeLine; }
	void setUserData(void* userData) { mUserData = userData; }
	void* getUserData() { return mUserData; }
protected:
	void complete();
protected:
	std::string mName;
	PLAY_STATE mTreeState;						// 行为树当前的状态
	float mTimeLine;							// 当前时间线,表示执行到了行为树的哪个时间点
	bool mTimeDirction;							// 时间流逝方向,true表示递增,false表示递减
	txAction* mRootAction;						// 根行为
	txAction* mDeepestAction;					// 最后一个执行完的行为
	txMap<std::string, txAction*> mActionList;	// 行为树中所有的行为
	txSet<txAction*> mExecutingAction;			// 正在执行的行为列表
	void* mUserData;							// 用于扩展的自定义参数
};

#endif