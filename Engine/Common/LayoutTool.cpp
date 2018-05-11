#include "txEngineDefine.h"
#include "Layout.h"
#include "WindowHeader.h"
#include "LayoutScript.h"
#include "txEngineLog.h"
#include "LayoutTool.h"
#include "LayoutManager.h"
#include "SceneProcedure.h"

// 布局
//------------------------------------------------------------------------------------------------------------------------------------
void LayoutTool::LOAD_LAYOUT(const int& type, const int& renderOrder, const bool& visible, const bool& immediately, const std::string& param)
{
	CommandLayoutManagerLoadLayout* cmd = NEW_CMD_INFO(cmd);
	cmd->mLayoutID = type;
	cmd->mVisible = visible;
	cmd->mRenderOrder = renderOrder;
	cmd->mAsync = false;
	cmd->mImmediatelyShow = immediately;
	cmd->mParam = param;
	mCommandSystem->pushCommand(cmd, mLayoutManager);
}
void LayoutTool::LOAD_LAYOUT_ASYNC(const int& type, const int& renderOrder, LayoutAsyncDone callback, void* userData)
{
	CommandLayoutManagerLoadLayout* cmd = NEW_CMD_INFO(cmd);
	cmd->mLayoutID = type;
	cmd->mRenderOrder = renderOrder;
	cmd->mAsync = true;
	cmd->mCallback = callback;
	cmd->mUserData = userData;
	mCommandSystem->pushCommand(cmd, mLayoutManager);
}
void LayoutTool::UNLOAD_LAYOUT(const int& type)
{
	// 需要首先强制隐藏布局
	HIDE_LAYOUT_FORCE(type);
	CommandLayoutManagerUnloadLayout* cmd = NEW_CMD_INFO(cmd);
	cmd->mLayoutID = type;
	mCommandSystem->pushCommand(cmd, mLayoutManager);
}
void LayoutTool::UNLOAD_LAYOUT_DELAY(const int& type, const float& delayTime)
{
	CommandLayoutManagerUnloadLayout* cmd = NEW_CMD_DELAY_INFO(cmd);
	cmd->mLayoutID = type;
	mCommandSystem->pushDelayCommand(cmd, mLayoutManager, delayTime);
}
void LayoutTool::LOAD_LAYOUT_HIDE(const int& type, const int& renderOrder)
{
	LOAD_LAYOUT(type, renderOrder, false, false, EMPTY_STRING);
}
void LayoutTool::LOAD_LAYOUT_SHOW(const int& type, const int& renderOrder)
{
	LOAD_LAYOUT(type, renderOrder, true, false, EMPTY_STRING);
}
void LayoutTool::LOAD_LAYOUT_SHOW(const int& type, const int& renderOrder, const bool& immediately, const std::string& param)
{
	LOAD_LAYOUT(type, renderOrder, true, immediately, param);
}
void LayoutTool::HIDE_LAYOUT(const int& type, const bool& immediately, const std::string& param)
{
	VISIBLE_LAYOUT(type, false, immediately, param);
}
void LayoutTool::HIDE_LAYOUT_FORCE(const int& type)
{
	VISIBLE_LAYOUT_FORCE(type, false);
}
void LayoutTool::SHOW_LAYOUT(const int& type, const bool& immediately, const std::string& param)
{
	VISIBLE_LAYOUT(type, true, immediately, param);
}
void LayoutTool::SHOW_LAYOUT_FORCE(const int& type)
{
	VISIBLE_LAYOUT_FORCE(type, true);
}
void LayoutTool::VISIBLE_LAYOUT(const int& type, const bool& visible, const bool& immediately, const std::string& param)
{
	CommandLayoutManagerLayoutVisible* cmd = NEW_CMD_INFO(cmd);
	cmd->mLayoutID = type;
	cmd->mForce = false;
	cmd->mVisibility = visible;
	cmd->mImmediately = immediately;
	cmd->mParam = param;
	mCommandSystem->pushCommand(cmd, mLayoutManager);
}
void LayoutTool::VISIBLE_LAYOUT_FORCE(const int& type, const bool& visible)
{
	CommandLayoutManagerLayoutVisible* cmd = NEW_CMD_INFO(cmd);
	cmd->mLayoutID = type;
	cmd->mForce = true;
	cmd->mVisibility = visible;
	cmd->mImmediately = false;
	mCommandSystem->pushCommand(cmd, mLayoutManager);
}
CommandLayoutManagerLayoutVisible* LayoutTool::HIDE_LAYOUT_DELAY(SceneProcedure* procedure, float delayTime, const int& type, const bool& immediately, const std::string& param)
{
	return VISIBLE_LAYOUT_DELAY(procedure, delayTime, type, false, immediately, param);
}
CommandLayoutManagerLayoutVisible* LayoutTool::HIDE_LAYOUT_DELAY_FORCE(float delayTime, const int& type)
{
	return VISIBLE_LAYOUT_DELAY_FORCE(delayTime, type, false);
}
CommandLayoutManagerLayoutVisible* LayoutTool::SHOW_LAYOUT_DELAY(SceneProcedure* procedure, float delayTime, const int& type, const bool& immediately, const std::string& param)
{
	return VISIBLE_LAYOUT_DELAY(procedure, delayTime, type, true, immediately, param);
}
CommandLayoutManagerLayoutVisible* LayoutTool::SHOW_LAYOUT_DELAY_EX(SceneProcedure* procedure, float delayTime, const int& type, CommandCallback start, void* userData, const bool& immediately, const std::string& param)
{
	return VISIBLE_LAYOUT_DELAY_EX(procedure, delayTime, type, true, start, userData, immediately, param);
}
CommandLayoutManagerLayoutVisible* LayoutTool::SHOW_LAYOUT_DELAY_FORCE(float delayTime, const int& type)
{
	return VISIBLE_LAYOUT_DELAY_FORCE(delayTime, type, true);
}
CommandLayoutManagerLayoutVisible* LayoutTool::VISIBLE_LAYOUT_DELAY(SceneProcedure* procedure, float delayTime, const int& type, const bool& visible, const bool& immediately, const std::string& param)
{
	CommandLayoutManagerLayoutVisible* cmd = NEW_CMD_DELAY_INFO(cmd);
	cmd->mLayoutID = type;
	cmd->mForce = false;
	cmd->mVisibility = visible;
	cmd->mImmediately = immediately;
	cmd->mParam = param;
	mCommandSystem->pushDelayCommand(cmd, mLayoutManager, delayTime);
	if (procedure != NULL)
	{
		procedure->addDelayCmd(cmd);
	}
	return cmd;
}
CommandLayoutManagerLayoutVisible* LayoutTool::VISIBLE_LAYOUT_DELAY_EX(SceneProcedure* procedure, float delayTime, const int& type, const bool& visible, CommandCallback start, void* userData, const bool& immediately, const std::string& param)
{
	CommandLayoutManagerLayoutVisible* cmd = NEW_CMD_DELAY_INFO(cmd);
	cmd->mLayoutID = type;
	cmd->mForce = false;
	cmd->mVisibility = visible;
	cmd->mImmediately = immediately;
	cmd->mParam = param;
	cmd->addStartCommandCallback(start, userData);
	mCommandSystem->pushDelayCommand(cmd, mLayoutManager, delayTime);
	if (procedure != NULL)
	{
		procedure->addDelayCmd(cmd);
	}
	return cmd;
}
CommandLayoutManagerLayoutVisible* LayoutTool::VISIBLE_LAYOUT_DELAY_FORCE(float delayTime, const int& type, const bool& visible)
{
	CommandLayoutManagerLayoutVisible* cmd = NEW_CMD_DELAY_INFO(cmd);
	cmd->mLayoutID = type;
	cmd->mForce = true;
	cmd->mVisibility = visible;
	cmd->mImmediately = false;
	mCommandSystem->pushDelayCommand(cmd, mLayoutManager, delayTime);
	return cmd;
}
//--------------------------------------------------------------------------------------------------------------------------------------------
// 旋转
void LayoutTool::ROTATE_WINDOW(Window* obj, const VECTOR3& rotation)
{
	CommandWindowRotate* cmd = NEW_CMD(cmd);
	cmd->mName = EMPTY_STRING;
	cmd->mOnceLength = 0.0f;
	cmd->mStartAngle = rotation;
	cmd->mTargetAngle = rotation;
	mCommandSystem->pushCommand(cmd, obj);
}
void LayoutTool::ROTATE_KEYFRMAE_WINDOW_EX(Window* obj, const std::string& keyframe, const VECTOR3& start, const VECTOR3& target,
	const float& onceLength, const bool& loop, const float& offset, KeyFrameCallback doingCallback, void* doingUserData,
	KeyFrameCallback doneCallback, void* doneUserData)
{
	if (keyframe == EMPTY_STRING || txMath::isFloatZero(onceLength))
	{
		ENGINE_ERROR("时间或关键帧不能为空,如果要停止组件,请使用void ROTATE_WINDOW(txUIObject obj, Vector3 rotation)");
		return;
	}
	CommandWindowRotate* cmd = NEW_CMD(cmd);
	cmd->mName = keyframe;
	cmd->mOnceLength = onceLength;
	cmd->mStartAngle = start;
	cmd->mTargetAngle = target;
	cmd->mTimeOffset = offset;
	cmd->mLoop = loop;
	cmd->setDoingCallback(doingCallback, doingUserData);
	cmd->setDoneCallback(doneCallback, doneUserData);
	mCommandSystem->pushCommand(cmd, obj);
}
CommandWindowRotate* LayoutTool::ROTATE_WINDOW_DELAY(LayoutScript* script, Window* obj, const float& delayTime, const VECTOR3& rotation)
{
	CommandWindowRotate* cmd = NEW_CMD_DELAY(cmd);
	cmd->mName = EMPTY_STRING;
	cmd->mOnceLength = 0.0f;
	cmd->mStartAngle = rotation;
	cmd->mTargetAngle = rotation;
	mCommandSystem->pushDelayCommand(cmd, obj, delayTime);
	script->addDelayCmd(cmd);
	return cmd;
}
void LayoutTool::ROTATE_WINDOW(Window* obj, const VECTOR3& start, const VECTOR3& target, const float& time)
{
	ROTATE_KEYFRMAE_WINDOW_EX(obj, ZERO_ONE, start, target, time, false, 0.0f, NULL, NULL, NULL, NULL);
}
void LayoutTool::ROTATE_WINDOW_EX(Window* obj, const VECTOR3& start, const VECTOR3& target, const float& time,
	KeyFrameCallback doingCallback, void* doingUserData, KeyFrameCallback doneCallback, void* doneUserData)
{
	ROTATE_KEYFRMAE_WINDOW_EX(obj, ZERO_ONE, start, target, time, false, 0.0f, doingCallback, doingUserData, doneCallback, doneUserData);
}
void LayoutTool::ROTATE_WINDOW_EX(Window* obj, const VECTOR3& start, const VECTOR3& target, const float& time, KeyFrameCallback doneCallback, void* doneUserData)
{
	ROTATE_KEYFRMAE_WINDOW_EX(obj, ZERO_ONE, start, target, time, false, 0.0f, NULL, NULL, doneCallback, doneUserData);
}
void LayoutTool::ROTATE_KEYFRMAE_WINDOW(Window* obj, const std::string& keyframe, const VECTOR3& start, const VECTOR3& target, const float& onceLength)
{
	ROTATE_KEYFRMAE_WINDOW_EX(obj, keyframe, start, target, onceLength, false, 0.0f, NULL, NULL, NULL, NULL);
}
void LayoutTool::ROTATE_KEYFRMAE_WINDOW(Window* obj, const std::string& keyframe, const VECTOR3& start, const VECTOR3& target,
	const float& onceLength, const bool& loop, const float& offset)
{
	ROTATE_KEYFRMAE_WINDOW_EX(obj, keyframe, start, target, onceLength, loop, offset, NULL, NULL, NULL, NULL);
}
CommandWindowRotate* LayoutTool::ROTATE_WINDOW_DELAY(LayoutScript* script, Window* obj, const float& delayTime, const VECTOR3& start, const VECTOR3& target, const float& time)
{
	return ROTATE_KEYFRMAE_WINDOW_DELAY(script, obj, delayTime, ZERO_ONE, start, target, time, false, 0.0f);
}
CommandWindowRotate* LayoutTool::ROTATE_KEYFRMAE_WINDOW_DELAY(LayoutScript* script, Window* obj, const float& delayTime, const std::string& keyframe,
	const VECTOR3& start, const VECTOR3& target, const float& onceLength)
{
	return ROTATE_KEYFRMAE_WINDOW_DELAY(script, obj, delayTime, keyframe, start, target, onceLength, false, 0.0f);
}
CommandWindowRotate* LayoutTool::ROTATE_KEYFRMAE_WINDOW_DELAY(LayoutScript* script, Window* obj, const float& delayTime, const std::string& keyframe,
	const VECTOR3& start, const VECTOR3& target, const float& onceLength, const bool& loop)
{
	return ROTATE_KEYFRMAE_WINDOW_DELAY(script, obj, delayTime, keyframe, start, target, onceLength, loop, 0.0f);
}
CommandWindowRotate* LayoutTool::ROTATE_KEYFRMAE_WINDOW_DELAY(LayoutScript* script, Window* obj, const float& delayTime, const std::string& keyframe,
	const VECTOR3& start, const VECTOR3& target, const float& onceLength, const bool& loop, const float& offset)
{
	if (keyframe == EMPTY_STRING || txMath::isFloatZero(onceLength))
	{
		ENGINE_ERROR("时间或关键帧不能为空,如果要停止组件,请使用CommandWindowKeyFrameRotate* ROTATE_WINDOW_DELAY(LayoutScript* script, Window* obj, const float& delayTime, const VECTOR3& rotation)");
		return NULL;
	}
	CommandWindowRotate* cmd = NEW_CMD_DELAY(cmd);
	cmd->mName = keyframe;
	cmd->mOnceLength = onceLength;
	cmd->mStartAngle = start;
	cmd->mTargetAngle = target;
	cmd->mTimeOffset = offset;
	cmd->mLoop = loop;
	mCommandSystem->pushDelayCommand(cmd, obj, delayTime);
	script->addDelayCmd(cmd);
	return cmd;
}
void LayoutTool::ROTATE_SPEED_WINDOW(Window* obj)
{
	ROTATE_SPEED_WINDOW(obj, txMath::VEC3_ZERO, txMath::VEC3_ZERO, txMath::VEC3_ZERO);
}
void LayoutTool::ROTATE_SPEED_WINDOW(Window* obj, const VECTOR3& speed)
{
	ROTATE_SPEED_WINDOW(obj, speed, txMath::VEC3_ZERO, txMath::VEC3_ZERO);
}
void LayoutTool::ROTATE_SPEED_WINDOW(Window* obj, const VECTOR3& speed, const VECTOR3& startAngle)
{
	ROTATE_SPEED_WINDOW(obj, speed, startAngle, txMath::VEC3_ZERO);
}
void LayoutTool::ROTATE_SPEED_WINDOW(Window* obj, const VECTOR3& speed, const VECTOR3& startAngle, const VECTOR3& rotateAccelerationValue)
{
	CommandWindowRotateSpeed* cmd = NEW_CMD(cmd);
	cmd->mRotateSpeed = speed;
	cmd->mStartAngle = startAngle;
	cmd->mRotateAcceleration = rotateAccelerationValue;
	mCommandSystem->pushCommand(cmd, obj);
}
CommandWindowRotateSpeed* LayoutTool::ROTATE_SPEED_WINDOW_DELAY(LayoutScript* script, Window* obj, const float& delayTime,
	const VECTOR3& speed)
{
	return ROTATE_SPEED_WINDOW_DELAY(script, obj, delayTime, speed, txMath::VEC3_ZERO, txMath::VEC3_ZERO);
}
CommandWindowRotateSpeed* LayoutTool::ROTATE_SPEED_WINDOW_DELAY(LayoutScript* script, Window* obj, const float& delayTime,
	const VECTOR3& speed, const VECTOR3& startAngle)
{
	return ROTATE_SPEED_WINDOW_DELAY(script, obj, delayTime, speed, startAngle, txMath::VEC3_ZERO);
}
CommandWindowRotateSpeed* LayoutTool::ROTATE_SPEED_WINDOW_DELAY(LayoutScript* script, Window* obj, const float& delayTime,
	const VECTOR3& speed, const VECTOR3& startAngle, const VECTOR3& rotateAccelerationValue)
{
	CommandWindowRotateSpeed* cmd = NEW_CMD_DELAY(cmd);
	cmd->mRotateSpeed = speed;
	cmd->mStartAngle = startAngle;
	cmd->mRotateAcceleration = rotateAccelerationValue;
	mCommandSystem->pushDelayCommand(cmd, obj, delayTime);
	script->addDelayCmd(cmd);
	return cmd;
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------
// 移动
// 用关键帧移动窗口
void LayoutTool::MOVE_WINDOW(Window* obj, const txPoint& pos)
{
	CommandWindowMove* cmd = NEW_CMD(cmd);
	cmd->mName = EMPTY_STRING;
	cmd->mOnceLength = 0.0f;
	cmd->mStartPosition = pos;
	cmd->mTargetPosition = pos;
	mCommandSystem->pushCommand(cmd, obj);
}
void LayoutTool::MOVE_KEYFRAME_WINDOW_EX(Window* obj, const std::string& fileName, const txPoint& startPos, const txPoint& targetPos, const float& onceLength,
	const bool& loop, const float& offset, KeyFrameCallback doingCallback, void* doingUserData, KeyFrameCallback doneCallBack, void* doneUserData)
{
	if (fileName == EMPTY_STRING || txMath::isFloatZero(onceLength))
	{
		ENGINE_ERROR("时间或关键帧不能为空,如果要停止组件,请使用void MOVE_WINDOW(Window* obj, const txPoint& pos)");
		return;
	}
	CommandWindowMove* cmd = NEW_CMD(cmd);
	cmd->mName = fileName;
	cmd->mOnceLength = onceLength;
	cmd->mStartPosition = startPos;
	cmd->mTargetPosition = targetPos;
	cmd->mTimeOffset = offset;
	cmd->mLoop = loop;
	cmd->setDoingCallback(doingCallback, doingUserData);
	cmd->setDoneCallback(doneCallBack, doneUserData);
	mCommandSystem->pushCommand(cmd, obj);
}
void LayoutTool::MOVE_WINDOW(Window* obj, const txPoint& start, const txPoint& target, const float& onceLength)
{
	MOVE_KEYFRAME_WINDOW_EX(obj, ZERO_ONE, start, target, onceLength, false, 0.0f, NULL, NULL, NULL, NULL);
}
void LayoutTool::MOVE_WINDOW_EX(Window* obj, const txPoint& start, const txPoint& target, const float& onceLength,
	KeyFrameCallback doneCallback, void* doneUserData)
{
	MOVE_KEYFRAME_WINDOW_EX(obj, ZERO_ONE, start, target, onceLength, false, 0.0f, NULL, NULL, doneCallback, doneUserData);
}
void LayoutTool::MOVE_WINDOW_EX(Window* obj, const txPoint& start, const txPoint& target, const float& onceLength,
	KeyFrameCallback doingCallback, void* doingUserData, KeyFrameCallback doneCallback, void* doneUserData)
{
	MOVE_KEYFRAME_WINDOW_EX(obj, ZERO_ONE, start, target, onceLength, false, 0.0f, doingCallback, doingUserData, doneCallback, doneUserData);
}
void LayoutTool::MOVE_WINDOW_EX(Window* obj, const txPoint& start, const txPoint& target, const float& onceLength, const float& offsetTime,
	KeyFrameCallback doneCallback, void* doneUserData)
{
	MOVE_KEYFRAME_WINDOW_EX(obj, ZERO_ONE, start, target, onceLength, false, offsetTime, NULL, NULL, doneCallback, doneUserData);
}
void LayoutTool::MOVE_WINDOW_EX(Window* obj, const txPoint& start, const txPoint& target, const float& onceLength, const float& offsetTime,
	KeyFrameCallback doingCallback, void* doingUserData, KeyFrameCallback doneCallback, void* doneUserData)
{
	MOVE_KEYFRAME_WINDOW_EX(obj, ZERO_ONE, start, target, onceLength, false, offsetTime, doingCallback, doingUserData, doneCallback, doneUserData);
}
void LayoutTool::MOVE_KEYFRAME_WINDOW(Window* obj, const std::string& fileName, const txPoint& startPos, const txPoint& targetPos, const float& onceLength)
{
	MOVE_KEYFRAME_WINDOW_EX(obj, fileName, startPos, targetPos, onceLength, false, 0.0f, NULL, NULL, NULL, NULL);
}
void LayoutTool::MOVE_KEYFRAME_WINDOW(Window* obj, const std::string& fileName, const txPoint& startPos, const txPoint& targetPos, const float& onceLength, const bool& loop)
{
	MOVE_KEYFRAME_WINDOW_EX(obj, fileName, startPos, targetPos, onceLength, loop, 0.0f, NULL, NULL, NULL, NULL);
}
void LayoutTool::MOVE_KEYFRAME_WINDOW(Window* obj, const std::string& fileName, const txPoint& startPos, const txPoint& targetPos, const float& onceLength, const bool& loop, const float& offset)
{
	MOVE_KEYFRAME_WINDOW_EX(obj, fileName, startPos, targetPos, onceLength, loop, offset, NULL, NULL, NULL, NULL);
}
void LayoutTool::MOVE_KEYFRAME_WINDOW_EX(Window* obj, const std::string& fileName, const txPoint& startPos, const txPoint& targetPos, const float& onceLength,
	KeyFrameCallback TremblingCallBack, void* doingUserData, KeyFrameCallback doneCallBack, void* doneUserData)
{
	MOVE_KEYFRAME_WINDOW_EX(obj, fileName, startPos, targetPos, onceLength, false, 0.0f, TremblingCallBack, doingUserData, doneCallBack, doneUserData);
}
void LayoutTool::MOVE_KEYFRAME_WINDOW_EX(Window* obj, const std::string& fileName, const txPoint& startPos, const txPoint& targetPos, const float& onceLength,
	const bool& loop, KeyFrameCallback doingCallback, void* doingUserData, KeyFrameCallback doneCallBack, void* doneUserData)
{
	MOVE_KEYFRAME_WINDOW_EX(obj, fileName, startPos, targetPos, onceLength, loop, 0.0f, doingCallback, doingUserData, doneCallBack, doneUserData);
}
CommandWindowMove* LayoutTool::MOVE_KEYFRAME_WINDOW_DELAY_EX(LayoutScript* script, Window* obj, const float& delayTime, const std::string& keyframe, const txPoint& startPos, const txPoint& targetPos,
	const float& onceLength, const bool& loop, const float& offset, KeyFrameCallback doingCallback, void* doingUserData, KeyFrameCallback doneCallback, void* doneUserData)
{
	if (keyframe == "" || txMath::isFloatZero(onceLength))
	{
		ENGINE_ERROR("时间或关键帧不能为空,如果要停止组件,请使用CommandWindowMove* MOVE_WINDOW_DELAY(LayoutScript script, Window* obj, const float& delayTime, const txPoint& pos)");
		return NULL;
	}
	CommandWindowMove* cmd = NEW_CMD_DELAY(cmd);
	cmd->mName = keyframe;
	cmd->mStartPosition = startPos;
	cmd->mTargetPosition = targetPos;
	cmd->mOnceLength = onceLength;
	cmd->mTimeOffset = offset;
	cmd->mLoop = loop;
	cmd->setDoingCallback(doingCallback, doingUserData);
	cmd->setDoneCallback(doneCallback, doneUserData);
	mCommandSystem->pushDelayCommand(cmd, obj, delayTime);
	script->addDelayCmd(cmd);
	return cmd;
}
CommandWindowMove* LayoutTool::MOVE_WINDOW_DELAY(LayoutScript* script, Window* obj, const float& delayTime, const txPoint& pos)
{
	CommandWindowMove* cmd = NEW_CMD_DELAY(cmd);
	cmd->mName = EMPTY_STRING;
	cmd->mStartPosition = pos;
	cmd->mTargetPosition = pos;
	cmd->mOnceLength = 0.0f;
	mCommandSystem->pushDelayCommand(cmd, obj, delayTime);
	script->addDelayCmd(cmd);
	return cmd;
}
CommandWindowMove* LayoutTool::MOVE_WINDOW_DELAY(LayoutScript* script, Window* obj, const float& delayTime, const txPoint& start, const txPoint& target, const float& onceLength)
{
	return MOVE_KEYFRAME_WINDOW_DELAY_EX(script, obj, delayTime, ZERO_ONE, start, target, onceLength, false, 0.0f, NULL, NULL, NULL, NULL);
}
CommandWindowMove* LayoutTool::MOVE_WINDOW_DELAY_EX(LayoutScript* script, Window* obj, const float& delayTime, const txPoint& start, const txPoint& target,
	const float& onceLength, KeyFrameCallback moveDoneCallback, void* doneUserData)
{
	return MOVE_KEYFRAME_WINDOW_DELAY_EX(script, obj, delayTime, ZERO_ONE, start, target, onceLength, false, 0.0f, NULL, NULL, moveDoneCallback, doneUserData);
}
CommandWindowMove* LayoutTool::MOVE_WINDOW_DELAY_EX(LayoutScript* script, Window* obj, const float& delayTime, const txPoint& start, const txPoint& target,
	const float& onceLength, KeyFrameCallback doingCallback, void* doingUserData, KeyFrameCallback moveDoneCallback, void* doneUserData)
{
	return MOVE_KEYFRAME_WINDOW_DELAY_EX(script, obj, delayTime, ZERO_ONE, start, target, onceLength, false, 0.0f, doingCallback, doingUserData, moveDoneCallback, doneUserData);
}
CommandWindowMove* LayoutTool::MOVE_KEYFRAME_WINDOW_DELAY(LayoutScript* script, Window* obj, const float& delayTime, const std::string& keyframe, const txPoint& startPos,
	const txPoint& targetPos, const float& onceLength)
{
	return MOVE_KEYFRAME_WINDOW_DELAY_EX(script, obj, delayTime, keyframe, startPos, targetPos, onceLength, false, 0.0f, NULL, NULL, NULL, NULL);
}
CommandWindowMove* LayoutTool::MOVE_KEYFRAME_WINDOW_DELAY(LayoutScript* script, Window* obj, const float& delayTime, const std::string& keyframe, const txPoint& startPos,
	const txPoint& targetPos, const float& onceLength, const bool& loop)
{
	return MOVE_KEYFRAME_WINDOW_DELAY_EX(script, obj, delayTime, keyframe, startPos, targetPos, onceLength, loop, 0.0f, NULL, NULL, NULL, NULL);
}
CommandWindowMove* LayoutTool::MOVE_KEYFRAME_WINDOW_DELAY(LayoutScript* script, Window* obj, const float& delayTime, const std::string& keyframe, const txPoint& startPos,
	const txPoint& targetPos, const float& onceLength, const bool& loop, const float& offset)
{
	return MOVE_KEYFRAME_WINDOW_DELAY_EX(script, obj, delayTime, keyframe, startPos, targetPos, onceLength, loop, offset, NULL, NULL, NULL, NULL);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------
// 显示
void LayoutTool::ACTIVE_WINDOW(Window* obj, const bool& active)
{
	CommandWindowActive* cmd = NEW_CMD(cmd);
	cmd->mActive = active;
	mCommandSystem->pushCommand(cmd, obj);
}
CommandWindowActive* LayoutTool::ACTIVE_WINDOW_DELAY(LayoutScript* script, Window* obj, const bool& active, const float& delayTime)
{
	return ACTIVE_WINDOW_DELAY_EX(script, obj, active, delayTime, NULL, NULL);
}
CommandWindowActive* LayoutTool::ACTIVE_WINDOW_DELAY_EX(LayoutScript* script, Window* obj, const bool& active, const float& dealyTime, CommandCallback startCallback, void* userData)
{
	CommandWindowActive* cmd = NEW_CMD_DELAY(cmd);
	cmd->mActive = active;
	cmd->addStartCommandCallback(startCallback, userData);
	mCommandSystem->pushDelayCommand(cmd, obj, dealyTime);
	script->addDelayCmd(cmd);
	return cmd;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------
// 缩放
void LayoutTool::SCALE_WINDOW(Window* obj, const VECTOR2& scale)
{
	CommandWindowScale* cmd = NEW_CMD(cmd);
	cmd->mName = "";
	cmd->mOnceLength = 0.0f;
	cmd->mStartScale = scale;
	cmd->mTargetScale = scale;
	mCommandSystem->pushCommand(cmd, obj);
}
void LayoutTool::SCALE_KEYFRAME_WINDOW_EX(Window* obj, const std::string& fileName, const VECTOR2& start, const VECTOR2& target, const float& onceLength,
	const bool& loop, const float& offset, const bool& fullOnce, KeyFrameCallback doingCallback, void* doingUserData, KeyFrameCallback doneCallback, void* doneUserData)
{
	if (fileName == EMPTY_STRING || txMath::isFloatZero(onceLength))
	{
		ENGINE_ERROR("时间或关键帧不能为空,如果要停止组件,请使用void SCALE_WINDOW(Window* obj, const VECTOR2& scale)");
		return;
	}
	CommandWindowScale* cmd = NEW_CMD(cmd);
	cmd->mName = fileName;
	cmd->mOnceLength = onceLength;
	cmd->mTimeOffset = offset;
	cmd->mLoop = loop;
	cmd->mFullOnce;
	cmd->mStartScale = start;
	cmd->mTargetScale = target;
	cmd->setDoingCallback(doingCallback, doingUserData);
	cmd->setDoneCallback(doneCallback, doneUserData);
	mCommandSystem->pushCommand(cmd, obj);
}
void LayoutTool::SCALE_WINDOW(Window* obj, const VECTOR2& start, const VECTOR2& target, const float& onceLength)
{
	SCALE_KEYFRAME_WINDOW_EX(obj, ZERO_ONE, start, target, onceLength, false, 0.0f, false, NULL, NULL, NULL, NULL);
}
void LayoutTool::SCALE_WINDOW_EX(Window* obj, const VECTOR2& start, const VECTOR2& target, const float& onceLength,
	KeyFrameCallback doneCallback, void* doneUserData)
{
	SCALE_KEYFRAME_WINDOW_EX(obj, ZERO_ONE, start, target, onceLength, false, 0.0f, false, NULL, NULL, doneCallback, doneUserData);
}
void LayoutTool::SCALE_WINDOW_EX(Window* obj, const VECTOR2& start, const VECTOR2& target, const float& onceLength,
	KeyFrameCallback doingCallback, void* doingUserData, KeyFrameCallback doneCallback, void* doneUserData)
{
	SCALE_KEYFRAME_WINDOW_EX(obj, ZERO_ONE, start, target, onceLength, false, 0.0f, false, doingCallback, doingUserData, doneCallback, doneUserData);
}
void LayoutTool::SCALE_KEYFRAME_WINDOW(Window* obj, const std::string& fileName, const VECTOR2& start, const VECTOR2& target, const float& onceLength)
{
	SCALE_KEYFRAME_WINDOW_EX(obj, fileName, start, target, onceLength, false, 0.0f, false, NULL, NULL, NULL, NULL);
}
void LayoutTool::SCALE_KEYFRAME_WINDOW(Window* obj, const std::string& fileName, const VECTOR2& start, const VECTOR2& target,
	const float& onceLength, const bool& loop)
{
	SCALE_KEYFRAME_WINDOW_EX(obj, fileName, start, target, onceLength, loop, 0.0f, false, NULL, NULL, NULL, NULL);
}
void LayoutTool::SCALE_KEYFRAME_WINDOW(Window* obj, const std::string& fileName, const VECTOR2& start, const VECTOR2& target,
	const float& onceLength, const bool& loop, const float& offset)
{
	SCALE_KEYFRAME_WINDOW_EX(obj, fileName, start, target, onceLength, loop, offset, false, NULL, NULL, NULL, NULL);
}
void LayoutTool::SCALE_KEYFRAME_WINDOW_EX(Window* obj, const std::string& fileName, const VECTOR2& start, const VECTOR2& target,
	const float& onceLength, KeyFrameCallback doneCallback, void* doneUserData)
{
	SCALE_KEYFRAME_WINDOW_EX(obj, fileName, start, target, onceLength, false, 0.0f, false, NULL, NULL, doneCallback, doneUserData);
}
void LayoutTool::SCALE_KEYFRAME_WINDOW_EX(Window* obj, const std::string& fileName, const VECTOR2& start, const VECTOR2& target,
	const float& onceLength, const bool& loop, KeyFrameCallback doneCallback, void* doneUserData)
{
	SCALE_KEYFRAME_WINDOW_EX(obj, fileName, start, target, onceLength, loop, 0.0f, false, NULL, NULL, doneCallback, doneUserData);
}
void LayoutTool::SCALE_KEYFRAME_WINDOW_EX(Window* obj, const std::string& fileName, const VECTOR2& start, const VECTOR2& target,
	const float& onceLength, const bool& loop, const float& offset, KeyFrameCallback doneCallback, void* doneUserData)
{
	SCALE_KEYFRAME_WINDOW_EX(obj, fileName, start, target, onceLength, loop, offset, false, NULL, NULL, doneCallback, doneUserData);
}
void LayoutTool::SCALE_KEYFRAME_WINDOW_EX(Window* obj, const std::string& fileName, const VECTOR2& start, const VECTOR2& target, const float& onceLength,
	KeyFrameCallback doingCallback, void* doingUserData, KeyFrameCallback doneCallback, void* doneUserData)
{
	SCALE_KEYFRAME_WINDOW_EX(obj, fileName, start, target, onceLength, false, 0.0f, false, doingCallback, doingUserData, doneCallback, doneUserData);
}
void LayoutTool::SCALE_KEYFRAME_WINDOW_EX(Window* obj, const std::string& fileName, const VECTOR2& start, const VECTOR2& target, const float& onceLength, const bool& loop,
	KeyFrameCallback doingCallback, void* doingUserData, KeyFrameCallback doneCallback, void* doneUserData)
{
	SCALE_KEYFRAME_WINDOW_EX(obj, fileName, start, target, onceLength, loop, 0.0f, false, doingCallback, doingUserData, doneCallback, doneUserData);
}
CommandWindowScale* LayoutTool::SCALE_WINDOW_DELAY(LayoutScript* script, Window* obj, const float& delayTime, const VECTOR2& scale)
{
	CommandWindowScale* cmd = NEW_CMD_DELAY(cmd);
	cmd->mName = EMPTY_STRING;
	cmd->mOnceLength = 0.0f;
	cmd->mStartScale = scale;
	cmd->mTargetScale = scale;
	mCommandSystem->pushDelayCommand(cmd, obj, delayTime);
	script->addDelayCmd(cmd);
	return cmd;
}
CommandWindowScale* LayoutTool::SCALE_KEYFRAME_WINDOW_DELAY_EX(LayoutScript* script, Window* obj, const float& delayTime, const std::string& keyframe,
	const VECTOR2& start, const VECTOR2& target, const float& onceLength, const bool& loop, const float& offset, const bool& fullOnce, 
	KeyFrameCallback doingCallback, void* doingUserData, KeyFrameCallback doneCallback, void* doneUserData)
{
	if (keyframe == EMPTY_STRING || txMath::isFloatZero(onceLength))
	{
		ENGINE_ERROR("时间或关键帧不能为空,如果要停止组件请使用CommandWindowScale* SCALE_WINDOW_DELAY(LayoutScript* script, Window* obj, const float& delayTime, const VECTOR2& scale)");
		return NULL;
	}
	CommandWindowScale* cmd = NEW_CMD_DELAY(cmd);
	cmd->mName = keyframe;
	cmd->mOnceLength = onceLength;
	cmd->mTimeOffset = offset;
	cmd->mLoop = loop;
	cmd->mStartScale = start;
	cmd->mTargetScale = target;
	cmd->setDoingCallback(doingCallback, doingUserData);
	cmd->setDoneCallback(doneCallback, doneUserData);
	mCommandSystem->pushDelayCommand(cmd, obj, delayTime);
	script->addDelayCmd(cmd);
	return cmd;
}
CommandWindowScale* LayoutTool::SCALE_WINDOW_DELAY(LayoutScript* script, Window* obj, const float& delayTime, const VECTOR2& start,
	const VECTOR2& target, const float& onceLength)
{
	return SCALE_KEYFRAME_WINDOW_DELAY_EX(script, obj, delayTime, ZERO_ONE, start, target, onceLength, false, 0.0f, false, NULL, NULL, NULL, NULL);
}
CommandWindowScale* LayoutTool::SCALE_WINDOW_DELAY_EX(LayoutScript* script, Window* obj, const float& delayTime, const VECTOR2& start,
	const VECTOR2& target, const float& onceLength, KeyFrameCallback doingCallback, void* doingUserData, KeyFrameCallback doneCallback, void* doneUserData)
{
	return SCALE_KEYFRAME_WINDOW_DELAY_EX(script, obj, delayTime, ZERO_ONE, start, target, onceLength, false, 0.0f, false, doingCallback, doingUserData, doneCallback, doneUserData);
}
CommandWindowScale* LayoutTool::SCALE_KEYFRAME_WINDOW_DELAY(LayoutScript* script, Window* obj, const float& delayTime, const std::string& keyframe,
	const VECTOR2& start, const VECTOR2& target, const float& onceLength)
{
	return SCALE_KEYFRAME_WINDOW_DELAY_EX(script, obj, delayTime, keyframe, start, target, onceLength, false, 0.0f, false, NULL, NULL, NULL, NULL);
}
CommandWindowScale* LayoutTool::SCALE_KEYFRAME_WINDOW_DELAY(LayoutScript* script, Window* obj, const float& delayTime, const std::string& keyframe,
	const VECTOR2& start, const VECTOR2& target, const float& onceLength, const bool& loop)
{
	return SCALE_KEYFRAME_WINDOW_DELAY_EX(script, obj, delayTime, keyframe, start, target, onceLength, loop, 0.0f, false, NULL, NULL, NULL, NULL);
}
CommandWindowScale* LayoutTool::SCALE_KEYFRAME_WINDOW_DELAY(LayoutScript* script, Window* obj, const float& delayTime, const std::string& keyframe,
	const VECTOR2& start, const VECTOR2& target, const float& onceLength, const bool& loop, const float& offset)
{
	return SCALE_KEYFRAME_WINDOW_DELAY_EX(script, obj, delayTime, keyframe, start, target, onceLength, loop, offset, false, NULL, NULL, NULL, NULL);
}
CommandWindowScale* LayoutTool::SCALE_KEYFRAME_WINDOW_DELAY_EX(LayoutScript* script, Window* obj, const float& delayTime, const std::string& keyframe,
	const VECTOR2& start, const VECTOR2& target, const float& onceLength, KeyFrameCallback doingCallback, void* doingUserData, KeyFrameCallback doneCallback, void* doneUserData)
{
	return SCALE_KEYFRAME_WINDOW_DELAY_EX(script, obj, delayTime, keyframe, start, target, onceLength, false, 0.0f, false, doingCallback, doingUserData, doneCallback, doneUserData);
}
CommandWindowScale* LayoutTool::SCALE_KEYFRAME_WINDOW_DELAY_EX(LayoutScript* script, Window* obj, const float& delayTime, const std::string& keyframe,
	const VECTOR2& start, const VECTOR2& target, const float& onceLength, const bool& loop, KeyFrameCallback doingCallback, void* doingUserData, KeyFrameCallback doneCallback, void* doneUserData)
{
	return SCALE_KEYFRAME_WINDOW_DELAY_EX(script, obj, delayTime, keyframe, start, target, onceLength, loop, 0.0f, false, doingCallback, doingUserData, doneCallback, doneUserData);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 透明度
void LayoutTool::ALPHA_WINDOW(Window* obj, const float& alpha)
{
	CommandWindowAlpha* cmd = NEW_CMD(cmd);
	cmd->mName = EMPTY_STRING;
	cmd->mOnceLength = 0.0f;
	cmd->mStartAlpha = alpha;
	cmd->mTargetAlpha = alpha;
	mCommandSystem->pushCommand(cmd, obj);
}
void LayoutTool::ALPHA_KEYFRAME_WINDOW_EX(Window* obj, const std::string& name, const float& start, const float& target, const float& onceLength, const bool& loop,
	const float& offset, KeyFrameCallback doingCallback, void* doingUserData, KeyFrameCallback doneCallback, void* doneUserData)
{
	if (name == EMPTY_STRING || txMath::isFloatZero(onceLength))
	{
		ENGINE_ERROR("时间或关键帧不能为空,如果要停止组件,请使用void ALPHA_WINDOW(Window* obj, const float& alpha)");
		return;
	}
	CommandWindowAlpha* cmd = NEW_CMD(cmd);
	cmd->mName = name;
	cmd->mLoop = loop;
	cmd->mOnceLength = onceLength;
	cmd->mTimeOffset = offset;
	cmd->mStartAlpha = start;
	cmd->mTargetAlpha = target;
	cmd->setDoingCallback(doingCallback, doingUserData);
	cmd->setDoneCallback(doneCallback, doneUserData);
	mCommandSystem->pushCommand(cmd, obj);
}
void LayoutTool::ALPHA_WINDOW(Window* obj, const float& start, const float& target, const float& onceLength)
{
	ALPHA_KEYFRAME_WINDOW_EX(obj, ZERO_ONE, start, target, onceLength, false, 0.0f, NULL, NULL, NULL, NULL);
}
void LayoutTool::ALPHA_WINDOW_EX(Window* obj, const float& start, const float& target, const float& onceLength,
	KeyFrameCallback doneCallback, void* doneUserData)
{
	ALPHA_KEYFRAME_WINDOW_EX(obj, ZERO_ONE, start, target, onceLength, false, 0.0f, NULL, NULL, doneCallback, doneUserData);
}
void LayoutTool::ALPHA_WINDOW_EX(Window* obj, const float& start, const float& target, const float& onceLength,
	KeyFrameCallback doingCallback, void* doingUserData, KeyFrameCallback doneCallback, void* doneUserData)
{
	ALPHA_KEYFRAME_WINDOW_EX(obj, ZERO_ONE, start, target, onceLength, false, 0.0f, doingCallback, doingUserData, doneCallback, doneUserData);
}
void LayoutTool::ALPHA_KEYFRAME_WINDOW(Window* obj, const std::string& name, const float& start, const float& target, const float& onceLength)
{
	ALPHA_KEYFRAME_WINDOW_EX(obj, name, start, target, onceLength, false, 0.0f, NULL, NULL, NULL, NULL);
}
void LayoutTool::ALPHA_KEYFRAME_WINDOW(Window* obj, const std::string& name, const float& start, const float& target, const float& onceLength,
	const bool& loop)
{
	ALPHA_KEYFRAME_WINDOW_EX(obj, name, start, target, onceLength, loop, 0.0f, NULL, NULL, NULL, NULL);
}
void LayoutTool::ALPHA_KEYFRAME_WINDOW(Window* obj, const std::string& name, const float& start, const float& target, const float& onceLength,
	const bool& loop, const float& offset)
{
	ALPHA_KEYFRAME_WINDOW_EX(obj, name, start, target, onceLength, loop, offset, NULL, NULL, NULL, NULL);
}
void LayoutTool::ALPHA_KEYFRAME_WINDOW_EX(Window* obj, const std::string& name, const float& start, const float& target, const float& onceLength,
	KeyFrameCallback doneCallback, void* doneUserData)
{
	ALPHA_KEYFRAME_WINDOW_EX(obj, name, start, target, onceLength, false, 0.0f, NULL, NULL, doneCallback, doneUserData);
}
void LayoutTool::ALPHA_KEYFRAME_WINDOW_EX(Window* obj, const std::string& name, const float& start, const float& target, const float& onceLength,
	KeyFrameCallback doingCallback, void* doingUserData, KeyFrameCallback doneCallback, void* doneUserData)
{
	ALPHA_KEYFRAME_WINDOW_EX(obj, name, start, target, onceLength, false, 0.0f, doingCallback, doingUserData, doneCallback, doneUserData);
}
CommandWindowAlpha* LayoutTool::ALPHA_WINDOW_DELAY(LayoutScript* script, Window* obj, const float& delayTime, const float& alpha)
{
	CommandWindowAlpha* cmd = NEW_CMD_DELAY(cmd);
	cmd->mName = EMPTY_STRING;
	cmd->mOnceLength = 0.0f;
	cmd->mStartAlpha = alpha;
	cmd->mTargetAlpha = alpha;
	mCommandSystem->pushDelayCommand(cmd, obj, delayTime);
	script->addDelayCmd(cmd);
	return cmd;
}
CommandWindowAlpha* LayoutTool::ALPHA_KEYFRAME_WINDOW_DELAY_EX(LayoutScript* script, Window* obj, const float& delayTime, const std::string& keyframe, const float& start,
	const float& target, const float& onceLength, const bool& loop, const float& offset, KeyFrameCallback doingCallback, void* doingUserData, KeyFrameCallback doneCallback, void* doneUserData)
{
	if (keyframe == EMPTY_STRING || txMath::isFloatZero(onceLength))
	{
		ENGINE_ERROR("时间或关键帧不能为空,如果要停止组件,请使用CommandWindowAlpha* ALPHA_WINDOW_DELAY(LayoutScript* script, Window* obj, const float& delayTime, const float& alpha)");
		return NULL;
	}
	CommandWindowAlpha* cmd = NEW_CMD_DELAY(cmd);
	cmd->mName = keyframe;
	cmd->mLoop = loop;
	cmd->mOnceLength = onceLength;
	cmd->mTimeOffset = offset;
	cmd->mStartAlpha = start;
	cmd->mTargetAlpha = target;
	cmd->setDoingCallback(doingCallback, doingUserData);
	cmd->setDoneCallback(doneCallback, doneUserData);
	mCommandSystem->pushDelayCommand(cmd, obj, delayTime);
	script->addDelayCmd(cmd);
	return cmd;
}
CommandWindowAlpha* LayoutTool::ALPHA_WINDOW_DELAY(LayoutScript* script, Window* obj, const float& delayTime, const float& start, const float& target,
	const float& onceLength)
{
	return ALPHA_KEYFRAME_WINDOW_DELAY_EX(script, obj, delayTime, ZERO_ONE, start, target, onceLength, false, 0.0f, NULL, NULL, NULL, NULL);
}
CommandWindowAlpha* LayoutTool::ALPHA_WINDOW_DELAY_EX(LayoutScript* script, Window* obj, const float& delayTime, const float& start, const float& target,
	const float& onceLength, KeyFrameCallback doneCallback, void* doneUserData)
{
	return ALPHA_KEYFRAME_WINDOW_DELAY_EX(script, obj, delayTime, ZERO_ONE, start, target, onceLength, false, 0.0f, NULL, NULL, doneCallback, doneUserData);
}
CommandWindowAlpha* LayoutTool::ALPHA_WINDOW_DELAY_EX(LayoutScript* script, Window* obj, const float& delayTime, const float& start, const float& target,
	const float& onceLength, KeyFrameCallback doingCallback, void* doingUserData, KeyFrameCallback doneCallback, void* doneUserData)
{
	return ALPHA_KEYFRAME_WINDOW_DELAY_EX(script, obj, delayTime, ZERO_ONE, start, target, onceLength, false, 0.0f, doingCallback, doingUserData, doneCallback, doneUserData);
}
CommandWindowAlpha* LayoutTool::ALPHA_KEYFRAME_WINDOW_DELAY(LayoutScript* script, Window* obj, const float& delayTime, const std::string& keyframe,
	const float& start, const float& target, const float& onceLength)
{
	return ALPHA_KEYFRAME_WINDOW_DELAY_EX(script, obj, delayTime, keyframe, start, target, onceLength, false, 0.0f, NULL, NULL, NULL, NULL);
}
CommandWindowAlpha* LayoutTool::ALPHA_KEYFRAME_WINDOW_DELAY(LayoutScript* script, Window* obj, const float& delayTime, const std::string& keyframe,
	const float& start, const float& target, const float& onceLength, const bool& loop)
{
	return ALPHA_KEYFRAME_WINDOW_DELAY_EX(script, obj, delayTime, keyframe, start, target, onceLength, loop, 0.0f, NULL, NULL, NULL, NULL);
}
CommandWindowAlpha* LayoutTool::ALPHA_KEYFRAME_WINDOW_DELAY(LayoutScript* script, Window* obj, const float& delayTime, const std::string& keyframe,
	const float& start, const float& target, const float& onceLength, const bool& loop, const float& offset)
{
	return ALPHA_KEYFRAME_WINDOW_DELAY_EX(script, obj, delayTime, keyframe, start, target, onceLength, loop, offset, NULL, NULL, NULL, NULL);
}
CommandWindowAlpha* LayoutTool::ALPHA_KEYFRAME_WINDOW_DELAY_EX(LayoutScript* script, Window* obj, const float& delayTime, const std::string& keyframe,
	const float& start, const float& target, const float& onceLength, KeyFrameCallback doingCallback, void* doingUserData, KeyFrameCallback doneCallback, void* doneUserData)
{
	return ALPHA_KEYFRAME_WINDOW_DELAY_EX(script, obj, delayTime, keyframe, start, target, onceLength, false, 0.0f, doingCallback, doingUserData, doneCallback, doneUserData);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// HSL
void LayoutTool::HSL_WINDOW(Window* obj, const VECTOR3& hsl)
{
	CommandWindowHSL* cmd = NEW_CMD(cmd);
	cmd->mName = EMPTY_STRING;
	cmd->mOnceLength = 0.0f;
	cmd->mStartHSL = hsl;
	cmd->mTargetHSL = hsl;
	mCommandSystem->pushCommand(cmd, obj);
}
void LayoutTool::HSL_WINDOW(Window* obj, const VECTOR3& start, const VECTOR3& target, const float& onceLength)
{
	HSL_KEYFRAME_WINDOW_EX(obj, ZERO_ONE, start, target, onceLength, false, 0.0f, NULL, NULL, NULL, NULL);
}
void LayoutTool::HSL_KEYFRAME_WINDOW(Window* obj, const std::string& keyframe, const VECTOR3& start, const VECTOR3& target, const float& onceLength)
{
	HSL_KEYFRAME_WINDOW_EX(obj, keyframe, start, target, onceLength, false, 0.0f, NULL, NULL, NULL, NULL);
}
void LayoutTool::HSL_KEYFRAME_WINDOW(Window* obj, const std::string& keyframe, const VECTOR3& start, const VECTOR3& target, const float& onceLength, const bool& loop, const float& offset)
{
	HSL_KEYFRAME_WINDOW_EX(obj, keyframe, start, target, onceLength, loop, offset, NULL, NULL, NULL, NULL);
}
void LayoutTool::HSL_KEYFRAME_WINDOW_EX(Window* obj, const std::string& keyframe, const VECTOR3& start, const VECTOR3& target, const float& onceLength, 
	const bool& loop, const float& offset, KeyFrameCallback doneCallback, void* doneUserData)
{
	HSL_KEYFRAME_WINDOW_EX(obj, keyframe, start, target, onceLength, loop, offset, NULL, NULL, doneCallback, doneUserData);
}
void LayoutTool::HSL_KEYFRAME_WINDOW_EX(Window* obj, const std::string& keyframe, const VECTOR3& start, const VECTOR3& target, const float& onceLength, 
	const bool& loop, const float& offset, KeyFrameCallback doingCallback, void* doingUserData, KeyFrameCallback doneCallback, void* doneUserData)
{
	if (keyframe == EMPTY_STRING || txMath::isFloatZero(onceLength))
	{
		ENGINE_ERROR("时间或关键帧不能为空,如果要停止组件,请使用void HSL_WINDOW(Window* obj, const VECTOR3& hsl)");
		return;
	}
	CommandWindowHSL* cmd = NEW_CMD(cmd);
	cmd->mName = keyframe;
	cmd->mLoop = loop;
	cmd->mOnceLength = onceLength;
	cmd->mTimeOffset = offset;
	cmd->mStartHSL = start;
	cmd->mTargetHSL = target;
	cmd->setDoingCallback(doingCallback, doingUserData);
	cmd->setDoneCallback(doneCallback, doneUserData);
	mCommandSystem->pushCommand(cmd, obj);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------
// 音效
void LayoutTool::STOP_AUDIO(Window* obj)
{
	CommandWindowStopAudio* cmd = NEW_CMD(cmd);
	mCommandSystem->pushCommand(cmd, obj);
}
// name为sound文件夹的相对路径,
void LayoutTool::PLAY_AUDIO(Window* obj, const std::string& name, const bool& loop, const float& volume)
{
	CommandWindowPlayAudio* cmd = NEW_CMD(cmd);
	cmd->mSoundName = name;
	cmd->mLoop = loop;
	cmd->mVolume = volume;
	mCommandSystem->pushCommand(cmd, obj);
}
CommandWindowPlayAudio* LayoutTool::PLAY_AUDIO_DELAY(LayoutScript* script, Window* obj, const float& delayTime, const std::string& name, const bool& loop, const float& volume)
{
	CommandWindowPlayAudio* cmd = NEW_CMD_DELAY(cmd);
	cmd->mSoundName = name;
	cmd->mLoop = loop;
	cmd->mVolume = volume;
	mCommandSystem->pushDelayCommand(cmd, obj, delayTime);
	script->addDelayCmd(cmd);
	return cmd;
}