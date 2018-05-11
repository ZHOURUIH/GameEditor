#include "InputSystem.h"
#include "txRenderWindow.h"

InputSystem::InputSystem()
:
mCurMouseWheelDelta(0.0f),
mFrameState(FS_PRE_UPDATE)
{
	mMouseState.insert(MBD_LEFT, KEY_STATE(KPS_KEEP_UP));
	mMouseState.insert(MBD_RIGHT, KEY_STATE(KPS_KEEP_UP));
	mMouseState.insert(MBD_MIDDLE, KEY_STATE(KPS_KEEP_UP));
}

void InputSystem::preUpdate(const float& elapsedTime)
{
	// 根据mKeyDownBuffer和mKeyUpBuffer判断按键的当前按下
	// 预更新时更新所有按键的状态,主要是按键的CURRENT_DOWN和CURRENT_UP
	mFrameState = FS_PRE_UPDATE;
	auto iterDown = mKeyDownBuffer.begin();
	auto iterDownEnd = mKeyDownBuffer.end();
	FOR_STL(mKeyDownBuffer, ; iterDown != iterDownEnd; ++iterDown)
	{
		setKeyDown(*iterDown);
	}
	END_FOR_STL(mKeyDownBuffer);
	mKeyDownBuffer.clear();

	auto iterUp = mKeyUpBuffer.begin();
	auto iterUpEnd = mKeyUpBuffer.end();
	FOR_STL(mKeyUpBuffer, ; iterUp != iterUpEnd; ++iterUp)
	{
		setKeyUp(*iterUp);
	}
	END_FOR_STL(mKeyUpBuffer);
	mKeyUpBuffer.clear();
}

void InputSystem::update(const float& elapsedTime)
{
	mFrameState = FS_UPDATE;
	auto iter = mKeyState.begin();
	auto iterEnd = mKeyState.end();
	FOR_STL(mKeyState, ; iter != iterEnd; ++iter)
	{
		iter->second.mStateTime += elapsedTime;
	}
	END_FOR_STL(mKeyState);

	auto iterMouse = mMouseState.begin();
	auto iterMouseEnd = mMouseState.end();
	FOR_STL(mMouseState, ; iterMouse != iterMouseEnd; ++iterMouse)
	{
		iterMouse->second.mStateTime += elapsedTime;
	}
	END_FOR_STL(mMouseState);
}

void InputSystem::lateUpdate(const float& elapsedTime)
{
	// 更新按键状态,主要是KEEP_DOWN和KEEP_UP
	mFrameState = FS_LATE_UPDATE;
	auto iter = mKeyState.begin();
	auto iterEnd = mKeyState.end();
	FOR_STL(mKeyState, ; iter != iterEnd; ++iter)
	{
		if (iter->second.mState == KPS_CURRENT_UP)
		{
			iter->second.mState = KPS_KEEP_UP;
		}
		else if (iter->second.mState == KPS_CURRENT_DOWN)
		{
			iter->second.mState = KPS_KEEP_DOWN;
		}
	}
	END_FOR_STL(mKeyState);

	auto iterMouse = mMouseState.begin();
	auto iterMouseEnd = mMouseState.end();
	FOR_STL(mMouseState, ; iterMouse != iterMouseEnd; ++iterMouse)
	{
		if (iterMouse->second.mState == KPS_CURRENT_UP)
		{
			iterMouse->second.mState = KPS_KEEP_UP;
		}
		else if (iterMouse->second.mState == KPS_CURRENT_DOWN)
		{
			iterMouse->second.mState = KPS_KEEP_DOWN;
		}
	}
	END_FOR_STL(mMouseState);
	// 清空字符列表
	mCurCharList.clear();
	mCurMouseWheelDelta = 0.0f;
	mLastMousePosition = mCurMousePosition;
}

void InputSystem::keyDown(const int& key)
{
	mKeyDownBuffer.insert(key);
}

void InputSystem::keyUp(const int& key)
{
	if (mKeyDownBuffer.find(key) != mKeyDownBuffer.end())
	{
		mKeyDownBuffer.erase(mKeyDownBuffer.find(key));
	}
	mKeyUpBuffer.insert(key);
}

bool InputSystem::isKeyDown(const int& key)
{
	auto iter = mKeyState.find(key);
	if (iter != mKeyState.end())
	{
		return iter->second.mState == KPS_CURRENT_DOWN || iter->second.mState == KPS_KEEP_DOWN;
	}
	return false;
}

bool InputSystem::isKeyCurrentDown(const int& key)
{
	auto iter = mKeyState.find(key);
	if (iter != mKeyState.end())
	{
		return iter->second.mState == KPS_CURRENT_DOWN;
	}
	return false;
}

float InputSystem::getKeyTime(const int& key)
{
	auto iter = mKeyState.find(key);
	if (iter != mKeyState.end())
	{
		return iter->second.mStateTime;
	}
	return 0.0f;
}

void InputSystem::setCurMousePosition(const txIPoint& curPos)
{
	// 需要将坐标转换为以左下角为原点的坐标
	txIPoint transPos = curPos;
	transPos.y = mRenderWindow->getHeight() - transPos.y;
	mLastMousePosition = mCurMousePosition;
	mCurMousePosition = transPos;
}

void InputSystem::setKeyDown(const int& key)
{
	auto iter = mKeyState.find(key);
	if (iter == mKeyState.end())
	{
		KEY_STATE keyState;
		keyState.mStateTime = 0.0f;
		keyState.mState = KPS_CURRENT_DOWN;
		mKeyState.insert(key, keyState);
	}
	else
	{
		if (iter->second.mState == KPS_CURRENT_UP || iter->second.mState == KPS_KEEP_UP)
		{
			iter->second.mState = KPS_CURRENT_DOWN;
			iter->second.mStateTime = 0.0f;
		}
	}
}

void InputSystem::setKeyUp(const int& key)
{
	auto iter = mKeyState.find(key);
	if (iter == mKeyState.end())
	{
		KEY_STATE keyState;
		keyState.mStateTime = 0.0f;
		keyState.mState = KPS_CURRENT_UP;
		mKeyState.insert(key, keyState);
	}
	else
	{
		if (iter->second.mState == KPS_CURRENT_DOWN || iter->second.mState == KPS_KEEP_DOWN)
		{
			iter->second.mState = KPS_CURRENT_UP;
			iter->second.mStateTime = 0.0f;
		}
	}
}