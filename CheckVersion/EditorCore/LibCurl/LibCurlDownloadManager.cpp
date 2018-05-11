#include "LibCurlDownloadManager.h"
#include "EditorCoreLog.h"
#include "Utility.h"

void LibCurlDownloadManager::init()
{
	char name[64];
	GetModuleFileNameA(NULL, name, 64);
	mLocalPath = StringUtility::getParentDir(name);
	mDownloadFinish = true;
	mRunning = true;
	curl_global_init(CURL_GLOBAL_ALL);
	mDownloadingThread = CreateThread(NULL, 0, LibCurlDownloadManager::downloadingThread, this, 0, NULL);
}

void LibCurlDownloadManager::destroy()
{
	int count = mDownloadList.size();
	for (int i = 0; i < count; ++i)
	{
		TRACE_DELETE(mDownloadList[i]);
	}
	mDownloadList.clear();
	mRunning = false;
	while (!mDownloadFinish){}
	curl_global_cleanup();
}

void LibCurlDownloadManager::download(const std::string& fullURL, const std::string& fileName, ProgressCallback progress,
	DownloadingCallback downloading, StartCallback start, FinishCallback finish, void* userData)
{
	LOCK(mDownloadListLock);
	DownloadInfo* info = TRACE_NEW(DownloadInfo, info);
	// 下载地址里需要将空格替换为%20
	info->mURL = StringUtility::strReplaceAll(fullURL, " ", "%20");
	info->mFileName = fileName;
	info->mProgressCallback = progress;
	info->mDownloadingCallback = downloading;
	info->mStartCallback = start;
	info->mFinishCallback = finish;
	info->mUserData = userData;
	mDownloadList.push_back(info);
	UNLOCK(mDownloadListLock);
}

double LibCurlDownloadManager::getDownloadFileLenth(const std::string& url)
{
	double len = 0.0;
	CURL* curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_NOBODY, 1);			// 不需求body
	int ret = curl_easy_perform(curl);
	int infoRet = 0;
	if (ret == CURLE_OK)
	{
		infoRet = curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &len);
		if (infoRet != CURLE_OK)
		{
			EDITOR_CORE_INFO_DELAY("curl_easy_getinfo failed!\n");
		}
	}
	else
	{
		len = -1;
	}
	return len;
}

DWORD WINAPI LibCurlDownloadManager::downloadingThread(LPVOID lpParameter)
{
	LibCurlDownloadManager* downloadManager = (LibCurlDownloadManager*)(lpParameter);
	downloadManager->mDownloadFinish = false;
	while (downloadManager->mRunning)
	{
		LOCK(downloadManager->mDownloadListLock);
		if (downloadManager->mDownloadList.size() == 0)
		{
			UNLOCK(downloadManager->mDownloadListLock);
			continue;
		}
		DownloadInfo* info = downloadManager->mDownloadList[0];
		downloadManager->mDownloadList.erase(downloadManager->mDownloadList.begin());
		UNLOCK(downloadManager->mDownloadListLock);
		if (info->mStartCallback != NULL)
		{
			info->mStartCallback(info->mUserData, info->mFileName, -1);
		}
		CURL* curl = curl_easy_init();
		if (curl == NULL)
		{
			continue;
		}
		curl_easy_setopt(curl, CURLOPT_URL, StringUtility::ANSIToUTF8(info->mURL).c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, info->mDownloadingCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, info->mUserData);
		curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 5);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);
		curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, info->mProgressCallback);
		curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, info->mUserData);
		// 开始执行perform下载文件
		CURLcode retCode = curl_easy_perform(curl);
		std::string strError = curl_easy_strerror(retCode);
		curl_easy_cleanup(curl);
		if (retCode != CURLE_OK)
		{
			if (retCode == CURLE_ABORTED_BY_CALLBACK)
			{
				// 取消下载
				downloadManager->mRunning = false;
			}
			else if (retCode == CURLE_COULDNT_CONNECT)
			{
				EDITOR_CORE_INFO_DELAY("无法连接至服务器!");
				downloadManager->mRunning = false;
			}
			else
			{
				EDITOR_CORE_INFO_DELAY("下载失败 : %d, %s", retCode, strError.c_str());
				downloadManager->mRunning = false;
			}
		}
		//清理curl，和前面的初始化匹配  
		if (info->mFinishCallback != NULL)
		{
			info->mFinishCallback(info->mUserData, info->mFileName, retCode == CURLE_OK);
		}
		TRACE_DELETE(info);
	}
	downloadManager->mDownloadFinish = true;
	return 0;
}