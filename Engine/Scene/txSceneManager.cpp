#include "json/json.h"
#include "txSceneManager.h"
#include "txEngineLog.h"
#include "txEntity.h"
#include "txCamera.h"
#include "txDirLight.h"
#include "txTexture.h"
#include "txObject.h"
#include "txBillboardManager.h"
#include "txMagicParticleSystem.h"
#include "txNodeManager.h"
#include "txTextureManager.h"
#include "txEntityManager.h"
#include "txScene.h"
#include "txSkyBox.h"
#include "txEngineRoot.h"

txSceneManager::txSceneManager()
{
	bSwitchShadowMap = false;
	mRootNode = NULL;
	mFogColor = VECTOR3(1.0f, 1.0f, 1.0f);
	mNearDis = 10000.0f;
	mFarDis = 10000.0f;
	fSky_FogCoef = 0.0f;
	mLockUpdate = false;
	mSkyBox = NULL;

	// 创建线程互斥锁
#if RUN_PLATFORM == PLATFORM_WINDOWS
	mMutex = CreateMutex(NULL, false, NULL);
#elif RUN_PLATFORM == PLATFORM_ANDROID
	mMutex = PTHREAD_MUTEX_INITIALIZER;
#endif
}

txSceneManager::~txSceneManager()
{
	// 销毁线程互斥锁
#if RUN_PLATFORM == PLATFORM_WINDOWS
	CloseHandle(mMutex);
#elif RUN_PLATFORM == PLATFORM_ANDROID
	pthread_mutex_destroy(&mMutex);
#endif
}

void txSceneManager::init()
{
	mRootNode = mNodeManager->CreateNode();
}

void txSceneManager::lock()
{
	mLockUpdate = true;
	// 锁定线程互斥锁
#if RUN_PLATFORM == PLATFORM_WINDOWS
	DWORD d = WaitForSingleObject(mMutex, INFINITE);
#elif RUN_PLATFORM == PLATFORM_ANDROID
	pthread_mutex_lock(&mMutex);
#endif
}

void txSceneManager::unlock()
{
	mLockUpdate = false;
	// 解锁线程互斥锁
#if RUN_PLATFORM == PLATFORM_WINDOWS
	ReleaseMutex(mMutex);
#elif RUN_PLATFORM == PLATFORM_ANDROID
	pthread_mutex_unlock(&mMutex);
#endif
}

void txSceneManager::update(float fElaspedTime)
{
	if(!mLockUpdate)
	{
		// 更新场景中节点
		mRootNode->Update(fElaspedTime);
	}
}

txScene* txSceneManager::getScene(const std::string& sceneName)
{
	auto iter = mSceneList.find(sceneName);
	if (iter == mSceneList.end())
	{
		return NULL;
	}
	return iter->second;
}

bool txSceneManager::setSceneVisible(const std::string& sceneName, const bool& visible)
{
	txScene* scene = getScene(sceneName);
	if (scene == NULL)
	{
		ENGINE_ERROR("error : there is no scene named : %s, can not set visible!", sceneName.c_str());
		return false;
	}
	scene->setVisible(visible);
	return true;
}

bool txSceneManager::getSceneEntityList(const std::string& sceneName, txMap<std::string, txObject*>& entityList)
{
	txScene* scene = getScene(sceneName);
	if (scene == NULL)
	{
		ENGINE_ERROR("error : there is no scene named : %s, can not get entities!", sceneName.c_str());
		return false;
	}
	entityList = scene->getObjectList();
	return true;
}

bool txSceneManager::moveSceneEntities(const std::string& sceneName, const VECTOR3& delta)
{
	txScene* scene = getScene(sceneName);
	if (scene == NULL)
	{
		ENGINE_ERROR("error : there is no scene named : %s, can not get entities!", sceneName.c_str());
		return false;
	}
	scene->moveScene(delta);
	return true;
}

bool txSceneManager::alignedToWorldOrigin(const std::string& sceneName, const bool& alignedHeight)
{
	if (sceneName == EMPTY_STRING)
	{
		auto itr = mSceneList.begin();
		auto itrEnd = mSceneList.end();
		FOR_STL(mSceneList, ; itr != itrEnd; ++itr)
		{
			itr->second->alignedToWorldOrigin(alignedHeight);
		}
		END_FOR_STL(mSceneList);
	}
	else
	{
		txScene* scene = getScene(sceneName);
		if (scene == NULL)
		{
			ENGINE_ERROR("error : there is no scene named : %s, can not aligned to world origin!", sceneName.c_str());
			return false;
		}
		scene->alignedToWorldOrigin(alignedHeight);
	}

	return true;
}

bool txSceneManager::setSceneMove(const std::string& sceneName, const VECTOR3& pos)
{
	txScene* scene = getScene(sceneName);
	if (scene == NULL)
	{
		ENGINE_ERROR("error : there is no scene named : %s, can not set scene move!", sceneName.c_str());
		return false;
	}
	scene->resetScenePosition();
	scene->moveScene(pos);
	return true;
}

void txSceneManager::destroyScene(const std::string& sceneName)
{
	txScene* scene = getScene(sceneName);
	if (scene == NULL)
	{
		ENGINE_ERROR("error : there is no scene named : %s, can not destroy entities!", sceneName.c_str());
		return;
	}
	TRACE_DELETE(scene);
	auto iter = mSceneList.find(sceneName);
	if (iter != mSceneList.end())
	{
		mSceneList.erase(iter);
	}
}

void txSceneManager::destroyAllScene()
{
	auto iter = mSceneList.begin();
	auto iterEnd = mSceneList.end();
	FOR_STL(mSceneList, ; iter != iterEnd; ++iter)
	{
		TRACE_DELETE(iter->second);
	}
	END_FOR_STL(mSceneList);
	mSceneList.clear();
}

bool txSceneManager::getHeight(const float& x, const float& z, float& y, const std::string& sceneName)
{
	if (sceneName == EMPTY_STRING)
	{
		bool ret = false;
		auto iter = mSceneList.begin();
		auto iterEnd = mSceneList.end();
		FOR_STL(mSceneList, ; iter != iterEnd; ++iter)
		{
			if (iter->second->getHeight(x, z, y))
			{
				ret = true;
			}
		}
		END_FOR_STL(mSceneList);
		return ret;
	}
	else
	{
		txScene* scene = getScene(sceneName);
		if (scene == NULL)
		{
			ENGINE_ERROR("error : there is no scene named : %s, can not get scene height!", sceneName.c_str());
			return false;
		}
		return scene->getHeight(x, z, y);
	}
	return false;
}

TERRAIN_TYPE txSceneManager::getTerrainType(const float& x, const float& z, const std::string& sceneName)
{
	if (sceneName == EMPTY_STRING)
	{
		TERRAIN_TYPE type = TT_GROUND;
		auto iter = mSceneList.begin();
		auto iterEnd = mSceneList.end();
		FOR_STL(mSceneList, ; iter != iterEnd; ++iter)
		{
			type = iter->second->getTerrainType(x, z);
			break;
		}
		END_FOR_STL(mSceneList);
		return type;
	}
	else
	{
		txScene* scene = getScene(sceneName);
		if (scene == NULL)
		{
			ENGINE_ERROR("error : there is no scene named : %s, can not get scene height!", sceneName.c_str());
			return TT_GROUND;
		}
		return scene->getTerrainType(x, z);
	}
	return TT_GROUND;
}

void txSceneManager::destroy()
{
	destroyAllScene();

	if (mRootNode != NULL)
	{
		mNodeManager->DestroyNode(mRootNode->GetName());
		mRootNode = NULL;
	}
}

bool txSceneManager::notifySceneRenamed(const std::string& oldName, txScene* scene)
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

//异步加载方法后创建纹理(在加载场景之后调用来创建纹理)
void txSceneManager::createSceneTexture(const std::string& sceneName)
{
	if (sceneName == EMPTY_STRING)
	{
		auto iter = mSceneList.begin();
		auto iterEnd = mSceneList.end();
		FOR_STL(mSceneList, ; iter != iterEnd; ++iter)
		{
			iter->second->generaTextures();
		}
		END_FOR_STL(mSceneList);
	}
	else
	{
		auto iter = mSceneList.find(sceneName);
		if (iter != mSceneList.end())
		{
			iter->second->generaTextures();
		}
	}
}

void txSceneManager::generateSceneAABB(const std::string& sceneName)
{
	if (sceneName == EMPTY_STRING)
	{
		auto iter = mSceneList.begin();
		auto iterEnd = mSceneList.end();
		FOR_STL(mSceneList, ; iter != iterEnd; ++iter)
		{
			iter->second->generateSceneRect();
		}
		END_FOR_STL(mSceneList);
	}
	else
	{
		auto iter = mSceneList.find(sceneName);
		if (iter != mSceneList.end())
		{
			iter->second->generateSceneRect();
		}
	}
}

void txSceneManager::destroySkyBox()
{
	TRACE_DELETE(mSkyBox);
}

txSkyBox* txSceneManager::createSkyBox(const float& fDistance, const float& fUpDistance, const float& fBottomDistance, const std::string& texturePath)
{
	if (mSkyBox != NULL)
	{
		return mSkyBox;
	}
	mSkyBox = TRACE_NEW(txSkyBox, mSkyBox);
	mSkyBox->init(fDistance, fUpDistance, fBottomDistance, texturePath);
	return mSkyBox;
}

txScene* txSceneManager::newScene(const std::string& newSceneName)
{
	auto iterScene = mSceneList.find(newSceneName);
	if (iterScene != mSceneList.end())
	{
		ENGINE_ERROR("error : scene file named : %s have already loaded! txSceneManager::newScene", newSceneName.c_str());
		return NULL;
	}
	txScene* scene = TRACE_NEW(txScene, scene, newSceneName);
	mSceneList.insert(scene->getName(), scene);
	return scene;
}

bool txSceneManager::loadSceneFromFile(const char* sceneName, const bool& binary)
{
	auto iterScene = mSceneList.find(sceneName);
	if (iterScene != mSceneList.end())
	{
		ENGINE_ERROR("error : scene file named : %s have already loaded! txSceneManager::LoadSceneFromFile", sceneName);
		return false;
	}

	txScene* scene = TRACE_NEW(txScene, scene, sceneName);
	bool ret = scene->loadScene(binary);
	mSceneList.insert(scene->getName(), scene);
	return ret;
}

bool txSceneManager::copyScene(const char* srcSceneName, const char* newSceneName)
{
	auto iterScene = mSceneList.find(srcSceneName);
	if (iterScene == mSceneList.end())
	{
		ENGINE_ERROR("error : can not find src scene named : %s, new scene name : %s", srcSceneName, newSceneName);
		return false;
	}
	txScene* scene = TRACE_NEW(txScene, scene, newSceneName);
	bool ret = scene->copySceneFrom(iterScene->second);
	mSceneList.insert(scene->getName(), scene);
	return ret;
}
