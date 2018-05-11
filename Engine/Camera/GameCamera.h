#ifndef _GAME_CAMERA_H_
#define _GAME_CAMERA_H_

#include "txMovableObject.h"

class txCamera;
class CameraLinker;

// 摄像机类
class GameCamera : public txMovableObject
{
public:
	GameCamera(const std::string& name);
	virtual ~GameCamera(){ destroy(); }
	virtual void init();
	virtual void initComponents();
	virtual void update(float elapsedTime);
	virtual void destroy();
	virtual void keyProcess(float elapsedTime);
	virtual void notifyAddComponent(txComponent* component);
	virtual void notifyComponentDestroied(txComponent* component);
	void setViewport(const int& x, const int& y, const int& width, const int& height);
	void linkTarget(const std::string& linkerName, txMovableObject* target);
    void move(const VECTOR3& moveDelta, const TRANS_SPACE& space = TS_LOCAL);
	void lookAt(const VECTOR3& lookat);
    void yawpitch(const float& fYaw, const float& fPitch);
	void resetCamera();
	void resetRotation();
    void refresh();
	virtual void setPosition(const VECTOR3& position, const bool& refreshNow = false);
	virtual void setRotation(const MATRIX3& rotation, const bool& refreshNow = false);
	void setRealPositon(const VECTOR3& realPosition)
	{
		mRealPosition = realPosition;
		mDirty = true;
		refresh();
	}

	//获取
	VECTOR3 getDirection();
	VECTOR3 getUp();
	txCamera* getCamera(){ return mCamera; }
	const VECTOR3& getRealPosition(){ return mRealPosition; }
	CameraLinker* getCurLinker() { return mCurLinker; }
	const bool& getLockKeyProcess() { return mLockKeyProcess; }
	const float& getMoveSpeed() { return mMoveSpeed; }
	const float& getRotateSpeed() { return mRotateSpeed; }
	const bool& getVerticalDown() { return mVerticalDown; }

	void setLockKeyProcess(const bool& lock) { mLockKeyProcess = lock; }
	void setMoveSpeed(const float& speed) { mMoveSpeed = speed; }
	void setRotateSpeed(const float& speed) { mRotateSpeed = speed; }
	void setVerticalDown(const bool& verticalDown) { mVerticalDown = verticalDown; }
protected:
	// 摄像机当前的欧拉角,由于欧拉角在和旋转矩阵互相转换时存在不对应的情况,所以旋转摄像机使用欧拉角累加,然后计算矩阵
	VECTOR3 mCameraEulerAngle;
	bool mDontWriteEulerAngle;	// 是否在设置旋转矩阵时不改变已保存的欧拉角
	txCamera* mCamera;   
    bool mDirty;
	VECTOR3 mRealPosition;
	CameraLinker* mCurLinker;	// 只是记录当前连接器方便外部获取
	txMap<std::string, CameraLinker*> mLinkerList; // 由于对连接器的操作比较多,所以单独放到一个表中,组件列表中不变
	float mMoveSpeed;			// 使用按键控制时的移动速度
	float mRotateSpeed;			// 鼠标旋转摄像机时的旋转速度
	bool mLockKeyProcess;		// 锁定摄像机的按键控制
	bool mVerticalDown;			// 是否垂直向下
};

#endif
