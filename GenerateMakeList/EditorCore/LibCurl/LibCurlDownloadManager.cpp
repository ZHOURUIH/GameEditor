#include "LibCurlDownloadManager.h"
#include "EditorCoreLog.h"
#include "StringUtility.h"

void LibCurlDownloadManager::init()
{
	mFinish = true;
	mRunning = true;
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
	while (!mFinish){}
}

void LibCurlDownloadManager::download(const std::string& fullURL, const std::string& fileName, ProgressCallback progress,
	DownloadingCallback downloading, StartDownload start, FinishDownload finish, void* userData)
{
	LOCK(mListLock, LT_WRITE);
	DownloadInfo* info = TRACE_NEW(DownloadInfo, info);
	// 下载地址里需要将空格替换为%20
	info->mURL = StringUtility::strReplaceAll(fullURL, " ", "%20");
	info->mFileName = fileName;
	info->mProgressCallback = progress;
	info->mDownloadingCallback = downloading;
	info->mStartDownload = start;
	info->mFinishDownload = finish;
	info->mUserData = userData;
	mDownloadList.push_back(info);
	UNLOCK(mListLock, LT_WRITE);
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
	downloadManager->mFinish = false;
	while (downloadManager->mRunning)
	{
		LOCK(downloadManager->mListLock, LT_WRITE);
		if (downloadManager->mDownloadList.size() == 0)
		{
			UNLOCK(downloadManager->mListLock, LT_WRITE);
			continue;
		}
		DownloadInfo* info = downloadManager->mDownloadList[0];
		downloadManager->mDownloadList.erase(downloadManager->mDownloadList.begin());
		UNLOCK(downloadManager->mListLock, LT_WRITE);
		if (info->mStartDownload != NULL)
		{
			// 文件太大时会获取失败
			int length = (int)downloadManager->getDownloadFileLenth(info->mURL.c_str());
			info->mStartDownload(info->mUserData, info->mFileName, length);
		}
		CURL* curl = curl_easy_init();
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
				// 取消下载;
			}
			else if (retCode == CURLE_COULDNT_CONNECT)
			{
				EDITOR_CORE_INFO_DELAY("无法连接至服务器!");
			}
			else
			{
				EDITOR_CORE_INFO_DELAY("下载失败 : %s", strError.c_str());
			}
			downloadManager->mFinish = true;
			downloadManager->mRunning = false;
		}
		//清理curl，和前面的初始化匹配  
		if (info->mFinishDownload != NULL)
		{
			info->mFinishDownload(info->mUserData, info->mFileName, retCode == CURLE_OK);
		}
		TRACE_DELETE(info);
	}
	downloadManager->mFinish = true;
	return 0;
}