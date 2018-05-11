#include "txCamera.h"
#include "txSceneManager.h"
#include "txEngineRoot.h"
#include "txRenderWindow.h"
#include "txMathUtility.h"
#include "txMovableObject.h"
#include "txComponentHeader.h"

#include "GameCamera.h"
#include "CameraLinker.h"
#include "CameraLinkerFixed.h"
#include "CameraLinkerSpring.h"
#include "CameraManager.h"
#include "InputSystem.h"

GameCamera::GameCamera(const std::string& name)
:
txMovableObject(name),
mCamera(NULL),
mDirty(true),
mCurLinker(NULL),
mMoveSpeed(50.0f),
mRotateSpeed(5.0f),
mLockKeyProcess(true),
mDontWriteEulerAngle(false)
{
	mRealPosition = getPosition();
}

void GameCamera::init()
{
	if (mCamera != NULL)
	{
		return;
	}
	mCamera = TRACE_NEW(txCamera, mCamera);

	float aspect = mRenderWindow->getAspect();
	mCamera->setPerspectiveParameters(45.0f, aspect, 1.0f, 1000.0f);
	setViewport(0, 0, mRenderWindow->getWidth(), mRenderWindow->getHeight());
	mCamera->setName(mName);
	// 刷新视图矩阵
	refresh();
	txMovableObject::init();
}

void GameCamera::initComponents()
{
	// 给摄像机添加默认组件
	addComponent("fixedLinker", TOSTRING(CameraLinkerFixed));
	addComponent("springLinker", TOSTRING(CameraLinkerSpring));
	addComponent("fixedTimeLinker", TOSTRING(CameraLinkerFixedTime));
	addComponent("accelerationLinker", TOSTRING(CameraLinkerAcceleration));
}

void GameCamera::destroy()
{
	TRACE_DELETE(mCamera);
	mLinkerList.clear();
	mCurLinker = NULL;
}

void GameCamera::keyProcess(float elapsedTime)
{
#if RUN_PLATFORM == PLATFORM_WINDOWS
	float cameraSpeed = mMoveSpeed;
	if (!txMath::isFloatZero(cameraSpeed))
	{
		// 键盘移动摄像机
		if (mInputSystem->isKeyDown(VK_CONTROL))
		{
			cameraSpeed *= 2.0f;
		}
		// 向前移动摄像机
		if (mInputSystem->isKeyDown('W'))
		{
			move(txMath::AXIS_Z * cameraSpeed * elapsedTime);
		}
		// 向左移动摄像机
		if (mInputSystem->isKeyDown('A'))
		{
			move(txMath::AXIS_X * cameraSpeed * elapsedTime);
		}
		// 向后移动摄像机
		if (mInputSystem->isKeyDown('S'))
		{
			move(-txMath::AXIS_Z * cameraSpeed * elapsedTime);
		}
		// 向右移动摄像机
		if (mInputSystem->isKeyDown('D'))
		{
			move(-txMath::AXIS_X * cameraSpeed * elapsedTime);
		}
		// 竖直向上移动摄像机
		if (mInputSystem->isKeyDown('Q'))
		{
			move(txMath::AXIS_Y * cameraSpeed * elapsedTime, TS_WORLD);
		}
		// 竖直向下移动摄像机
		if (mInputSystem->isKeyDown('E'))
		{
			move(-txMath::AXIS_Y * cameraSpeed * elapsedTime, TS_WORLD);
		}
	}
	// 鼠标旋转摄像机
	KEY_STATE mouseState = mInputSystem->getMouseButtonState(MBD_RIGHT);
	if (mouseState.mState == KPS_CURRENT_DOWN || mouseState.mState == KPS_KEEP_DOWN)
	{
		const txIPoint& lastPosition = mInputSystem->getLastMousePosition();
		const txIPoint& curPosition = mInputSystem->getMousePosition();
		txIPoint moveDelta = curPosition - lastPosition;
		if (moveDelta.x != 0 || moveDelta.y != 0)
		{
			yawpitch(moveDelta.x / 1000.0f * mRotateSpeed, moveDelta.y / 1000.0f * mRotateSpeed);
		}
	}
	// 鼠标滚轮移动摄像机
	float mouseWheelDelta = mInputSystem->getMouseWheelDelta();
	if (!txMath::isFloatZero(mouseWheelDelta))
	{
		move(-txMath::AXIS_Z * mouseWheelDelta / 120.0f * cameraSpeed * 2.0f);
	}
#endif
}

void GameCamera::setViewport(const int& x, const int& y, const int& width, const int& height)
{
	if (mCamera != NULL)
	{
		mCamera->setViewport((float)x, (float)y, (float)width, (float)height);
	}
}

void GameCamera::linkTarget(const std::string& linkerName, txMovableObject* target)
{
	txComponent* com = getComponent(linkerName);
	if (com->getBaseType() != TOSTRING(CameraLinker))
	{
		return;
	}
	auto iterLinker = mLinkerList.begin();
	auto iterLinkerEnd = mLinkerList.end();
	FOR_STL(mLinkerList, ; iterLinker != iterLinkerEnd; ++iterLinker)
	{
		// 使用指针比较会更快
		if ((txComponent*)iterLinker->second != com)
		{
			iterLinker->second->setLinkObject(NULL);
			// 将没有使用的连接器设置为不激活
			iterLinker->second->setActive(false);
		}
		else
		{
			iterLinker->second->setLinkObject(target);
			// 如果使用了该连接器,则激活该连接器
			iterLinker->second->setActive(target != NULL);
		}
	}
	END_FOR_STL(mLinkerList);

	// 如果是要断开当前连接器,则将连接器名字清空
	if ((txComponent*)mCurLinker == com && target == NULL)
	{
		mCurLinker = NULL;
	}
	// 否则记录当前连接器
	else
	{
		mCurLinker = static_cast<CameraLinker*>(com);
	}
}

void GameCamera::notifyAddComponent(txComponent* component)
{
	// 如果是连接器,则还要加入连接器列表中
	if (component->getBaseType() == TOSTRING(CameraLinker))
	{
		mLinkerList.insert(component->getName(), static_cast<CameraLinker*>(component));
	}
}

void GameCamera::notifyComponentDestroied(txComponent* component)
{
	// 如果是销毁的当前正在使用的连接器,则将记录的连接器清空
	if ((txComponent*)mCurLinker == component)
	{
		mCurLinker = NULL;
	}

	auto iter = mLinkerList.find(component->getName());
	if (iter != mLinkerList.end())
	{
		mLinkerList.erase(iter);
	}
}

void GameCamera::move(const VECTOR3& moveDelta, const TRANS_SPACE& space)
{
	if (mCamera == NULL)
	{
		return;
	}
	VECTOR3 delta = moveDelta;
	if (space == TS_LOCAL)
	{
		delta = txMath::rotateVector3(delta, getRotation());
	}
	setPosition(getPosition() + delta);
}

void GameCamera::lookAt(const VECTOR3& lookat)
{
	MATRIX3 rot(txMath::lookAt(getPosition(), lookat, txMath::AXIS_Y));
	mCameraEulerAngle = txMath::matrix3ToEulerAngle(rot);
	mDontWriteEulerAngle = true;
	setRotation(rot);
}

void GameCamera::yawpitch(const float& fYaw, const float& fPitch)
{
	if (mCamera == NULL)
	{
		return;
	}
	mCameraEulerAngle += VECTOR3(fYaw, fPitch, 0.0f);
	// 欧拉角已经计算了,不能再改变
	mDontWriteEulerAngle = true;
	setRotation(txMath::eulerAngleToMatrix3(mCameraEulerAngle), true);
}

void GameCamera::resetCamera()
{
	setPosition(txMath::VEC3_ZERO);
	resetRotation();
}

void GameCamera::resetRotation()
{
	mCameraEulerAngle = txMath::VEC3_ZERO;
	mDontWriteEulerAngle = true;
	setRotation(txMath::MAT3_IDENTITY);
}

void GameCamera::update(float elapsedTime)
{	
	txMovableObject::update(elapsedTime);
	// 判断按键
	if (!mLockKeyProcess)
	{
		keyProcess(elapsedTime);
	}
}

void GameCamera::refresh()
{
	if (mDirty && mCamera != NULL)
	{
		mCamera->setViewMatrix(txMath::lookAt(mRealPosition, mRealPosition + getDirection(), getUp()));
		mDirty = false;
	}
}

void GameCamera::setPosition(const VECTOR3& position, const bool& refreshNow)
{
	txMovableObject::setPosition(position, refreshNow);
	mRealPosition = position;
	mDirty = true;
	refresh();
}

void GameCamera::setRotation(const MATRIX3& rotation, const bool& refreshNow)
{
	if (!mDontWriteEulerAngle)
	{
		mCameraEulerAngle = txMath::matrix3ToEulerAngle(rotation);
	}
	// 每次设置矩阵之前如果没有明确标记不修改欧拉角,则设置矩阵时否需要修改欧拉角
	mDontWriteEulerAngle = false;
	MATRIX3 rotMat = rotation;
	txMath::resetRight(rotMat);
	txMovableObject::setRotation(rotMat, refreshNow);
	mDirty = true;
	refresh();
}

VECTOR3 GameCamera::getDirection()
{
	return txMath::rotateVector3(txMath::AXIS_Z, getRotation());
}

VECTOR3 GameCamera::getUp()
{ 
	return txMath::rotateVector3(txMath::AXIS_Y, getRotation());
}