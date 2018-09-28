#ifndef _MEMORY_INFO_H_
#define _MEMORY_INFO_H_

#include "ServerDefine.h"

struct MemoryInfo
{
	MemoryInfo(int s, const std::string& f, int l, const std::string& t)
		:
		size(s),
		file(f),
		line(l),
		type(t)
	{}
	int size;			// 内存大小
	std::string file;	// 开辟内存的文件
	int line;			// 开辟内存的代码行号
	std::string type;	// 内存的对象类型
};

struct MemoryType
{
	MemoryType(const std::string& t = "")
		:
		type(t),
		count(0),
		size(0)
	{}
	MemoryType(const std::string& t, int c, int s)
		:
		type(t),
		count(c),
		size(s)
	{}
	std::string type;
	int count;
	int size;
};

#endif