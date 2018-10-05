#ifndef _IMAGE_DEFINE_H_
#define _IMAGE_DEFINE_H_

#include "ServerDefine.h"

// WIX 文件头格式 
struct WIXFileImageInfo
{
	char    mStrHeader[44];     // 库文件标题 'WEMADE Entertainment inc.' WIL文件头
	int     mIndexCount;   // 图片数量
	txVector<int>    mPositionList;    // 起始位置数组,长度等于nIndexCount
};

struct WILFileHeader
{
	char mInfo[44];
	char mPlayInfo[12];
	unsigned char mColor[256][4];
};
const int ColorPadIndex = 44 + 12;

struct WILFileImageInfo
{
	short mWidth;		// 图片宽
	short mHeight;		// 图片高
	short mPosX;		// 图片像素偏移X
	short mPosY;		// 图片像素偏移Y
	char* mColor;		// 长度为mWidth * mHeight * 4
};
const int ImageHeaderLength = 8;

struct ActionInfo
{
	std::string mName;	// 动作名称
	int mMaxFrame;		// 该动作最大帧数,指资源中该动作的所有帧数,可能包含无效帧
	int mFrameCount;	// 动作的有效帧数
};

const int DIRECTION_COUNT = 8;
const int ACTION_COUNT = 11;
const int HUMAN_GROUP_SIZE = 600;
const int WEAPON_GROUP_SIZE = 1200;
const int EFFECT_GROUP_SIZE = 10;
static ActionInfo HUMAN_ACTION[ACTION_COUNT] =
{
	{ "stand", 8, 4 },
	{ "walk", 8, 6 },
	{ "run", 8, 6 },
	{ "noname0", 1, 1 },
	{ "attack", 8, 6 },
	{ "dig", 8, 6 },
	{ "jumpAttack", 8, 8 },
	{ "skill", 8, 6 },
	{ "search", 2, 2 },
	{ "hit", 8, 3 },
	{ "die", 8, 4 },
};

struct MonsterActionInfo 
{
	std::string mName;
	int mFrameCount;
};

const int MONSTER_ACTION_COUNT = 5;
const int MONSTER_GROUP_SIZE = 360;
static MonsterActionInfo MONSTER_ACTION[MONSTER_ACTION_COUNT] =
{
	{ "attack", 4 },
	{ "die", 10 },
	{ "hit", 2 },
	{ "stand", 4 },
	{ "walk", 6 },
};

enum IMAGE_TYPE
{
	IT_HUMAN,
	IT_WEAPON,
	IT_MONSTER,
};

#endif