#ifndef _TX_ACTION_H_
#define _TX_ACTION_H_

#include "txEngineBase.h"
#include "txEngineDefine.h"
#include "txEngineLog.h"
#include "txPropertyReceiver.h"
#include "txCommandReceiver.h"

class txActionTree;
class txAction : public txPropertyReceiver, public txCommandReceiver
{
public:
	txAction(const std::string& typeName, const std::string& name)
		:
		txCommandReceiver(name),
		mTypeName(typeName),
		mParentActionTree(NULL),
		mActionState(AES_NOT_EXECUTE),
		mParentAction(NULL),
		mStartCallback(NULL),
		mStartUserData(NULL),
		mDoneCallback(NULL),
		mDoneUserData(NULL),
		mInverseStartCallback(NULL),
		mInverseStartUserData(NULL),
		mInverseDoneCallback(NULL),
		mInverseDoneUserData(NULL),
		mTimeCount(0.0f),
		mTimeDepth(0.0f),
		mExecuteTime(0.0f),
		mExecuteDoneTimeDepth(0.0f),
		mDirection(true)
	{}
	virtual ~txAction(){ destroy(); }
	virtual void init(txActionTree* tree)
	{
		mParentActionTree = tree;
		initReceiver();
	}
	virtual void initProperty(){}
	virtual void update(float elapsedTime);
	const std::string& getTypeName()			{ return mTypeName; }
	const float& getExecuteTime()				{ return mExecuteTime; }
	const float& getTimeDepth()					{ return mTimeDepth; }
	const float& getExecuteDoneTimeDepth()		{ return mExecuteDoneTimeDepth; }
	txActionTree* getActionTree()				{ return mParentActionTree; }
	const ACTION_EXECUTE_STATE& getActionState(){ return mActionState; }
	txAction* getParent()						{ return mParentAction; }
	txVector<txAction*>& getChildList(){ return mChildActionList; }
	// 由于开始执行行为时的时间与行为实际的时间深度有偏差(由一帧时间影响),所以在开始执行行为时需要有一定的时间偏移
	virtual void execute(const float& timeOffset, const bool& direction = true);
	virtual void setDefaultParam(){}
	void destroy();
	void pause();
	void resume(const bool& direciton = true);
	void stop();			// 停止当前以及所有子行为中正在执行和已经执行完毕的行为,并恢复初始状态
	void recover();
	void startInverse();	// 从最底端行为开始往上倒序执行行为
	void setActionState(const ACTION_EXECUTE_STATE& state);
	void setParent(txAction* parent)
	{
		mParentAction = parent;
		refreshTimeDepth();
	}
	void addChild(txAction* child);
	void removeChild(txAction* child);
	void setActionCallback(ActionCallback startCallback, void* startUserData, ActionCallback doneCallback, void* doneUserData)
	{
		mStartCallback = startCallback;
		mStartUserData = startUserData;
		mDoneCallback = doneCallback;
		mDoneUserData = doneUserData;
	}
	void setInverseActionCallback(ActionCallback inverseStart, void* startUserData, ActionCallback inverseDone, void* doneUserData)
	{
		mInverseStartCallback = inverseStart;
		mInverseStartUserData = startUserData;
		mInverseDoneCallback = inverseDone;
		mInverseDoneUserData = doneUserData;
	}
	// 由派生类调用设置行为完全执行所需要的时间
	virtual void setExecuteTime(const float& time);
	float getMaxExecuteDoneTimeDepth();	// 得到子行为中最大的执行完毕时的时间深度
	void resetStateToNotExecute(const float& timeLine, const bool& direction = true);// 将根据当前时间线将行为以及子行为的状态设置为未执行状态
	void getNeedExecuteAction(txMap<float, txVector<txAction*> >& actionTimeMap, const float& timeLine, const bool& direction = true);
	void getDeepestExecuteDoneAction(txAction*& action);	// 得到执行完成时间深度最深的行为
protected:
	virtual void pauseAction(){}
	virtual void resumeAction(const bool& direction = true){}
	virtual void recoverAction(){}
	virtual void stopAction(){}
	// 通知该行为父行为被销毁了
	void notifyParentDestroied()
	{
		mParentAction = NULL;
		refreshTimeDepth();
	}
	// 通知该行为子行为被销毁了
	void notifyChildDestroied(txAction* action)
	{
		removeChild(action);
	}
	// 通知该行为父行为的时间或者自己的时间有改变,需要更新自己的时间深度
	void notifyTimeChanged();
	// 刷新自己的时间深度
	void refreshTimeDepth()
	{
		if (mParentAction != NULL)
		{
			mTimeDepth = mParentAction->getTimeDepth() + mParentAction->getExecuteTime();
		}
		else
		{
			mTimeDepth = 0.0f;
		}
		mExecuteDoneTimeDepth = mTimeDepth + mExecuteTime;
	}
protected:
	std::string mTypeName;						// 行为的类型
	bool mDirection;							// 行为执行方向,为true则表示正向执行,为false则表示反向执行
	float mTimeCount;							// 当前计时
	float mTimeDepth;							// 行为当前的时间深度,顺序开始执行行为时的时间
	float mExecuteTime;							// 行为完全执行完所需要的时间
	float mExecuteDoneTimeDepth;				// 行为执行完时的时间深度,值等于mTimeDepth + mExecuteTime
	ACTION_EXECUTE_STATE mActionState;			// 当前行为的执行状态
	txActionTree* mParentActionTree;			// 所属行为树
	txAction* mParentAction;					// 父行为
	txVector<txAction*> mChildActionList;	// 子行为列表,保存着子行为之间的顺序
	txSet<txAction*> mChildActionSet;		// 防止子行为重复添加而存在的列表
	ActionCallback mStartCallback;				// 行为开始执行时的回调函数
	void* mStartUserData;
	ActionCallback mDoneCallback;				// 行为执行完毕时的回调函数
	void* mDoneUserData;
	ActionCallback mInverseStartCallback;		// 行为开始倒序执行时的回调函数
	void* mInverseStartUserData;
	ActionCallback mInverseDoneCallback;		// 行为倒序执行完毕时的回调函数
	void* mInverseDoneUserData;
};

#endif