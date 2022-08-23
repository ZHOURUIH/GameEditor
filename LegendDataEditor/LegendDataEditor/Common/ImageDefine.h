#ifndef _IMAGE_DEFINE_H_
#define _IMAGE_DEFINE_H_

#include "ServerDefine.h"
#include "txMemoryTrace.h"

// WIX 文件头格式 
struct WIXFileImageInfo
{
	char    mStrHeader[44];     // 库文件标题 'WEMADE Entertainment inc.' WIL文件头
	int     mIndexCount;   // 图片数量
	myVector<int>    mPositionList;    // 起始位置数组,长度等于nIndexCount
};

struct WILFileHeader
{
	char mInfo[44];
	int mImageCount;
	int mColorCount;
	int mColorPadSize;
	unsigned char* mColor;
	~WILFileHeader()
	{
		DELETE_ARRAY(mColor);
	}
};
const int ColorPadIndex = 44 + 12;

struct WILFileImageInfo
{
	short mWidth;		// 图片宽
	short mHeight;		// 图片高
	short mPosX;		// 图片像素偏移X
	short mPosY;		// 图片像素偏移Y
	unsigned char* mColor;		// 长度为mWidth * mHeight * 4
};
const int ImageHeaderLength = 8;

struct HumanActionInfo
{
	string mName;		// 动作名称
	int mMaxFrame;		// 该动作最大帧数,指资源中该动作的所有帧数,可能包含无效帧
};

const int DIRECTION_COUNT = 8;
const int NPC_DIRECTION_COUNT = 3;	// NPC资源只有3个方向的动作
const int HUMAN_ACTION_COUNT = 11;
const int HUMAN_GROUP_SIZE = 600;
const int EFFECT_GROUP_SIZE = 10;
// 角色,武器,翅膀的格式一致
static HumanActionInfo HUMAN_ACTION[HUMAN_ACTION_COUNT] =
{
	{ "stand", 8 },
	{ "walk", 8 },
	{ "run", 8 },
	{ "noname", 1 },
	{ "attack", 8 },
	{ "dig", 8 },
	{ "jumpAttack", 8 },
	{ "skill", 8 },
	{ "search", 2 },
	{ "hit", 8 },
	{ "die", 8 },
};

struct MonsterActionInfo 
{
	string mName;
	int mFrameCount;
};

const int MONSTER_ACTION_COUNT = 5;
const int MONSTER_GROUP_SIZE = 360;
static MonsterActionInfo MONSTER_ACTION[MONSTER_ACTION_COUNT] =
{
	{ "stand", 4 },
	{ "walk", 6 },
	{ "attack", 6 },
	{ "die", 10 },
	{ "hit", 2 },
};

struct NPCActionInfo
{
	string mName;	// 动作名称
	int mMaxFrame;		// 该动作最大帧数,指资源中该动作的所有帧数,可能包含无效帧
	int mFrameCount;	// 动作的有效帧数
};

const int NPC_ACTION_COUNT = 2;
const int NPC_GROUP_SIZE = 60;
static NPCActionInfo NPC_ACTION[NPC_ACTION_COUNT] =
{
	{ "stand0", 10, 4 },
	{ "stand1", 10, 10 },
};

enum IMAGE_TYPE
{
	IT_HUMAN,
	IT_WEAPON,
	IT_MONSTER,
	IT_EFFECT,
	IT_NPC,
};

// 一块地砖中的三角形定义
enum class TILE_TRIANGLE : byte
{
	LEFT_TOP,            // 左上角
	RIGHT_TOP,           // 右上角
	RIGHT_BOTTOM,        // 右下角
	LEFT_BOTTOM,         // 左下角
	INNER_LEFT_TOP,      // 中心左上角
	INNER_RIGHT_TOP,     // 中心右上角
	INNER_RIGHT_BOTTOM,  // 中心右下角
	INNER_LEFT_BOTTOM,   // 中心左下角
	MAX,
};

const int TILE_WIDTH = 48;				// 地砖的像素宽度
const int TILE_HEIGHT = 32;				// 地砖的像素高度

// 商品所属的商店类型
enum class GOODS_SHOP_TYPE : byte
{
	NPC,			// NPC商店
	GUILD,			// 行会商店
};

#endif