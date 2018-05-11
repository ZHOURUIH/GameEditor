#ifndef _COMMON_DEFINE_H_
#define _COMMON_DEFINE_H_

#include <string>
#include "Utility.h"

const std::string CONFIG_PATH = "Config/";

// 游戏中的公共变量定义
enum GAME_DEFINE_FLOAT
{
	GDF_MAX,
};

enum GAME_DEFINE_STRING
{
	GDS_COPY_RESOUCE_DIR,		// 源exe文件目录
	GDS_COPY_DEST_DIR,			// 目标exe文件目录
	GDS_COPY_DEST_NAME,			// exe 名文件目录
	GDS_COPY_RESOUCE_MEDIA_DIR, // 源media文件目录
	GDS_COPY_DEST_MEDIA_DIR,	// 目标media文件目录
	GDS_COPY_DEST_GAME_NAME,	// 游戏名称
	GDS_EXCEPT_FILE,			// 排除的文件名
	GDS_MAX,
};
#endif