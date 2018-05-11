#ifndef _CAMERA_MANAGER_H_
#define _CAMERA_MANAGER_H_

#include "txEngineDefine.h"

// 相机管理类
class GameCamera;
class CameraManager
{
public:
	CameraManager()
		:
		mMainCamera(NULL)
	{}
	virtual ~CameraManager(){ destroy(); }
	GameCamera* createCamera(const std::string& name);
	void destoryCamera(const std::string& name);
	void destoryAllCamera();
	void update(float elapsedTime);
	//获得主相机
	GameCamera* getMainCamera(){ return mMainCamera; }
	void init()
	{
		// 创建主摄像机
		mMainCamera = createCamera("main camera");
	}
	void setMainCamera(GameCamera* camera) { mMainCamera = camera; }
	void destroy()
	{
		// 先销毁所有摄像机
		destoryAllCamera();
	}
protected:
	txMap<std::string, GameCamera*> mCameraList;
	GameCamera* mMainCamera;
};

#endif