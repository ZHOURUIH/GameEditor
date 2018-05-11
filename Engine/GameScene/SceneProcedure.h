#ifndef _SCENE_PROCEDURE_H_
#define _SCENE_PROCEDURE_H_

#include "Utility.h"
#include "txEngineBase.h"

class GameScene;
class SceneProcedure : public txEngineBase
{
public:
	SceneProcedure(const int& type, GameScene* gameScene);
	virtual ~SceneProcedure(){}
	// 由GameScene调用
	// 进入流程
	virtual void init(SceneProcedure* lastProcedure, const std::string& intent);
	// 更新流程
	virtual void update(const float& elapsedTime);
	// 退出流程
	virtual void exit(SceneProcedure* exitTo, SceneProcedure* nextPro);
	// 准备退出
	void prepareExit(SceneProcedure* next, const float& time, const std::string& intent);
	void keyProcess(const float& elapsedTime);
	void addDelayCmd(txCommand* cmd);
	void getParentList(txVector<SceneProcedure*>& parentList);
	// 获得自己和otherProcedure的共同的父节点
	SceneProcedure* getSameParent(SceneProcedure* otherProcedure);
	bool isThisOrParent(const int& type);
	// 得到指定类型的父流程,如果所有父流程中没有该类型,则返回NULL
	SceneProcedure* getParent(const int& type);
	template<typename T>
	T* getThisOrParent(const int& type)
	{
		if (mProcedureType == type)
		{
			return (T*)this;
		}
		else
		{
			SceneProcedure* parent = getParent(type);
			if (parent != NULL)
			{
				return (T*)parent;
			}
		}
		return NULL;
	}
	SceneProcedure* getChildProcedure(const int& type);
	SceneProcedure* getCurChildProcedure() { return mCurChildProcedure; }
	bool addChildProcedure(SceneProcedure* child);
	const int& getProcedureType() { return mProcedureType; }
	GameScene* getGameScene() { return mGameScene; }
	SceneProcedure* getParent() { return mParentProcedure; }
	// 是否正在准备退出流程
	bool isPreparingExit() { return mCurPrepareTime >= 0.0f; }
	// 是否为具有准备退出的流程
	bool hasPrepareExit() { return mExitTime > 0.0f; }
	// 进入的目标流程已经准备完成(资源加载完毕等等)时的回调
	virtual void onNextProcedurePrepared(SceneProcedure* nextPreocedure) {}
protected:
	// 从自己的子流程进入当前流程时调用
	virtual void onInitFromChild(SceneProcedure* lastProcedure, const std::string& intent) {}
	// 在进入流程时调用
	// 在onInit中如果要跳转流程,必须使用延迟命令进行跳转
	virtual void onInit(SceneProcedure* lastProcedure, const std::string& intent) = 0;
	// 更新流程时调用
	virtual void onUpdate(float elapsedTime) = 0;
	// 更新流程时调用
	virtual void onKeyProcess(float elapsedTime) = 0;
	// 退出当前流程,进入的不是自己的子流程时调用
	virtual void onExit(SceneProcedure* nextProcedure) = 0;
	// 退出当前流程,进入自己的子流程时调用
	virtual void onExitToChild(SceneProcedure* nextProcedure) {}
	// 退出当前流程进入其他任何流程时调用
	virtual void onExitSelf() {}
	virtual void onPrepareExit(SceneProcedure* nextPreocedure) {}
	bool setParent(SceneProcedure* parent);
	static void onCmdStarted(txCommand* cmd, void* userdata);
protected:
	txMap<int, SceneProcedure*> mChildProcedureList;	// 子流程列表
	txSet<int>		mDelayCmdList;		// 流程进入时的延迟命令列表,当命令执行时,会从列表中移除该命令
	int				mProcedureType;		// 该流程的类型
	GameScene*		mGameScene;			// 流程所属的场景
	SceneProcedure*	mParentProcedure;	// 父流程
	SceneProcedure*	mCurChildProcedure;	// 当前正在运行的子流程
	bool			mInited;			// 是否已经初始化,子节点在初始化时需要先确保父节点已经初始化
	// 以下变量为准备退出时使用的
	float			mExitTime;			// 从准备退出到真正退出流程所需要的时间,小于等于0表示不需要准备退出
	float			mCurPrepareTime;    // 准备退出的计时,大于等于0表示正在准备退出,小于0表示没有正在准备退出
	SceneProcedure*	mPrepareNext;
	std::string		mPrepareIntent;
};

#endif