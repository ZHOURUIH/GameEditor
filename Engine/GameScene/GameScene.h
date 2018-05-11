#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include "txComponentOwner.h"
#include "SceneProcedure.h"
#include "Utility.h"

class SceneProcedure;
// 这是一个空场景,里面没有任何东西,其他的场景都从该类继承
class GameScene : public txComponentOwner
{
public:
	GameScene(const int& type, const std::string& sceneName);
	virtual ~GameScene(){ destroy(); }
	virtual void init();
	void destroy();
	virtual void update(const float& elapsedTime);
	virtual void keyProcess(const float& elapsedTime);
	// 退出场景
	void exit();
	bool atProcedure(const int& procedureType);
	// 是否在指定的流程,并且不在其子流程
	bool atSelfProcedure(const int& type);
	void prepareChangeProcedure(const int& procedure, const float& time, const std::string& intent);
	void backToLastProcedure(const std::string& intend);
	bool changeProcedure(const int& procedure, const std::string& intent, const bool& addToLastList = true);
	// 由当前流程通知场景自己已经准备好了,可以卸载上一个流程了
	void notifyProcedurePrepared();
	int getLastProcedureType();
	SceneProcedure* getSceneProcedure(const int& type);
	SceneProcedure* getCurSceneProcedure() { return mCurProcedure; }
	//template<typename T>
	//T getCurOrParentProcedure<T>(const int& type)
	//{
	//	return mCurProcedure->getThisOrParent<T>(type);
	//}
	const int& getSceneType() { return mSceneType; }
	// 通知场景用户点击了界面空白处
	virtual void notifyScreenActived() { }
protected:
	virtual void assignStartExitProcedure() = 0;
	virtual void createSceneProcedure() = 0;
	virtual void initComponents();
	template<typename T>
	T* addProcedure(const int& type, SceneProcedure* parent = NULL)
	{
		T* procedure = TRACE_NEW(T, procedure, type, this);
		if (parent != NULL)
		{
			parent->addChildProcedure(procedure);
		}
		mSceneProcedureList.insert(procedure->getProcedureType(), procedure);
		return procedure;
	}
protected:
	txMap<int, SceneProcedure*>	mSceneProcedureList;
	txVector<SceneProcedure*>	mLastProcedureList;	// 所进入过的所有流程
	SceneProcedure*				mCurProcedure;
	int		mSceneType;
	int		mStartProcedure;
	int		mExitProcedure;
	int		mLastProcedureType;
	int		mMaxLastProcedureCount;
};

#endif