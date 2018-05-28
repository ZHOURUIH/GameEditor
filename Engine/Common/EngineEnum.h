#ifndef _ENGINE_ENUM_H_
#define _ENGINE_ENUM_H_

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

#endif