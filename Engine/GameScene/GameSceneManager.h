#ifndef _GAME_SCENE_MANAGER_H_
#define _GAME_SCENE_MANAGER_H_

#include "txCommandReceiver.h"

class GameScene;
class GameSceneFactoryManager;
class GameSceneManager : public txCommandReceiver
{
public:
	GameSceneManager();
	virtual ~GameSceneManager(){ destroy(); }
	bool enterScene(const int& type);
	virtual void init();
	virtual void update(float elapsedTime);
	void destroy();
	GameScene* getCurScene() { return mCurScene; }
protected:
	GameScene* mCurScene;
	txVector<GameScene*> mLastSceneList;
	GameSceneFactoryManager* mGameSceneFactoryManager;
};

#endif