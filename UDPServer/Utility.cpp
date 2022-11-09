#include "Utility.h"

const Array<11, llong> Utility::POWER_INT_10{ 1L, 10L, 100L, 1000L, 10000L, 100000L, 1000000L, 10000000L, 100000000L, 1000000000L, 10000000000L };

void Utility::makeSockAddr(sockaddr_in& addr, const ulong hostlong, const ushort port)
{
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
#ifdef WIN32
	addr.sin_addr.S_un.S_addr = htonl(hostlong);
#else
	addr.sin_addr.s_addr = htonl(hostlong);
#endif
}

string Utility::getInternetIP(MY_SOCKET socket)
{
#ifdef WIN32
	SOCKADDR_IN addr_conn;
	int nSize = sizeof(addr_conn);
	memset((void*)&addr_conn, 0, sizeof(addr_conn));
	getpeername(socket, (SOCKADDR*)(&addr_conn), &nSize);
	return string(inet_ntoa(addr_conn.sin_addr));
#else
	struct sockaddr_in addr_conn;
	int nSize = sizeof(addr_conn);
	memset((void*)&addr_conn, 0, sizeof(addr_conn));
	getpeername(socket, (struct sockaddr*)(&addr_conn), (socklen_t*)&nSize);
	return string(inet_ntoa(addr_conn.sin_addr));
#endif
}

string Utility::getLocalIP(MY_SOCKET socket)
{
#ifdef WIN32
	SOCKADDR_IN addr_conn;
	int nSize = sizeof(addr_conn);
	memset((void*)&addr_conn, 0, sizeof(addr_conn));
	getsockname(socket, (SOCKADDR*)(&addr_conn), &nSize);
	return string(inet_ntoa(addr_conn.sin_addr));
#else
	struct sockaddr_in addr_conn;
	int nSize = sizeof(addr_conn);
	memset((void*)&addr_conn, 0, sizeof(addr_conn));
	getsockname(socket, (struct sockaddr*)(&addr_conn), (socklen_t*)&nSize);
	return string(inet_ntoa(addr_conn.sin_addr));
#endif
}

llong Utility::getRealTimeMS()
{
#ifdef WIN32
	return timeGetTime();
#else
	struct timeval tv;
	if (gettimeofday(&tv, nullptr) != 0)
	{
		ERROR("time get error : " + intToString(errno));
	}
	return tv.tv_sec * 1000 + (llong)(tv.tv_usec * 0.001f);
#endif
}

void Utility::sleep(ulong timeMS)
{
#ifdef WIN32
	Sleep(timeMS);
#else
	usleep(timeMS * 1000);
#endif
}

string Utility::intToString(const int value, const int limitLen)
{
	Array<16> temp{ 0 };
	const int len = _itoa_s(value, temp);
	if (limitLen > len)
	{
		return zeroString(limitLen - len) + temp.str();
	}
	return temp.str();
}

string Utility::zeroString(const int zeroCount)
{
	Array<16> charArray{ 0 };
	for (int i = 0; i < zeroCount; ++i)
	{
		charArray[i] = '0';
	}
	charArray[zeroCount] = '\0';
	return charArray.str();
}

string Utility::getTime(const bool timeStamp)
{
	Array<128> timeBuffer{ 0 };
#ifdef WIN32
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	if (timeStamp)
	{
		SPRINTF(timeBuffer.toBuffer(), timeBuffer.size(), "%d-%02d-%02d %02d:%02d:%02d", sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond);
	}
	else
	{
		SPRINTF(timeBuffer.toBuffer(), timeBuffer.size(), "%04d年%02d月%02d日%02d时%02d分%02d秒%03d毫秒", sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds);
	}
#else
	time_t tt;
	time(&tt);
	struct tm date;
	localtime_r(&tt, &date);
	if (timeStamp)
	{
		SPRINTF(timeBuffer.toBuffer(), timeBuffer.size(), "%d-%02d-%02d %02d:%02d:%02d", date.tm_year + 1900, date.tm_mon + 1, date.tm_mday, date.tm_hour, date.tm_min, date.tm_sec);
	}
	else
	{
		SPRINTF(timeBuffer.toBuffer(), timeBuffer.size(), "%04d年%02d月%02d日%02d时%02d分%02d秒", date.tm_year + 1900, date.tm_mon + 1, date.tm_mday, date.tm_hour, date.tm_min, date.tm_sec);
	}
#endif
	return timeBuffer.str();
}