#ifndef _INPUT_SYSTEM_H_
#define _INPUT_SYSTEM_H_

#include "txEngineDefine.h"
#include "txIPoint.h"
#include "txEngineBase.h"

enum FRAME_STATE
{
	FS_PRE_UPDATE,
	FS_UPDATE,
	FS_LATE_UPDATE,
};

class InputSystem : txEngineBase
{
public:
	InputSystem();
	virtual ~InputSystem(){}
	virtual void init(){}
	void preUpdate(const float& elapsedTime);
	void update(const float& elapsedTime);
	void lateUpdate(const float& elapsedTime);
	void keyDown(const int& key);
	void keyUp(const int& key);
	void onChar(const char& key){mCurCharList.insert(key); }
	txSet<int>& getCurCharList() { return mCurCharList; }
	const txIPoint& getMousePosition() { return mCurMousePosition; }
	const txIPoint& getLastMousePosition() { return mLastMousePosition; }
	const float& getMouseWheelDelta() { return mCurMouseWheelDelta; }
	KEY_STATE getMouseButtonState(const MOUSE_BUTTON_DEFINE& mouse)
	{
		auto iter = mMouseState.find(mouse);
		if (iter != mMouseState.end())
		{
			return iter->second;
		}
		return KEY_STATE();
	}
	bool isKeyDown(const int& key);
	bool isKeyCurrentDown(const int& key);
	float getKeyTime(const int& key);
	void mouseLeftDown(const txIPoint& position)
	{
		mMouseState[MBD_LEFT].mStateTime = 0.0f;
		mMouseState[MBD_LEFT].mState = KPS_CURRENT_DOWN;
		setCurMousePosition(position);
	}
	void mouseRightDown(const txIPoint& position)
	{
		mMouseState[MBD_RIGHT].mStateTime = 0.0f;
		mMouseState[MBD_RIGHT].mState = KPS_CURRENT_DOWN;
		setCurMousePosition(position);
	}
	void mouseMiddleDown(const txIPoint& position)
	{
		mMouseState[MBD_MIDDLE].mStateTime = 0.0f;
		mMouseState[MBD_MIDDLE].mState = KPS_CURRENT_DOWN;
		setCurMousePosition(position);
	}
	void mouseLeftUp(const txIPoint& position)
	{
		mMouseState[MBD_LEFT].mStateTime = 0.0f;
		mMouseState[MBD_LEFT].mState = KPS_CURRENT_UP;
		setCurMousePosition(position);
	}
	void mouseRightUp(const txIPoint& position)
	{
		mMouseState[MBD_RIGHT].mStateTime = 0.0f;
		mMouseState[MBD_RIGHT].mState = KPS_CURRENT_UP;
		setCurMousePosition(position);
	}
	void mouseMiddleUp(const txIPoint& position)
	{
		mMouseState[MBD_MIDDLE].mStateTime = 0.0f;
		mMouseState[MBD_MIDDLE].mState = KPS_CURRENT_UP;
		setCurMousePosition(position);
	}
	void mouseMove(const txIPoint& position)
	{
		setCurMousePosition(position); 
	}
	void mouseWheel(const float& delta)
	{
		mCurMouseWheelDelta = delta; 
	}
	void setCurMousePosition(const txIPoint& curPos);
protected:
	void setKeyDown(const int& key);
	void setKeyUp(const int& key);
protected:
	txMap<int, KEY_STATE> mKeyState;
	txMap<MOUSE_BUTTON_DEFINE, KEY_STATE> mMouseState;
	txIPoint mLastMousePosition;
	txIPoint mCurMousePosition;
	float mCurMouseWheelDelta;
	txSet<int> mCurCharList;	// 当前按下的字符列表
	txSet<int> mKeyDownBuffer;	// 所有处于当前按下状态的键
	txSet<int> mKeyUpBuffer;
	FRAME_STATE mFrameState;
};

#endif
