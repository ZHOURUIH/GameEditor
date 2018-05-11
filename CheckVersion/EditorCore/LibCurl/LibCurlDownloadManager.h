#ifndef _LIB_CURL_DOWNLOAD_MANAGER_H_
#define _LIB_CURL_DOWNLOAD_MANAGER_H_

#include "CommonDefine.h"
#include "EditorCoreBase.h"
#include "curl_windows/curl.h"
#include "txThreadLock.h"

typedef int(*ProgressCallback)(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow);
typedef int(*DownloadingCallback)(void* pBuffer, size_t nSize, size_t nMemByte, void* pParam);
typedef void(*StartCallback)(void* userData, const std::string& fileName, const int& fileLength);
typedef void(*FinishCallback)(void* userData, const std::string& fileName, const bool& success);

class DownloadInfo
{
public:
	DownloadInfo()
	{
		mUserData = NULL;
		mProgressCallback = NULL;
		mDownloadingCallback = NULL;
		mStartCallback = NULL;
		mFinishCallback = NULL;
	}
public:
	std::string mURL;
	std::string mFileName;
	std::string mUploadPath;	// 上传后的路径
	void * mUserData;
	ProgressCallback mProgressCallback;
	DownloadingCallback mDownloadingCallback;
	StartCallback mStartCallback;
	FinishCallback mFinishCallback;
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
		DownloadingCallback downloading, StartCallback start, FinishCallback finish, void* userData);
	static DWORD WINAPI downloadingThread(LPVOID lpParameter);
	double getDownloadFileLenth(const std::string& url);
protected:
	txThreadLock mDownloadListLock;
	txVector<DownloadInfo*> mDownloadList;
	HANDLE mDownloadingThread;
	std::string mLocalPath;
	volatile std::atomic<bool> mDownloadFinish;
	volatile std::atomic<bool> mRunning;
};

#endif