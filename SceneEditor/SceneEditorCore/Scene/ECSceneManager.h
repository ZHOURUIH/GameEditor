#ifndef _EC_SCENE_MANAGER_H_
#define _EC_SCENE_MANAGER_H_

#include "CommonDefine.h"

class ECScene;
class ECSceneManager
{
public:
	ECSceneManager(){}
	virtual ~ECSceneManager(){ destroy(); }
	virtual void init(){}
	virtual void update(float elapsedTime){}
	void destroy(){}
	bool notifySceneRenamed(const std::string& oldName, ECScene* scene);
	ECScene* createScene(const std::string& name);
	void destroyScene(const std::string& name);
	ECScene* getScene(const std::string& name);
protected:
	txMap<std::string, ECScene*> mSceneList;
};

#endif