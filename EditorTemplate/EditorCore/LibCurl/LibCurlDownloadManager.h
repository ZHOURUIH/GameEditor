#ifndef _LIB_CURL_DOWNLOAD_MANAGER_H_
#define _LIB_CURL_DOWNLOAD_MANAGER_H_

#include "CommonDefine.h"
#include "EditorCoreBase.h"
#include "curl_windows/curl.h"
#include "txThreadLock.h"

typedef int(*ProgressCallback)(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow);
typedef int(*DownloadingCallback)(void* pBuffer, size_t nSize, size_t nMemByte, void* pParam);
typedef void(*StartDownload)(void* userData, const std::string& fileName, const int& fileLength);
typedef void(*FinishDownload)(void* userData, const std::string& fileName, const bool& downloadSuccess);

class DownloadInfo
{
public:
	DownloadInfo()
	{
		mUserData = NULL;
		mProgressCallback = NULL;
		mDownloadingCallback = NULL;
		mStartDownload = NULL;
		mFinishDownload = NULL;
	}
public:
	std::string mURL;
	std::string mFileName;
	void * mUserData;
	ProgressCallback mProgressCallback;
	DownloadingCallback mDownloadingCallback;
	StartDownload mStartDownload;
	FinishDownload mFinishDownload;
};

class LibCurlDownloadManager : EditorCoreBase
{
public:
	LibCurlDownloadManager()
	{
		;
	}
	virtual ~LibCurlDownloadManager(){ destroy(); }
	void init();
	void destroy();
	void update(float elapsedTime){}
	void download(const std::string& fullURL, const std::string& fileName, ProgressCallback progress, 
		DownloadingCallback downloading, StartDownload start, FinishDownload finish, void* userData);
	static DWORD WINAPI downloadingThread(LPVOID lpParameter);
	double getDownloadFileLenth(const std::string& url);
protected:
	txThreadLock mListLock;
	txVector<DownloadInfo*> mDownloadList;
	HANDLE mDownloadingThread;
	volatile bool mFinish;
	volatile bool mRunning;
};

#endif