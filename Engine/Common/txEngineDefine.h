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

// 地形类型定义
enum TERRAIN_TYPE
{
	TT_GROUND,		// 地面
	TT_WATER,		// 水面
};

// 碰撞体类型定义
enum GEOM_TYPE
{
	GT_NONE,
	GT_BOX,
	GT_PLANE,
	GT_SPHERE,
	GT_CAPSULE,
	GT_CYLINDER,
};

enum ENGINE_EVENT
{
	EE_LOAD_SCENE_OBJECT,	// 加载场景文件中的物体,参数为物体名
	EE_START_LOAD_SCENE,	// 开始加载场景文件,参数为场景名
	EE_END_LOAD_SCENE,	// 加载场景文件结束,无参数
	EE_SCENE_LOAD_PROGRESS,	// 场景文件加载进度,参数为0~1的浮点数的字符串
	EE_CHARACTER_LOADED,	// 加载角色完成,无参数
	EE_CALCULATE_HEIGHT_MAP_DONE, // 计算场景高度图完成,参数为场景名和计算所使用的时间,单位为秒
	EE_START_LOAD_DYNAMIC_ENTITY,	// 开始加载动态实体,参数为实体名
	EE_DYNAMIC_ENTITY_MODEL_LOADED,	// 动态实体的模型文件加载完成,参数1是实体名,参数2是模型文件名
	EE_DYNAMIC_ENTITY_SKELETON_LOADED,// 动态实体的骨骼文件加载完成,参数1是实体名,参数2是骨骼文件名
	EE_START_LOAD_ENTITY_ANIMATION,		// 开始加载动画,参数1是实体名,参数2是动画的数量
	EE_ONE_ENTITY_ANIMATION_LOADED,	// 一个动画加载完成,参数1是实体名,参数2是独立动画名,参数3是完整动画文件名
	EE_ENTITY_ALL_ANIMATION_LOADED,	// 动态实体的动画全部加载完成,参数1是实体名,参数2是动画数量
	EE_DYNAMIC_ENTITY_LOAD_PROGRESS_CHANGED, // 实体的加载进度改变,参数1是实体名,参数2是实体模型文件名,参数3是加载进度
	EE_END_DYNAMIC_ENTITY_LOAD,	// 动态实体加载完成,参数为实体名
	EE_ENGINE_ERROR_INFO,	// 引擎错误信息,参数1为错误信息内容
	EE_WINDOW_SELECTED,			// 当某个窗口被选中,参数1为窗口名
	EE_WINDOW_UNSELECTED,		// 当某个窗口取消选中,参数1为窗口名
	EE_MOUSE_ENTER_WINDOW,		// 当鼠标进入某个窗口,参数1为窗口名
	EE_MOUSE_LEAVE_WINDOW,		// 当鼠标离开某个窗口,参数1为窗口名
	EE_WINDOW_PREFAB_CHANGED,	// 窗口改变了关联的预设,参数1为窗口名,参数2为改变后的预设名
	EE_PREFAB_RESOURCE_CHANGED,	// 预设资源有改变,无参数
	EE_TEMPLATE_RESOURCE_CHANGED,// 模板资源有改变,无参数
	EE_ERROR_LOG,				// 向编辑层发送错误信息
	EE_MAX,
};

enum ENTITY_COPY_TYPE
{
	ECT_NONE,	// 无拷贝
	ECT_DEEP,	// 深度拷贝
	ECT_SIMPLE,	// 浅拷贝
	ECT_ANIM_SIMPLE,	// 动态模型的浅拷贝,浅拷贝顶点信息和动作,深拷贝骨骼
};

enum PLAY_STATE
{
	PS_PLAY,
	PS_PAUSE,
	PS_STOP,
};

// 包围盒与射线相交检测的面
enum BOX_INTERSECT_FACE
{
	BIF_FRONT,	// 只检测正面
	BIF_BACK,	// 只检测背面
	BIF_BOTH,	// 所有面都检测
};

enum BLEND_MODE
{
	BM_ADD,			// 直接进行颜色相加,大于255的强制为255
	BM_MULTI,		// 颜色各分量相乘
	BM_REPLACE,		// 颜色替换
	BM_MAX,
};

// 窗口渲染的对称方式
enum SYMMETRY_MODE
{
	SM_NONE,					// 不作对称处理
	SM_LEFT_COPY_TO_RIGHT,		// 将左边的直接复制到右边
	SM_LEFT_MIRROR_TO_RIGHT,	// 将左边的镜像复制到右边
	SM_HORI_INVERSE,			// 水平方向上翻转图片
	SM_VERT_INVERSE,			// 竖直方向上翻转图片
	SM_HORI_VERT_MIRROR,		// 水平和竖直都镜像对称
	SM_BOTTOM_COPY_TO_TOP,		// 下边的直接复制到上边
	SM_BOTTOM_MIRROR_TO_TOP,	// 下边的镜像复制到上边
};

enum OBJECT_TYPE
{
	OT_ENTITY,
	OT_DIRECTIONAL_LIGHT,
	OT_BILLBOARD,
	OT_EMITTER,
	OT_MAX,
};

// 组件属性的类型
enum PROPERTY_TYPE
{
	PT_BOOL,
	PT_INT,
	PT_STRING,
	PT_VECTOR2,
	PT_VECTOR3,
	PT_VECTOR4,
	PT_FLOAT,
	PT_ENUM,
	PT_TEXTURE,
	PT_DIM,
	PT_POINT,
};

// 行为执行的状态
enum ACTION_EXECUTE_STATE
{
	AES_NOT_EXECUTE,		// 还未执行
	AES_EXECUTING,			// 正在执行
	AES_PAUSE,				// 暂停执行
	AES_EXECUTE_DONE,		// 执行完毕
};

// windows系统
enum WINDOWS_SYSTEM
{
	WS_NONE,
	WS_WIN_XP,
	WS_WIN_VISTA,
	WS_WIN_7,
	WS_WIN_8,
	WS_WIN_10,
};

enum POLYGON_MODE
{
	PM_LINES = GL_LINES,
	PM_TRIANGLES = GL_TRIANGLES,
};

// 摄像机连接器转换器
enum CAMERA_LINKER_SWITCH
{
	CLS_NONE,
	CLS_LINEAR,
	CLS_CIRCLE,
	CLS_AROUND_TARGET,
};

// 鼠标按钮
enum MOUSE_BUTTON_DEFINE
{
	MBD_NONE,
	MBD_LEFT,
	MBD_RIGHT,
	MBD_MIDDLE,
};

enum TRANS_SPACE
{
	TS_LOCAL,
	TS_WORLD,
};

enum EXECUTE_STATE
{
	ES_NOT_EXECUTE,
	ES_EXECUTING,
	ES_EXECUTED,
};

// 按键状态
enum KEY_PUSH_STATE
{
	KPS_KEEP_UP, // 持续放开
	KPS_KEEP_DOWN, // 持续按下
	KPS_CURRENT_UP, // 当前放开
	KPS_CURRENT_DOWN, // 当前按下
};

// 窗口类型定义
enum WINDOW_TYPE
{
	WT_PREFAB,
	WT_WINDOW,
	WT_BUTTON,
	WT_EDITBOX,
	WT_CHECKBOX,
	WT_NUMBER_WINDOW,
	WT_PROGRESS_WINDOW,
	WT_TEXTURE_WINDOW,
	WT_TEXTURE_ANIM_WINDOW,
	WT_TEXT_WINDOW,
	WT_VIDEO_WINDOW,
	WT_MAX,
};

// 窗口事件,主要是用于脚本的
enum WINDOW_EVENT
{
	WE_CLICKED,	// 窗口被点击
	WE_RETURN,	// 在窗口获得焦点时按下了回车键
	WE_TABLE,	// 在窗口获得焦点时按下了table键
	WE_SELECT,	// 设置为选中状态
	WE_UNSELECT,// 设置为非选中状态
	WE_MOUSE_ENTER,	// 鼠标进入窗口
	WE_MOUSE_LEAVE,	// 鼠标离开窗口
};

// 按钮状态
enum BUTTON_STATE
{
	BS_NORMAL,
	BS_HOVER,
	BS_PUSHED,
	BS_DISABLED,
	BS_MAX_STATE,
};

// UI停靠定义
enum WINDOW_DOCKING_POSITION
{
	WDP_MIN = -1,
	WDP_LEFT,
	WDP_RIGHT,
	WDP_CENTER,
	WDP_MAX,
};

// 窗口大小的自动计算方式
enum AUTO_SIZE_TYPE
{
	AST_NONE,			// 不自动计算大小
	AST_AUTO_HEIGHT,	// 自动计算高度
	AST_AUTO_WIDTH,		// 自动计算宽度
};

// 序列帧播放循环方式
enum LOOP_MODE
{
	LM_NONE,			// 不循环
	LM_NORMAL,			// 头-尾,头-尾循环
	LM_PINGPONG,		// 头-尾,尾-头循环
};

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

class TextureAnimWindow;
class VideoWindow;
// isBreak表示是否是中断了序列帧的播放而导致的调用回调,下同
typedef void(*TextureAnimPlayEndCallback)(TextureAnimWindow* window, void* userData, const bool& isBreak);
// isBreak表示是否是中断了视频的播放而导致的调用回调,下同
typedef void(*VideoPlayEndCallback)(VideoWindow* window, void* userData, const bool& isBreak);

// 录音机的回调,在接收到录音数据时调用
class txWavRecorder;
typedef void(*RecordCallback)(txWavRecorder* recorder, void* userData, char* data, const int& dataSize);

// 动作关键帧的回调函数,curAnimTime为播放到的帧数,不是播放的秒数,cutAnim为true表示是因为动作被切换了而调用的该回调函数
typedef void(*AnimKeyFrameCallback) (void* user_data, const int& aditionalParam, const std::string& animName, const std::string& entityName, const float& curAnimTime, const float& animSpeed, const bool& cutAnim);

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

class txCommand;
class txCommandReceiver;
struct DelayCommand
{
	DelayCommand(const float& delayTime, txCommand* cmd, txCommandReceiver* receiver)
	:
	mDelayTime(delayTime),
	mCommand(cmd),
	mReceiver(receiver)
	{}
	float mDelayTime;
	txCommand* mCommand;
	txCommandReceiver* mReceiver;
};

// 命令回调
typedef void(*CommandCallback)(txCommand* cmd, void* user_data);

// 行为回调
class txAction;
typedef void(*ActionCallback)(txAction* action, void* userData);

// 组件回调
class txComponentKeyFrame;
typedef void(*KeyFrameCallback)(txComponentKeyFrame* component, void* userData, const bool& breakTremling, const bool& done);

// 窗口事件回调
class txTexture;
class Window;
typedef void(*WindowEventCallback)(Window* window, void* userData);

// 布局异步加载回调
class Layout;
typedef void (*LayoutAsyncDone)(Layout* layout, void* userData);

// 线程回调
typedef bool(*CustomThreadCallback)(void* args);

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

#define CMD_CAST static_cast

// 设置value的指定位置pos的字节的值为byte,并且不影响其他字节
#define SET_BYTE(value, byte, pos) value = (value & ~(0x000000ff << (8 * pos))) | (byte << (8 * pos))
// 获得value的指定位置pos的字节的值
#define GET_BYTE(value, pos) (value & (0x000000ff << (8 * pos))) >> (8 * pos)

#define DEBUG_EMPTY "%s", ""

// 再次封装后的容器的遍历宏
#define FOR_STL(stl, expression)									\
	stl.lock(__FILE__, __LINE__);									\
for (expression)

#define END_FOR_STL(stl)											\
	stl.unlock();

#define NEW_CMD(cmd) mCommandSystem->newCmd(cmd, __FILE__, __LINE__, false, false);
#define NEW_CMD_INFO(cmd) mCommandSystem->newCmd(cmd, __FILE__, __LINE__, true, false);
#define NEW_CMD_DELAY(cmd) mCommandSystem->newCmd(cmd, __FILE__, __LINE__, false, true);
#define NEW_CMD_DELAY_INFO(cmd) mCommandSystem->newCmd(cmd, __FILE__, __LINE__, true, true);

// 命令调试信息宏
#define COMMAND_DEBUG(...)															\
{																					\
	static char strBuf[512];														\
	SPRINTF(strBuf, 512, __VA_ARGS__);												\
	std::string cmdName = typeid(*this).name();										\
	cmdName = cmdName.substr(strlen("class "), cmdName.length() - strlen("class "));\
	if (strlen(strBuf) > 0)															\
	{																				\
		cmdName += std::string(" : ") + strBuf;										\
	}																				\
	return cmdName;																	\
}

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

const std::string ONE_ZERO = "OneZero";
const std::string ONE_ZERO_ONE = "OneZeroOne";
const std::string ONE_ZERO_ONE_CURVE = "OneZeroOne_Curve";
const std::string QUADRATIC_CURVE = "Quadratic_Curve";
const std::string SIN_CURVE = "Sin_Curve";
const std::string ZERO_ONE = "ZeroOne";
const std::string ZERO_ONE_ZERO = "ZeroOneZero";
const std::string ZERO_ONE_DELAY = "ZeroOne_Delay";

#endif