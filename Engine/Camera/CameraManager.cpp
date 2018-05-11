#include "Utility.h"
#include "txEngineLog.h"
#include "CameraManager.h"
#include "GameCamera.h"

void CameraManager::update(float elapsedTime)
{
	auto iter = mCameraList.begin();
	auto iterEnd = mCameraList.end();
	FOR_STL(mCameraList, ; iter != iterEnd; ++iter)
	{
		iter->second->update(elapsedTime);
	}
	END_FOR_STL(mCameraList);
}

GameCamera* CameraManager::createCamera(const std::string& name)
{
	auto iterFind = mCameraList.find(name);
	if (iterFind != mCameraList.end())
	{
		// 摄像机已经存在
		ENGINE_ERROR("GameCamera : %s is already exist!", name.c_str());
		return NULL;
	}
	GameCamera* camera = TRACE_NEW(GameCamera, camera, name);
	camera->init();
	mCameraList.insert(name, camera);
	return camera;
}

void CameraManager::destoryCamera(const std::string& name)
{
	auto iterFind = mCameraList.find(name);
	if (iterFind != mCameraList.end())
	{
		TRACE_DELETE(iterFind->second);
		mCameraList.erase(iterFind);
	}
}

void CameraManager::destoryAllCamera()
{
	auto iterCamera = mCameraList.begin();
	auto iterCameraEnd = mCameraList.end();
	FOR_STL(mCameraList, ; iterCamera != iterCameraEnd; ++iterCamera)
	{
		TRACE_DELETE(iterCamera->second);
	}
	END_FOR_STL(mCameraList);
	mCameraList.clear();
}