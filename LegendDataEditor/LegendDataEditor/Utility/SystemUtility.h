#ifndef _TX_UTILITY_H_
#define _TX_UTILITY_H_

#include "txMemoryTrace.h"
#include "ThreadLock.h"

class SystemUtility
{
	static ThreadLock mTimeLock;
public:
	static void stop();
	static void sleep(unsigned long timeMS);
	static long getTimeMS();
	static const char* getTime();
	// 返回media的路径,不带/
	static const std::string& getMediaPath()
	{
		return MEDIA_PATH;
	}
	// 通过一个media下的相对路径,或者绝对路径,转化为一个可直接打开的路径
	static std::string getAvailableResourcePath(std::string name)
	{
		std::string mediaPath = getMediaPath();
		// 如果文件名已经是不带media路径并且不是绝对路径
		if (mediaPath != "" && (name.length() <= mediaPath.length() || name.substr(0, mediaPath.length()) != mediaPath) && (name.length() > 1 && name[1] != ':'))
		{
			name = mediaPath + "/" + name;
		}
		return name;
	}

	static std::string getIPFromSocket(const TX_SOCKET& socket)
	{
#if RUN_PLATFORM == PLATFORM_WINDOWS
		SOCKADDR_IN addr_conn;
#elif RUN_PLATFORM == PLATFORM_LINUX
		struct sockaddr_in addr_conn;
#endif
		int nSize = sizeof(addr_conn);
		memset((void *)&addr_conn, 0, sizeof(addr_conn));
#if RUN_PLATFORM == PLATFORM_WINDOWS
		getpeername(socket, (SOCKADDR *)(&addr_conn), &nSize);
#elif RUN_PLATFORM == PLATFORM_LINUX
		getpeername(socket, (struct sockaddr *)(&addr_conn), (socklen_t*)&nSize);
#endif
		return std::string(inet_ntoa(addr_conn.sin_addr));
	}

	static void print(const std::string& str)
	{
#if RUN_PLATFORM == PLATFORM_WINDOWS
		std::cout << str;
#elif RUN_PLATFORM == PLATFORM_LINUX
		printf(str.c_str());
#endif
	}
	static void input(std::string& str)
	{
		scanf_s(str.c_str());
#if RUN_PLATFORM == PLATFORM_WINDOWS
		//std::cin >> str;
#elif RUN_PLATFORM == PLATFORM_LINUX
		scanf(str.c_str());
#endif
	}
};

#endif
