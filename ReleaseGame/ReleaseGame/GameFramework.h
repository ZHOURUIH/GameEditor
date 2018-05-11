#ifndef _GAME_FRAMEWORK_H_
#define _GAME_FRAMEWORK_H_

#include <map>

#include "txConfig.h"
#include "txSingleton.h"

#include "CommonDefine.h"

class DataBase;
class GameConfig;
class txDataManager;

class GameFramework : public txSingleton<GameFramework>
{
public:
	GameFramework();
	virtual ~GameFramework();
	bool init();

	void update(float elapsedTime);
	void render();
	void destroy();

	GameConfig* mGameConfig;
	txDataManager* mDataManager;
};

#endif
