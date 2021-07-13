using System;
using UnityEngine;
using System.Collections;
using System.Collections.Generic;

// 游戏枚举定义-----------------------------------------------------------------------------------------------
// 界面布局定义
public enum LAYOUT
{
	L_SCENE,
	L_MAX,
};
// 音效定义
public enum SOUND_DEFINE
{
	SD_MIN = -1,
	SD_MAX,
};
// 场景的类型
public enum GAME_SCENE
{
	GS_START,
	GS_MAIN,
	GS_MAX,
};
// 游戏场景流程类型
public enum PROCEDURE
{
	P_NONE,
	P_START_MIN,
	P_START_EDITOR_SELECT,
	P_START_SCENE_EDITOR,
	P_START_EXIT,
	P_START_MAX,
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
	GDF_USE_FIXED_TIME,             // 是否将每帧的时间固定下来
	GDF_FIXED_TIME,                 // 每帧的固定时间,单位秒
	GDF_VSYNC,                      // 垂直同步,0为关闭垂直同步,1为开启垂直同步
	GDF_FORCE_TOP,                  // 是否将窗口置顶
	GDF_APPLICATION_MAX,

	// 框架配置参数
	GDF_FRAME_MIN,

	GDF_LOAD_RESOURCES,             // 游戏加载资源的路径,0代表在Resources中读取,1代表从AssetBundle中读取
	GDF_LOG_LEVEL,                  // 日志输出等级
	GDF_ENABLE_KEYBOARD,            // 是否响应键盘按键
	GDF_PERSISTENT_DATA_FIRST,      // 当从AssetBundle加载资源时,是否先去persistentDataPath中查找资源
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
	GDS_GAME_MAX,
};

// 表格数据类型
public enum DATA_TYPE
{
	DT_GAME_SOUND,
	DT_MAX,
}

// 角色装备的类型
public enum EUIQP_TYPE
{
	ET_CLOTH,
	ET_WEAPON,
	ET_HELMET,
}
public enum CHARACTER_TYPE
{
	CT_NORMAL,
	CT_AI,
	CT_OTHER,
	CT_MYSELF,
	CT_MAX,
}
public enum PLAYER_STATE
{
	PS_MAX,
}
public enum STATE_GROUP
{
	SG_BEHAVIOUR,   // 角色行为
	SG_BUFF,        // 角色buff和debuff
}
// 一块地砖中的三角形定义
public enum TILE_TRIANGLE
{
	TT_LEFT_TOP,            // 左上角
	TT_RIGHT_TOP,           // 右上角
	TT_RIGHT_BOTTOM,        // 右下角
	TT_LEFT_BOTTOM,         // 左下角
	TT_INNER_LEFT_TOP,      // 中心左上角
	TT_INNER_RIGHT_TOP,     // 中心右上角
	TT_INNER_RIGHT_BOTTOM,  // 中心右下角
	TT_INNER_LEFT_BOTTOM,   // 中心左下角
	TT_MAX,
}
public enum PACKET_TYPE
{
	PT_MAX,
}
// 游戏常量定义-------------------------------------------------------------------------------------------------------------
public class GameDefine : CommonDefine
{
	// 路径定义
	public const string EFFECT = "Effect";
	public const string MAP = "Map";
	public const string MAGIC = "Magic";
	public const string MONSTER_MAGIC = "MonsterMagic";
	public const string MONSTER = "Monster";
	public const string MINI_MAP = "MiniMap";
	public const string NPC = "NPC";
	public const string R_ATLAS_EFFECT_PATH = R_ATLAS_TEXTURE_ANIM_PATH + EFFECT + "/";
	public const string R_ATLAS_EFFECT_MAGIC_PATH = R_ATLAS_EFFECT_PATH + MAGIC + "/";
	public const string R_ATLAS_EFFECT_MONSTER_MAGIC_PATH = R_ATLAS_EFFECT_PATH + MONSTER_MAGIC + "/";
	public const string R_ATLAS_EFFECT_MAP_PATH = R_ATLAS_EFFECT_PATH + MAP + "/";
	public const string R_MINI_MAP_PATH = R_GAME_TEXTURE_PATH + MINI_MAP + "/";
	public static string F_MAP_PATH = F_STREAMING_ASSETS_PATH + MAP + "/";
	//-----------------------------------------------------------------------------------------------------------------
	// 常量定义
	//-----------------------------------------------------------------------------------------------------------------
	public const int DIRECTION_COUNT = 8;
	public const int MAX_OCCUPATION = 6;
	public const float NORMAL_ANIM_SPEED = 0.25f;   // 动作的正常播放速度
	public const int NAME_LENGTH = 16;              // 角色名字最大长度
	public const int CHARACTER_COUNT = 2;           // 每个账号的角色数量
	public const int NPC_DIRECTION_COUNT = 3;       // NPC只有3个方向的动作
	public const int NPC_DIRECTION_START = 3;       // NPC的方向只有3,4,5
	public const int MONSTER_BATCH = 64;            // 进入地图获取怪物列表时,每个包最多包含64个怪物信息
	public const int NPC_BATCH = 16;                // 获取NPC列表时,每个包最多包含16个NPC信息
	public const int OTHER_PLAYER_BATCH = 32;       // 获取其他玩家列表时,每个包最多包含16个玩家的信息
	public const float HUMAN_ANIM_SPEED = 0.35f;    // 动作的正常播放速度
	public const float MONSTER_ANIM_SPEED = 0.08f;   // 怪物动作的正常播放速度
	public const float NPC_ANIM_SPEED = 0.15f;      // NPC动作的正常播放速度
	public const int WINDOW_PER_PANEL = 5000;       // 地图布局中存放BackTile的单个Panel中最多可存放的BackTile的数量
	public static int TILE_VIEW_WIDTH = 800;          // 地图中可见视野中横向地砖的数量
	public static int TILE_VIEW_HEIGHT = 800;         // 地图中可见视野中纵向地砖的数量
	public const int TILE_WIDTH = 48;               // 地砖的像素宽度
	public const int TILE_HEIGHT = 32;              // 地砖的像素高度
	public const int CHAR_PACK_GRID = 8 * 5;        // 角色最大的背包格子数量
	public const int OBJECT_BATCH = 64;             // 单次请求物品属性的最大数量,当需要请求的数量超过该值时,需要分包请求
	public const int OBJECT_LABEL_LENGTH = 32;      // 物品名称的最大长度
	public const int FAST_ITEM_GIRD = 6;            // 快捷栏数量
	public const int CHAT_SHOW_COUNT = 8;           // 聊天界面显示的聊天数量
	public const int CHAT_LENGTH = 256;             // 单条聊天消息的最大长度
	public const float SHOW_POINT_DISTANCE = 600.0f * 600.0f;     // 距离多少像素内的物体(怪物,其他玩家)会显示在小地图上,以及会接收该范围内的玩家移动,攻击消息
	public const float SHOW_NPC_POINT_DISTANCE = 1200.0f * 1200.0f;// 距离多少像素内的NPC会显示在小地图上
	public const int DEFAULT_HUMAN_SHAPE = 0;       // 男女角色没有穿衣服时在场景中的外观ID
	public const int DEFAULT_MALE_LOOKS = 593;      // 男角色没有穿衣服时在装备面板中的外观ID
	public const int DEFAULT_FEMALE_LOOKS = 603;    // 女角色没有穿衣服时在装备面板中的外观ID
	public const float VISIBLE_DISTANCE = 600.0f * 600.0f;  // 距离角色多少像素内的怪物,NPC,其他玩家可见
	public const int ATTACK_TARGET_BATCH = 8;       // 一个包可发送的攻击目标数量
	// Frame需要的常量,因为Frame中需要该变量,但是每个项目的值都可能不一致,所以放到GameDefine中
	//-----------------------------------------------------------------------------------------------------------------
	// UI的制作标准,所有UI都是按1920*1080标准分辨率制作的
	public const int STANDARD_WIDTH = 800;
	public const int STANDARD_HEIGHT = 600;
	// 数据库文件名
	public const string DATA_BASE_FILE_NAME = "DataBase.db";
	// 清理时需要保留的目录和目录的meta
	public static string[] mKeepFolder = new string[] { "Config", "GameDataFile", "GamePlugin", "DataBase", "Video", "DataTemplate", "HelperExe", "CustomSound", "Map" };
	// Resources下的目录,带相对路径,且如果前缀符合,也会认为是不打包的目录
	public static string[] mUnPackFolder = new string[] { "Scene" };
}