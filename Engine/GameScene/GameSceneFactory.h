#ifndef _GAME_SCENE_FACTORY_H_
#define _GAME_SCENE_FACTORY_H_

#include "Utility.h"

#include "GameScene.h"

class GameSceneFactoryBase
{
public:
	GameSceneFactoryBase(const int& type)
		:
		mType(type)
	{}
	virtual ~GameSceneFactoryBase()
	{
		;
	}
	virtual GameScene* createScene() = 0;
	template<typename T>
	static GameSceneFactoryBase* createFactory(const int& type)
	{
		T* factory = TRACE_NEW(T, factory, type);
		return factory;
	}
	const int& getType() { return mType; }
protected:
	// 场景工厂是以名字来区分类型的,同一个场景类具有相同的名字,所以每个工厂的名字也是根据类型固定的
	int mType;
};

template<typename T>
class GameSceneFactory : public GameSceneFactoryBase
{
public:
	GameSceneFactory(const int& type)
		:
		GameSceneFactoryBase(type)
	{}
	virtual GameScene* createScene()
	{
		GameScene* newScene = TRACE_NEW(T, newScene, mType);
		++mCurCount;
		return newScene;
	}
};

#endif