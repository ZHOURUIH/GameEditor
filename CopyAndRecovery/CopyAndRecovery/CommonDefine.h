#ifndef _COMMON_DEFINE_H_
#define _COMMON_DEFINE_H_

#include <string>

const std::string CONFIG_PATH = "../Config/";

// 游戏中的公共变量定义
enum GAME_DEFINE_FLOAT
{
	GDF_MAX,
};

enum GAME_DEFINE_STRING
{
	GDS_EXCEPT_FILE,			// 排除的文件名
	GDS_COPY_FILE_DIR,			// 源文件目录
	GDS_COPY_FILE_DEST_DIR,		// 目标资源文件目录
	GDS_MAX,
};

#endif