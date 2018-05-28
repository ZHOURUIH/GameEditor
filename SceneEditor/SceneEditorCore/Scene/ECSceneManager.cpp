#include "Utility.h"

#include "ECSceneManager.h"
#include "ECScene.h"

bool ECSceneManager::notifySceneRenamed(const std::string& oldName, ECScene* scene)
{
	auto iterScene = mSceneList.find(oldName);
	if (iterScene == mSceneList.end())
	{
		return false;
	}
	auto itNew = mSceneList.find(scene->getName());
	if (itNew != mSceneList.end())
	{
		return false;
	}
	mSceneList.erase(iterScene);
	mSceneList.insert(scene->getName(), scene);
	return true;
}

ECScene* ECSceneManager::createScene(const std::string& name)
{
	auto iterScene = mSceneList.find(name);
	if (iterScene != mSceneList.end())
	{
		return NULL;
	}
	ECScene* scene = TRACE_NEW(ECScene, scene, name);
	mSceneList.insert(scene->getName(), scene);
	return scene;
}

void ECSceneManager::destroyScene(const std::string& name)
{
	auto iterScene = mSceneList.find(name);
	if (iterScene == mSceneList.end())
	{
		return;
	}
	TRACE_DELETE(iterScene->second);
	mSceneList.erase(iterScene);
}

ECScene* ECSceneManager::getScene(const std::string& name)
{
	auto iterScene = mSceneList.find(name);
	if (iterScene != mSceneList.end())
	{
		return iterScene->second;
	}
	return NULL;
}