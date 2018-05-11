#ifndef _GAME_SCENE_FACTORY_MANAGER_H_
#define _GAME_SCENE_FACTORY_MANAGER_H_

#include "txFactoryManager.h"

class GameSceneFactoryBase;
class GameSceneFactoryManager : public txFactoryManager<int, GameSceneFactoryBase>
{
public:
	virtual void init();
};

#endif