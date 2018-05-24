#ifndef _TX_UTILITY_H_
#define _TX_UTILITY_H_

#include "CommonDefine.h"

class txUtility
{
public:
	// 返回media的路径,不带/
	static std::string getMediaPath()
	{
		return "../media";
	}

	// 通过一个media下的相对路径,或者绝对路径,转化为一个可直接打开的路径
	static std::string getAvailableResourcePath(std::string name)
	{
		std::string mediaPath = txUtility::getMediaPath();
		// 如果文件名已经是不带media路径并且不是绝对路径
		if (mediaPath != "" && (name.length() <= mediaPath.length() || name.substr(0, mediaPath.length()) != mediaPath) && (name.length() > 1 && name[1] != ':'))
		{
			name = mediaPath + "/" + name;
		}
		return name;
	}
};

#endif
