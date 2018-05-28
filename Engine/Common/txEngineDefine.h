#ifndef _TX_ENGINE_DEFINE_H_
#define _TX_ENGINE_DEFINE_H_

// 平台标识宏
#define PLATFORM_WINDOWS 0
#define PLATFORM_ANDROID 1

// 正在运行的平台标识
#ifdef _WINDOWS
#define RUN_PLATFORM PLATFORM_WINDOWS
#else
#define  RUN_PLATFORM PLATFORM_ANDROID
#endif

// 只有在安卓平台下才可能从AssetManager中加载资源
#if RUN_PLATFORM == PLATFORM_ANDROID
// 从AssetManager加载资源
#define LOAD_FROM_ASSETMANAGER
#endif

// 是否使用安全版本的API,暂时只有windows平台才使用安全版本的函数
#ifdef _WINDOWS
#define _USE_SAFE_API
#endif

#ifdef _USE_SAFE_API
#define SPRINTF(buffer, bufferSize, ...) sprintf_s(buffer, bufferSize, __VA_ARGS__)
#else
#define SPRINTF(buffer, bufferSize, ...) sprintf(buffer, __VA_ARGS__)
#endif

#if RUN_PLATFORM == PLATFORM_ANDROID
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <unistd.h>
#include <dirent.h>
#elif RUN_PLATFORM == PLATFORM_WINDOWS
#include <glew.h>
#include <wglew.h>
#include <windows.h>
#include <winsock.h>
#include <iostream>
#include <io.h>
#include <direct.h>
#endif
#include <string>
#include <map>
#include <vector>
#include <set>
#include <list>
#include <stdlib.h>
#include <sys/stat.h>
#include <typeinfo>
#include <memory>
#include <time.h>
#include <cmath>
#include <math.h>
#include <sys/types.h>
#include <mmsystem.h>
#include <iphlpapi.h>
#include <comutil.h>
#include <Wbemidl.h>
#include <tchar.h>
#include <strsafe.h>
#include <algorithm>
#include <atlconv.h>
#include <ntddndis.h>
#include <winioctl.h>
#include <cctype>
#include <atomic>
#include <DbgHelp.h>

#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/vec3.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/intersect.hpp"

#include "txVector.h"
#include "txMap.h"
#include "txSet.h"
#include "EngineEnum.h"
#include "EngineCallback.h"

// 对常用数据类型的重命名,方便移植
// 基础数据类型暂时不重命名
//typedef void VOID;
//typedef bool BOOL;
//typedef char CHAR;
//typedef unsigned char UCHAR;
//typedef short SHORT;
//typedef unsigned short USHORT;
//typedef int INT;
//typedef unsigned int UINT;
//typedef long LONG;
//typedef unsigned long ULONG;
//typedef float FLOAT;
//typedef double DOUBLE;
typedef glm::vec2 VECTOR2;
typedef glm::vec3 VECTOR3;
typedef glm::vec4 VECTOR4;
typedef glm::fquat QUATERNION;
typedef glm::mat3 MATRIX3;
typedef glm::mat4 MATRIX4;

const std::string MEDIA = "media";
const std::string CONFIG = "Config";
const std::string FONT = "Font";
const std::string GAME_TABLE = "GameTable";
const std::string UI = "UI";
const std::string LAYOUT = "Layout";
const std::string TEXTURE = "Texture";
const std::string WINDOW_PREFAB = "WindowPrefab";
const std::string WINDOW_TEMPLATE = "WindowTemplate";
const std::string NUMBER = "Number";
const std::string TEXTURE_ANIM = "TextureAnim";
const std::string TREMBLING = "Trembling";
const std::string PARTICLE = "Particle";
const std::string MODEL = "Model";
const std::string DYNAMIC = "Dynamic";
const std::string STATIC = "Static";
const std::string SHADER = "Shader";
const std::string SNAP_SCREEN = "SnapScreen";
const std::string SCENE = "Scene";
const std::string VIDEO = "Video";

// P_表示相对于项目的路径
const std::string P_MEDIA_PATH = "../" + MEDIA + "/";
const std::string P_FONT_PATH = P_MEDIA_PATH + FONT + "/";
const std::string P_GAME_TABLE_PATH = P_MEDIA_PATH + GAME_TABLE + "/";
const std::string P_UI_PATH = P_MEDIA_PATH + UI + "/";
const std::string P_UI_LAYOUT_PATH = P_UI_PATH + LAYOUT + "/";
const std::string P_UI_TEXTURE_PATH = P_UI_PATH + TEXTURE + "/";
const std::string P_UI_WINDOW_PREFAB_PATH = P_UI_PATH + WINDOW_PREFAB + "/";
const std::string P_UI_WINDOW_TEMPLATE_PATH = P_UI_PATH + WINDOW_TEMPLATE + "/";
const std::string P_UI_NUMBER_PATH = P_UI_TEXTURE_PATH + NUMBER + "/";
const std::string P_UI_TEXTURE_ANIM_PATH = P_UI_TEXTURE_PATH + TEXTURE_ANIM + "/";
const std::string P_TREMBLING_PATH = P_MEDIA_PATH + TREMBLING + "/";
const std::string P_PARTICLE_PATH = P_MEDIA_PATH + PARTICLE + "/";
const std::string P_MODEL_PATH = P_MEDIA_PATH + MODEL + "/";
const std::string P_MODEL_DYNAMIC_PATH = P_MODEL_PATH + DYNAMIC + "/";
const std::string P_MODEL_STATIC_PATH = P_MODEL_PATH + STATIC + "/";
const std::string P_MODEL_TEXTURE_PATH = P_MODEL_PATH + TEXTURE + "/";
const std::string P_SNAP_SCREEN_PATH = P_MEDIA_PATH + SNAP_SCREEN + "/";
const std::string P_SHADER_PATH = P_MEDIA_PATH + SHADER + "/";
const std::string P_SCENE_PATH = P_MEDIA_PATH + SCENE + "/";
const std::string P_VIDEO_PATH = P_MEDIA_PATH + VIDEO + "/";

const std::string EMPTY_STRING = "";
const std::string TREMBLING_SUFFIX = ".KeyFrame";
const std::string PREFAB_SUFFIX = ".WindowPrefab";
const std::string TEMPLATE_SUFFIX = ".WindowTemplate";
const std::string TYPE_PROPERTY_NAME = "Type";
const std::string NAME_PROPERTY_NAME = "Name";
const std::string PARENT_PROPERTY_NAME = "Parent";
const std::string PREFAB_TYPE_NAME = "WindowPrefab";

// 关键字名定义
const std::string ONE_ZERO = "OneZero";
const std::string ONE_ZERO_ONE = "OneZeroOne";
const std::string ONE_ZERO_ONE_CURVE = "OneZeroOne_Curve";
const std::string QUADRATIC_CURVE = "Quadratic_Curve";
const std::string SIN_CURVE = "Sin_Curve";
const std::string ZERO_ONE = "ZeroOne";
const std::string ZERO_ONE_ZERO = "ZeroOneZero";
const std::string ZERO_ONE_DELAY = "ZeroOne_Delay";

const int UI_VERTEX_COUNT = 4;
const int UI_TEXTURE_COORD_COUNT = 4;
const int UI_INDEX_COUNT = 6;
const int WIREFRAME_INDEX_COUNT = 8;
const int COORDINATE_INDEX_COUNT = 4;
const float MIN_DELETE_DELAY_SIZE = 0.06f;	// 当纹理大小超过多少MB时就会延迟释放
const int MAX_DELETE_COUNT = 3;				// 在延迟释放纹理的函数中一次释放的最大数量
const int RENDER_BONE_MAXNUM = 100;			//最大骨骼数目
const int FRAMES_PER_SECOND = 30;			// 每秒动画帧数
const short BLOCK_GRID_COUNT = 5000;		// 5000个格子分为一个模型
const float ANIM_SWITCH_TIME = 0.2f;		// 动作切换时间
const int MAX_CHANNEL_COUNT = 32;			// 音效的最大通道数量

struct KEY_STATE
{
	KEY_PUSH_STATE mState;	// 按键的状态
	float mStateTime;		// 放开或者按下的持续时间
	KEY_STATE()
	{
		mState = KPS_KEEP_UP;
		mStateTime = 0.0f;
	}
	KEY_STATE(const KEY_PUSH_STATE& state)
	{
		mState = state;
		mStateTime = 0.0f;
	}
	KEY_STATE(const KEY_PUSH_STATE& state, const float& time)
	{
		mState = state;
		mStateTime = time;
	}
};

struct AnimInfo
{
	VECTOR3 pos;
	QUATERNION rot;
	VECTOR3 scale;
};

struct AnimKeyFrameCallbackInfo
{
	void* mUserData;				// 用户自定义数据
	int mAditionalParam;			// 用户自定义数据的附加参数
	float mKeyFrame;				// 回调的关键帧,当动作执行到这一帧时调用回调函数
	AnimKeyFrameCallback mCallback; // 回调的函数
	bool mCallbackOnce;				// 是否只回调一次
	bool mOnlyForCurAnim;			// 是否只能由当前动作触发
	bool mAlreadyCallback;			// 是否已经调用过了回调函数
};

struct SCREEN_INFO
{
	float density; // 屏幕密度（像素比例：0.75/1.0/1.5/2.0）
	int densityDPI; // 屏幕密度（每寸像素：120/160/240/320）
	float xDPI;
	float yDPI;
	int screenWidth; // 屏幕宽（像素，如：480px）
	int screenHeight; // 屏幕高（像素，如：800px）
};

#ifndef INVALID_SOCKET
#define INVALID_SOCKET ~0
#endif

#ifndef NULL
#define NULL 0
#endif

#if RUN_PLATFORM == PLATFORM_WINDOWS
#define TX_THREAD HANDLE
#define TX_SOCKET SOCKET
#define THREAD_CALLBACK_DECLEAR(func) static DWORD WINAPI func(LPVOID args)
#define THREAD_CALLBACK(class, func) DWORD WINAPI class##::##func(LPVOID args)
#define CREATE_THREAD(thread, func, args) thread = CreateThread(NULL, 0, func, args, 0, NULL)
#define CLOSE_THREAD(thread)	\
if (thread != NULL)				\
{								\
	TerminateThread(thread, 0);	\
	CloseHandle(thread);		\
	thread = NULL;				\
}
#elif RUN_PLATFORM == PLATFORM_LINUX
#define TX_THREAD pthread_t
#define TX_SOCKET unsigned int
#define SOCKADDR_IN sockaddr_in
#define THREAD_CALLBACK_DECLEAR(func) static void* func(void* args)
#define THREAD_CALLBACK(class, func) void* class##::##func(void* args)
#define CREATE_THREAD(thread, func, args) pthread_create(&thread, NULL, func, args);
#define CLOSE_THREAD(thread)	\
if (thread != NULL)				\
{								\
	pthread_cancel(thread);		\
	thread = NULL;				\
}
#ifdef __GNUC__
#define CSET_GBK    "GBK"
#define CSET_UTF8   "UTF-8"
#define LC_NAME_zh_CN   "zh_CN"
#endif
#define LC_NAME_zh_CN_GBK       LC_NAME_zh_CN "." CSET_GBK
#define LC_NAME_zh_CN_UTF8      LC_NAME_zh_CN "." CSET_UTF8
#define LC_NAME_zh_CN_DEFAULT   LC_NAME_zh_CN_GBK
#endif
// 可以将t的名字转化为字符串
#define TOSTRING(t) #t
#define LOCK(l) l.waitForUnlock(__FILE__, __LINE__)
#define UNLOCK(l) l.unlock()
// 设置value的指定位置pos的字节的值为byte,并且不影响其他字节
#define SET_BYTE(value, byte, pos) value = (value & ~(0x000000ff << (8 * pos))) | (byte << (8 * pos))
// 获得value的指定位置pos的字节的值
#define GET_BYTE(value, pos) (value & (0x000000ff << (8 * pos))) >> (8 * pos)
// 再次封装后的容器的遍历宏
#define FOR_STL(stl, expression) stl.lock(__FILE__, __LINE__); for (expression)
#define END_FOR_STL(stl) stl.unlock();

#define DEFINE_BASE_PROPERTY(C)						\
template<typename T, typename P>					\
class C##Property : public txRealProperty<T, P, C>	\
{													\
public:												\
	C##Property()									\
	{												\
		mReceiverType = TOSTRING(C);				\
	}												\
};

// 给指定窗口类型定义窗口属性基类
#define WINDOW_PROPERTY(W)							\
template<typename P, typename T>					\
class W##Property : public txRealProperty<P, T, W>	\
{													\
public:												\
	W##Property()									\
	{												\
		mReceiverType = TOSTRING(W);				\
	}												\
};

// 创建一个行为
#define CREATE_ACTION(T, action, tree, name, parent)\
	T* action = (T*)(tree->addAction(TOSTRING(T), name, parent));

// 创建一个窗口行为
#define CREATE_WINDOW_ACTION(T, action, tree, name, parent, window)\
	T* action = (T*)(tree->addAction(TOSTRING(T), name, parent)); \
	action->setWindow(window);

#endif