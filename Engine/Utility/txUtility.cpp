#include "txEngineDefine.h"
#include "txUtility.h"
#include "txEngineLog.h"
#include "txEngineRoot.h"
#include "txRenderWindow.h"

WINDOWS_SYSTEM txUtility::getWindowsSystem()
{
	static txMap<std::string, WINDOWS_SYSTEM> windowsSystem;
	if (windowsSystem.size() == 0)
	{
		windowsSystem.insert("5.1", WS_WIN_XP);
		windowsSystem.insert("6.0", WS_WIN_VISTA);
		windowsSystem.insert("6.1", WS_WIN_7);
		windowsSystem.insert("6.2", WS_WIN_8);
		windowsSystem.insert("6.3", WS_WIN_10);
	}
	// GetVersion() 对于判断Win10系统不太合适,需要通过注册表获得
	HKEY hKey;
	LONG nRet = RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", 0, KEY_ALL_ACCESS, &hKey);
	if (nRet != ERROR_SUCCESS)
	{
		return WS_NONE;
	}
	char szCurrentVersion[32] = { 0 };
	DWORD dwSize = 100;
	DWORD type;
	nRet = RegQueryValueExA(hKey, "CurrentVersion", NULL, &type, (BYTE *)szCurrentVersion, &dwSize);
	if (nRet != ERROR_SUCCESS)
	{
		return WS_NONE;
	}
	auto iter = windowsSystem.find(szCurrentVersion);
	if (iter != windowsSystem.end())
	{
		return iter->second;
	}
	else
	{
		return WS_NONE;
	}
}

void txUtility::copyTextToClipbord(const std::string& text)
{
	int strLen = text.length();
	HANDLE hGlobalMemory = GlobalAlloc(GHND, strLen + 1); // 分配内存  
	LPBYTE lpGlobalMemory = (LPBYTE)GlobalLock(hGlobalMemory); // 锁定内存
	memcpy(lpGlobalMemory, text.c_str(), strLen);
	GlobalUnlock(hGlobalMemory);				// 解锁内存块  
	HWND hWnd = mRenderWindow->getHWND();		// 获取安全窗口句柄  
	::OpenClipboard(hWnd);						// 打开剪贴板  
	::EmptyClipboard();							// 清空剪贴板  
	::SetClipboardData(CF_TEXT, hGlobalMemory); // 将内存中的数据放置到剪贴板  
	::CloseClipboard();							// 关闭剪贴板
}

std::string txUtility::getTime()
{
	char timeBuffer[64] = { 0 };
#if RUN_PLATFORM == PLATFORM_WINDOWS
	static SYSTEMTIME sys;
	GetLocalTime(&sys);
	SPRINTF(timeBuffer, 64, "%d时%d分%d秒%d毫秒", sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds);
#elif RUN_PLATFORM == PLATFORM_ANDROID
	static struct timeval tv;
	gettimeofday(&tv, NULL);
	unsigned long curMiliTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	const unsigned long& startMiliTime = ServerFramework::getSingletonPtr()->getStartMiliTime();
	unsigned long deltaMiliTime = curMiliTime - startMiliTime;
	unsigned long curSec = deltaMiliTime / 1000;
	unsigned long curMili = deltaMiliTime % 1000;
	SPRINTF(timeBuffer, 64, "%d秒%d毫秒", (int)curSec, (int)curMili);
#endif
	return timeBuffer;
}

void txUtility::logError(const std::string& info)
{
	mEngineRoot->logError(info);
}

void txUtility::sleep(const unsigned long& timeMS)
{
#if RUN_PLATFORM == PLATFORM_WINDOWS
	Sleep(timeMS);
#elif RUN_PLATFORM == PLATFORM_LINUX
	sleep(timeMS);
#endif
}

unsigned long txUtility::getTimeMS()
{
#if RUN_PLATFORM == PLATFORM_WINDOWS
	return timeGetTime();
#elif RUN_PLATFORM == PLATFORM_LINUX
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (unsigned long)(tv.tv_sec * 1000.0f + tv.tv_usec / 1000.0f);
#endif
}