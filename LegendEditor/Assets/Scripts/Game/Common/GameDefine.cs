using System;
using UnityEngine;
using System.Collections;
using System.Collections.Generic;

// 游戏枚举定义-----------------------------------------------------------------------------------------------
// 界面布局定义
public enum LAYOUT_TYPE
{
	//LT_OBJECT_EDITOR,
	LT_SCENE_EDITOR,
	LT_SCENE_ADVANCE_EDITOR,
	LT_EDITOR_SELECT,
	LT_EFFECT_TEXTURE_EDITOR,
	LT_EFFECT_SPRITE_EDITOR,
	LT_MONSTER_SPRITE_EDITOR,
	LT_HUMAN_SPRITE_EDITOR,
	LT_MAX,
};
// 音效定义
public enum SOUND_DEFINE
{
	SD_MIN = -1,
	SD_MAX,
};
// 场景的类型
public enum GAME_SCENE_TYPE
{
	GST_START,
	GST_MAIN,
	GST_MAX,
};
// 游戏场景流程类型
public enum PROCEDURE_TYPE
{
	PT_NONE,

	PT_START_MIN,
	PT_START_EDITOR_SELECT,
	PT_START_OBJECT_EDITOR,
	PT_START_EFFECT_TEXTURE_EDITOR,
	PT_START_EFFECT_SPRITE_EDITOR,
	PT_START_MONSTER_SPRITE_EDITOR,
	PT_START_HUMAN_SPRITE_EDITOR,
	PT_START_SCENE_EDITOR,
	PT_START_SCENE_ADVANCE_EDITOR,
	PT_START_EXIT,
	PT_START_MAX,
};
// 游戏中的公共变量定义
public enum GAME_DEFINE_FLOAT
{
	GDF_NONE,
	// 应用程序配置参数
	GDF_APPLICATION_MIN,
	GDF_FULL_SCREEN,                // 是否全屏,0为窗口模式,1为全屏,2为无边框窗口
	GDF_SCREEN_WIDTH,               // 分辨率的宽
	GDF_SCREEN_HEIGHT,              // 分辨率的高
	GDF_ADAPT_SCREEN,               // 屏幕自适应的方式,0为基于锚点的自适应,可以根据不同分辨率调整布局排列,1为简单拉伸,2为多屏拼接后复制显示
	GDF_SCREEN_COUNT,               // 显示屏数量,用于多屏横向组合为高分辨率,只能在GDF_ADAPT_SCREEN为2的情况下使用
	GDF_USE_FIXED_TIME,             // 是否将每帧的时间固定下来
	GDF_FIXED_TIME,                 // 每帧的固定时间,单位秒
	GDF_VSYNC,                      // 垂直同步,0为关闭垂直同步,1为开启垂直同步
	GDF_APPLICATION_MAX,

	// 框架配置参数
	GDF_FRAME_MIN,
	GDF_SOCKET_PORT,                // socket端口
	GDF_BROADCAST_PORT,             // 广播端口
	GDF_LOAD_RESOURCES,             // 游戏加载资源的路径,0代表在Resources中读取,1代表从AssetBundle中读取
	GDF_LOG_LEVEL,                  // 日志输出等级
	GDF_ENABLE_KEYBOARD,            // 是否响应键盘按键
	GDF_FRAME_MAX,

	// 游戏配置参数
	GDF_GAME_MIN,
	GDF_HEART_BEAT_NITERVAL,        // 心跳间隔时间
	GDF_GAME_MAX,
};
public enum GAME_DEFINE_STRING
{
	GDS_NONE,
	// 应用程序配置参数
	GDS_APPLICATION_MIN,
	GDS_APPLICATION_MAX,

	// 框架配置参数
	GDS_FRAME_MIN,
	GDS_FRAME_MAX,

	// 游戏配置参数
	GDS_GAME_MIN,
	GDS_TCP_SERVER_IP,  // 服务器IP
	GDS_TCP_HOST_NAME,	// 服务器域名,域名和IP只需要填一个,都填则使用IP
	GDS_ACCOUNT,
	GDS_PASSWORD,
	GDS_GAME_MAX,
};

// 表格数据类型
public enum DATA_TYPE
{
	DT_GAME_SOUND,
	DT_MAX,
}
// 游戏常量定义-------------------------------------------------------------------------------------------------------------
public class GameDefine : CommonDefine
{
	// 路径定义
	//-----------------------------------------------------------------------------------------------------------------
	// 常量定义
	//-----------------------------------------------------------------------------------------------------------------
	public const int DIRECTION_COUNT = 8;
	public const int MAX_OCCUPATION = 6;
	public const float NORMAL_ANIM_SPEED = 0.25f;   // 动作的正常播放速度
}