#ifndef _LAYOUT_TOOL_H_
#define _LAYOUT_TOOL_H_

#include "txEngineDefine.h"
#include "txPoint.h"
#include "txCommandHeader.h"

class LayoutScript;
class Window;
class SceneProcedure;
class LayoutTool : public txEngineBase
{
public:
	// 布局
	//------------------------------------------------------------------------------------------------------------------------------------
	static void LOAD_LAYOUT(const int& type, const int& renderOrder, const bool& visible, const bool& immediately, const std::string& param);
	static void LOAD_LAYOUT_ASYNC(const int& type, const int& renderOrder, LayoutAsyncDone callback, void* userData);
	static void UNLOAD_LAYOUT(const int& type);
	static void UNLOAD_LAYOUT_DELAY(const int& type, const float& delayTime);
	static void LOAD_LAYOUT_HIDE(const int& type, const int& renderOrder);
	static void LOAD_LAYOUT_SHOW(const int& type, const int& renderOrder);
	static void LOAD_LAYOUT_SHOW(const int& type, const int& renderOrder, const bool& immediately, const std::string& param = EMPTY_STRING);
	static void HIDE_LAYOUT(const int& type, const bool& immediately = false, const std::string& param = EMPTY_STRING);
	static void HIDE_LAYOUT_FORCE(const int& type);
	static void SHOW_LAYOUT(const int& type, const bool& immediately = false, const std::string& param = EMPTY_STRING);
	static void SHOW_LAYOUT_FORCE(const int& type);
	static void VISIBLE_LAYOUT(const int& type, const bool& visible, const bool& immediately = false, const std::string& param = EMPTY_STRING);
	static void VISIBLE_LAYOUT_FORCE(const int& type, const bool& visible);
	static CommandLayoutManagerLayoutVisible* HIDE_LAYOUT_DELAY(SceneProcedure* procedure, float delayTime, const int& type, const bool& immediately = false, const std::string& param = EMPTY_STRING);
	static CommandLayoutManagerLayoutVisible* HIDE_LAYOUT_DELAY_FORCE(float delayTime, const int& type);
	static CommandLayoutManagerLayoutVisible* SHOW_LAYOUT_DELAY(SceneProcedure* procedure, float delayTime, const int& type, const bool& immediately = false, const std::string& param = EMPTY_STRING);
	static CommandLayoutManagerLayoutVisible* SHOW_LAYOUT_DELAY_EX(SceneProcedure* procedure, float delayTime, const int& type, CommandCallback start, void* userData, const bool& immediately = false, const std::string& param = EMPTY_STRING);
	static CommandLayoutManagerLayoutVisible* SHOW_LAYOUT_DELAY_FORCE(float delayTime, const int& type);
	static CommandLayoutManagerLayoutVisible* VISIBLE_LAYOUT_DELAY(SceneProcedure* procedure, float delayTime, const int& type, const bool& visible, const bool& immediately = false, const std::string& param = EMPTY_STRING);
	static CommandLayoutManagerLayoutVisible* VISIBLE_LAYOUT_DELAY_EX(SceneProcedure* procedure, float delayTime, const int& type, const bool& visible, CommandCallback start, void* userData, const bool& immediately = false, const std::string& param = EMPTY_STRING);
	static CommandLayoutManagerLayoutVisible* VISIBLE_LAYOUT_DELAY_FORCE(float delayTime, const int& type, const bool& visible);
	//--------------------------------------------------------------------------------------------------------------------------------------------
	// 旋转
	static void ROTATE_WINDOW(Window* obj, const VECTOR3& rotation);
	static void ROTATE_KEYFRMAE_WINDOW_EX(Window* obj, const std::string& keyframe, const VECTOR3& start, const VECTOR3& target, const float& onceLength, const bool& loop, const float& offset, KeyFrameCallback doingCallback, void* doingUserData, KeyFrameCallback doneCallback, void* doneUserData);
	static CommandWindowRotate* ROTATE_WINDOW_DELAY(LayoutScript* script, Window* obj, const float& delayTime, const VECTOR3& rotation);
	static void ROTATE_WINDOW(Window* obj, const VECTOR3& start, const VECTOR3& target, const float& time);
	static void ROTATE_WINDOW_EX(Window* obj, const VECTOR3& start, const VECTOR3& target, const float& time, KeyFrameCallback doingCallback, void* doingUserData, KeyFrameCallback doneCallback, void* doneUserData);
	static void ROTATE_WINDOW_EX(Window* obj, const VECTOR3& start, const VECTOR3& target, const float& time, KeyFrameCallback doneCallback, void* doneUserData);
	static void ROTATE_KEYFRMAE_WINDOW(Window* obj, const std::string& keyframe, const VECTOR3& start, const VECTOR3& target, const float& onceLength);
	static void ROTATE_KEYFRMAE_WINDOW(Window* obj, const std::string& keyframe, const VECTOR3& start, const VECTOR3& target, const float& onceLength, const bool& loop, const float& offset);
	static CommandWindowRotate* ROTATE_WINDOW_DELAY(LayoutScript* script, Window* obj, const float& delayTime, const VECTOR3& start, const VECTOR3& target, const float& time);
	static CommandWindowRotate* ROTATE_KEYFRMAE_WINDOW_DELAY(LayoutScript* script, Window* obj, const float& delayTime, const std::string& keyframe, const VECTOR3& start, const VECTOR3& target, const float& onceLength);
	static CommandWindowRotate* ROTATE_KEYFRMAE_WINDOW_DELAY(LayoutScript* script, Window* obj, const float& delayTime, const std::string& keyframe, const VECTOR3& start, const VECTOR3& target, const float& onceLength, const bool& loop);
	static CommandWindowRotate* ROTATE_KEYFRMAE_WINDOW_DELAY(LayoutScript* script, Window* obj, const float& delayTime, const std::string& keyframe, const VECTOR3& start, const VECTOR3& target, const float& onceLength, const bool& loop, const float& offset);
	static void ROTATE_SPEED_WINDOW(Window* obj);
	static void ROTATE_SPEED_WINDOW(Window* obj, const VECTOR3& speed);
	static void ROTATE_SPEED_WINDOW(Window* obj, const VECTOR3& speed, const VECTOR3& startAngle);
	static void ROTATE_SPEED_WINDOW(Window* obj, const VECTOR3& speed, const VECTOR3& startAngle, const VECTOR3& rotateAccelerationValue);
	static CommandWindowRotateSpeed* ROTATE_SPEED_WINDOW_DELAY(LayoutScript* script, Window* obj, const float& delayTime, const VECTOR3& speed);
	static CommandWindowRotateSpeed* ROTATE_SPEED_WINDOW_DELAY(LayoutScript* script, Window* obj, const float& delayTime, const VECTOR3& speed, const VECTOR3& startAngle);
	static CommandWindowRotateSpeed* ROTATE_SPEED_WINDOW_DELAY(LayoutScript* script, Window* obj, const float& delayTime, const VECTOR3& speed, const VECTOR3& startAngle, const VECTOR3& rotateAccelerationValue);
	//-------------------------------------------------------------------------------------------------------------------------------------------------------
	// 移动
	static void MOVE_WINDOW(Window* obj, const txPoint& pos);
	static void MOVE_KEYFRAME_WINDOW_EX(Window* obj, const std::string& fileName, const txPoint& startPos, const txPoint& targetPos, const float& onceLength, const bool& loop, const float& offset, KeyFrameCallback doingCallback, void* doingUserData, KeyFrameCallback doneCallBack, void* doneUserData);
	static void MOVE_WINDOW(Window* obj, const txPoint& start, const txPoint& target, const float& onceLength);
	static void MOVE_WINDOW_EX(Window* obj, const txPoint& start, const txPoint& target, const float& onceLength, KeyFrameCallback doneCallback, void* doneUserData);
	static void MOVE_WINDOW_EX(Window* obj, const txPoint& start, const txPoint& target, const float& onceLength, KeyFrameCallback doingCallback, void* doingUserData, KeyFrameCallback doneCallback, void* doneUserData);
	static void MOVE_WINDOW_EX(Window* obj, const txPoint& start, const txPoint& target, const float& onceLength, const float& offsetTime, KeyFrameCallback doneCallback, void* doneUserData);
	static void MOVE_WINDOW_EX(Window* obj, const txPoint& start, const txPoint& target, const float& onceLength, const float& offsetTime, KeyFrameCallback doingCallback, void* doingUserData, KeyFrameCallback doneCallback, void* doneUserData);
	static void MOVE_KEYFRAME_WINDOW(Window* obj, const std::string& fileName, const txPoint& startPos, const txPoint& targetPos, const float& onceLength);
	static void MOVE_KEYFRAME_WINDOW(Window* obj, const std::string& fileName, const txPoint& startPos, const txPoint& targetPos, const float& onceLength, const bool& loop);
	static void MOVE_KEYFRAME_WINDOW(Window* obj, const std::string& fileName, const txPoint& startPos, const txPoint& targetPos, const float& onceLength, const bool& loop, const float& offset);
	static void MOVE_KEYFRAME_WINDOW_EX(Window* obj, const std::string& fileName, const txPoint& startPos, const txPoint& targetPos, const float& onceLength, KeyFrameCallback TremblingCallBack, void* doingUserData, KeyFrameCallback doneCallBack, void* doneUserData);
	static void MOVE_KEYFRAME_WINDOW_EX(Window* obj, const std::string& fileName, const txPoint& startPos, const txPoint& targetPos, const float& onceLength, const bool& loop, KeyFrameCallback doingCallback, void* doingUserData, KeyFrameCallback doneCallBack, void* doneUserData);
	static CommandWindowMove* MOVE_KEYFRAME_WINDOW_DELAY_EX(LayoutScript* script, Window* obj, const float& delayTime, const std::string& keyframe, const txPoint& startPos, const txPoint& targetPos, const float& onceLength, const bool& loop, const float& offset, KeyFrameCallback doingCallback, void* doingUserData, KeyFrameCallback doneCallback, void* doneUserData);
	static CommandWindowMove* MOVE_WINDOW_DELAY(LayoutScript* script, Window* obj, const float& delayTime, const txPoint& pos);
	static CommandWindowMove* MOVE_WINDOW_DELAY(LayoutScript* script, Window* obj, const float& delayTime, const txPoint& start, const txPoint& target, const float& onceLength);
	static CommandWindowMove* MOVE_WINDOW_DELAY_EX(LayoutScript* script, Window* obj, const float& delayTime, const txPoint& start, const txPoint& target, const float& onceLength, KeyFrameCallback moveDoneCallback, void* doneUserData);
	static CommandWindowMove* MOVE_WINDOW_DELAY_EX(LayoutScript* script, Window* obj, const float& delayTime, const txPoint& start, const txPoint& target, const float& onceLength, KeyFrameCallback doingCallback, void* doingUserData, KeyFrameCallback moveDoneCallback, void* doneUserData);
	static CommandWindowMove* MOVE_KEYFRAME_WINDOW_DELAY(LayoutScript* script, Window* obj, const float& delayTime, const std::string& keyframe, const txPoint& startPos, const txPoint& targetPos, const float& onceLength);
	static CommandWindowMove* MOVE_KEYFRAME_WINDOW_DELAY(LayoutScript* script, Window* obj, const float& delayTime, const std::string& keyframe, const txPoint& startPos, const txPoint& targetPos, const float& onceLength, const bool& loop);
	static CommandWindowMove* MOVE_KEYFRAME_WINDOW_DELAY(LayoutScript* script, Window* obj, const float& delayTime, const std::string& keyframe, const txPoint& startPos, const txPoint& targetPos, const float& onceLength, const bool& loop, const float& offset);
	//----------------------------------------------------------------------------------------------------------------------------------------------------------
	// 显示
	static void ACTIVE_WINDOW(Window* obj, const bool& active = true);
	static CommandWindowActive* ACTIVE_WINDOW_DELAY(LayoutScript* script, Window* obj, const bool& active, const float& delayTime);
	static CommandWindowActive* ACTIVE_WINDOW_DELAY_EX(LayoutScript* script, Window* obj, const bool& active, const float& dealyTime, CommandCallback startCallback, void* userData);
	//----------------------------------------------------------------------------------------------------------------------------------------------------------
	// 缩放
	static void SCALE_WINDOW(Window* obj, const VECTOR2& scale);
	static void SCALE_KEYFRAME_WINDOW_EX(Window* obj, const std::string& fileName, const VECTOR2& start, const VECTOR2& target, const float& onceLength, const bool& loop, const float& offset, const bool& fullOnce, KeyFrameCallback doingCallback, void* doingUserData, KeyFrameCallback doneCallback, void* doneUserData);
	static void SCALE_WINDOW(Window* obj, const VECTOR2& start, const VECTOR2& target, const float& onceLength);
	static void SCALE_WINDOW_EX(Window* obj, const VECTOR2& start, const VECTOR2& target, const float& onceLength, KeyFrameCallback doneCallback, void* doneUserData);
	static void SCALE_WINDOW_EX(Window* obj, const VECTOR2& start, const VECTOR2& target, const float& onceLength, KeyFrameCallback doingCallback, void* doingUserData, KeyFrameCallback doneCallback, void* doneUserData);
	static void SCALE_KEYFRAME_WINDOW(Window* obj, const std::string& fileName, const VECTOR2& start, const VECTOR2& target, const float& onceLength);
	static void SCALE_KEYFRAME_WINDOW(Window* obj, const std::string& fileName, const VECTOR2& start, const VECTOR2& target, const float& onceLength, const bool& loop);
	static void SCALE_KEYFRAME_WINDOW(Window* obj, const std::string& fileName, const VECTOR2& start, const VECTOR2& target, const float& onceLength, const bool& loop, const float& offset);
	static void SCALE_KEYFRAME_WINDOW_EX(Window* obj, const std::string& fileName, const VECTOR2& start, const VECTOR2& target, const float& onceLength, KeyFrameCallback doneCallback, void* doneUserData);
	static void SCALE_KEYFRAME_WINDOW_EX(Window* obj, const std::string& fileName, const VECTOR2& start, const VECTOR2& target, const float& onceLength, const bool& loop, KeyFrameCallback doneCallback, void* doneUserData);
	static void SCALE_KEYFRAME_WINDOW_EX(Window* obj, const std::string& fileName, const VECTOR2& start, const VECTOR2& target, const float& onceLength, const bool& loop, const float& offset, KeyFrameCallback doneCallback, void* doneUserData);
	static void SCALE_KEYFRAME_WINDOW_EX(Window* obj, const std::string& fileName, const VECTOR2& start, const VECTOR2& target, const float& onceLength, KeyFrameCallback doingCallback, void* doingUserData, KeyFrameCallback doneCallback, void* doneUserData);
	static void SCALE_KEYFRAME_WINDOW_EX(Window* obj, const std::string& fileName, const VECTOR2& start, const VECTOR2& target, const float& onceLength, const bool& loop, KeyFrameCallback doingCallback, void* doingUserData, KeyFrameCallback doneCallback, void* doneUserData);
	static CommandWindowScale* SCALE_WINDOW_DELAY(LayoutScript* script, Window* obj, const float& delayTime, const VECTOR2& scale);
	static CommandWindowScale* SCALE_KEYFRAME_WINDOW_DELAY_EX(LayoutScript* script, Window* obj, const float& delayTime, const std::string& keyframe, const VECTOR2& start, const VECTOR2& target, const float& onceLength, const bool& loop, const float& offset, const bool& fullOnce, KeyFrameCallback doingCallback, void* doingUserData, KeyFrameCallback doneCallback, void* doneUserData);
	static CommandWindowScale* SCALE_WINDOW_DELAY(LayoutScript* script, Window* obj, const float& delayTime, const VECTOR2& start, const VECTOR2& target, const float& onceLength);
	static CommandWindowScale* SCALE_WINDOW_DELAY_EX(LayoutScript* script, Window* obj, const float& delayTime, const VECTOR2& start, const VECTOR2& target, const float& onceLength, KeyFrameCallback doingCallback, void* doingUserData, KeyFrameCallback doneCallback, void* doneUserData);
	static CommandWindowScale* SCALE_KEYFRAME_WINDOW_DELAY(LayoutScript* script, Window* obj, const float& delayTime, const std::string& keyframe, const VECTOR2& start, const VECTOR2& target, const float& onceLength);
	static CommandWindowScale* SCALE_KEYFRAME_WINDOW_DELAY(LayoutScript* script, Window* obj, const float& delayTime, const std::string& keyframe, const VECTOR2& start, const VECTOR2& target, const float& onceLength, const bool& loop);
	static CommandWindowScale* SCALE_KEYFRAME_WINDOW_DELAY(LayoutScript* script, Window* obj, const float& delayTime, const std::string& keyframe, const VECTOR2& start, const VECTOR2& target, const float& onceLength, const bool& loop, const float& offset);
	static CommandWindowScale* SCALE_KEYFRAME_WINDOW_DELAY_EX(LayoutScript* script, Window* obj, const float& delayTime, const std::string& keyframe, const VECTOR2& start, const VECTOR2& target, const float& onceLength, KeyFrameCallback doingCallback, void* doingUserData, KeyFrameCallback doneCallback, void* doneUserData);
	static CommandWindowScale* SCALE_KEYFRAME_WINDOW_DELAY_EX(LayoutScript* script, Window* obj, const float& delayTime, const std::string& keyframe, const VECTOR2& start, const VECTOR2& target, const float& onceLength, const bool& loop, KeyFrameCallback doingCallback, void* doingUserData, KeyFrameCallback doneCallback, void* doneUserData);
	//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// 透明度
	static void ALPHA_WINDOW(Window* obj, const float& alpha);
	static void ALPHA_KEYFRAME_WINDOW_EX(Window* obj, const std::string& name, const float& start, const float& target, const float& onceLength, const bool& loop, const float& offset, KeyFrameCallback doingCallback, void* doingUserData, KeyFrameCallback doneCallback, void* doneUserData);
	static void ALPHA_WINDOW(Window* obj, const float& start, const float& target, const float& onceLength);
	static void ALPHA_WINDOW_EX(Window* obj, const float& start, const float& target, const float& onceLength, KeyFrameCallback doneCallback, void* doneUserData);
	static void ALPHA_WINDOW_EX(Window* obj, const float& start, const float& target, const float& onceLength, KeyFrameCallback doingCallback, void* doingUserData, KeyFrameCallback doneCallback, void* doneUserData);
	static void ALPHA_KEYFRAME_WINDOW(Window* obj, const std::string& name, const float& start, const float& target, const float& onceLength);
	static void ALPHA_KEYFRAME_WINDOW(Window* obj, const std::string& name, const float& start, const float& target, const float& onceLength, const bool& loop);
	static void ALPHA_KEYFRAME_WINDOW(Window* obj, const std::string& name, const float& start, const float& target, const float& onceLength, const bool& loop, const float& offset);
	static void ALPHA_KEYFRAME_WINDOW_EX(Window* obj, const std::string& name, const float& start, const float& target, const float& onceLength, KeyFrameCallback doneCallback, void* doneUserData);
	static void ALPHA_KEYFRAME_WINDOW_EX(Window* obj, const std::string& name, const float& start, const float& target, const float& onceLength, KeyFrameCallback doingCallback, void* doingUserData, KeyFrameCallback doneCallback, void* doneUserData);
	static CommandWindowAlpha* ALPHA_WINDOW_DELAY(LayoutScript* script, Window* obj, const float& delayTime, const float& alpha);
	static CommandWindowAlpha* ALPHA_KEYFRAME_WINDOW_DELAY_EX(LayoutScript* script, Window* obj, const float& delayTime, const std::string& keyframe, const float& start, const float& target, const float& onceLength, const bool& loop, const float& offset, KeyFrameCallback doingCallback, void* doingUserData, KeyFrameCallback doneCallback, void* doneUserData);
	static CommandWindowAlpha* ALPHA_WINDOW_DELAY(LayoutScript* script, Window* obj, const float& delayTime, const float& start, const float& target, const float& onceLength);
	static CommandWindowAlpha* ALPHA_WINDOW_DELAY_EX(LayoutScript* script, Window* obj, const float& delayTime, const float& start, const float& target, const float& onceLength, KeyFrameCallback doneCallback, void* doneUserData);
	static CommandWindowAlpha* ALPHA_WINDOW_DELAY_EX(LayoutScript* script, Window* obj, const float& delayTime, const float& start, const float& target, const float& onceLength, KeyFrameCallback doingCallback, void* doingUserData, KeyFrameCallback doneCallback, void* doneUserData);
	static CommandWindowAlpha* ALPHA_KEYFRAME_WINDOW_DELAY(LayoutScript* script, Window* obj, const float& delayTime, const std::string& keyframe, const float& start, const float& target, const float& onceLength);
	static CommandWindowAlpha* ALPHA_KEYFRAME_WINDOW_DELAY(LayoutScript* script, Window* obj, const float& delayTime, const std::string& keyframe, const float& start, const float& target, const float& onceLength, const bool& loop);
	static CommandWindowAlpha* ALPHA_KEYFRAME_WINDOW_DELAY(LayoutScript* script, Window* obj, const float& delayTime, const std::string& keyframe, const float& start, const float& target, const float& onceLength, const bool& loop, const float& offset);
	static CommandWindowAlpha* ALPHA_KEYFRAME_WINDOW_DELAY_EX(LayoutScript* script, Window* obj, const float& delayTime, const std::string& keyframe, const float& start, const float& target, const float& onceLength, KeyFrameCallback doingCallback, void* doingUserData, KeyFrameCallback doneCallback, void* doneUserData);
	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// HSL
	static void HSL_WINDOW(Window* obj, const VECTOR3& hsl);
	static void HSL_WINDOW(Window* obj, const VECTOR3& start, const VECTOR3& target, const float& onceLength);
	static void HSL_KEYFRAME_WINDOW(Window* obj, const std::string& keyframe, const VECTOR3& start, const VECTOR3& target, const float& onceLength);
	static void HSL_KEYFRAME_WINDOW(Window* obj, const std::string& keyframe, const VECTOR3& start, const VECTOR3& target, const float& onceLength, const bool& loop, const float& offset);
	static void HSL_KEYFRAME_WINDOW_EX(Window* obj, const std::string& keyframe, const VECTOR3& start, const VECTOR3& target, const float& onceLength, const bool& loop, const float& offset, KeyFrameCallback doneCallback, void* doneUserData);
	static void HSL_KEYFRAME_WINDOW_EX(Window* obj, const std::string& keyframe, const VECTOR3& start, const VECTOR3& target, const float& onceLength, const bool& loop, const float& offset, KeyFrameCallback doingCallback, void* doingUserData, KeyFrameCallback doneCallback, void* doneUserData);
	//--------------------------------------------------------------------------------------------------------------------------------------------------------
	// 音效
	static void STOP_AUDIO(Window* obj);
	// name为sound文件夹的相对路径,
	static void PLAY_AUDIO(Window* obj, const std::string& name, const bool& loop, const float& volume);
	static CommandWindowPlayAudio* PLAY_AUDIO_DELAY(LayoutScript* script, Window* obj, const float& delayTime, const std::string& name, const bool& loop, const float& volume);
};

#endif