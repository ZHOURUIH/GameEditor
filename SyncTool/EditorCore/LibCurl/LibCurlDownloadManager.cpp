#include "LibCurlDownloadManager.h"
#include "EditorCoreLog.h"
#include "Utility.h"
#include "EditorCore.h"

void LibCurlDownloadManager::init()
{
	char name[64];
	GetModuleFileNameA(NULL, name, 64);
	mLocalPath = StringUtility::getParentDir(name);
	mDownloadFinish = true;
	mUploadFinish = true;
	mRunning = true;
	curl_global_init(CURL_GLOBAL_ALL);
	mDownloadingThread = CreateThread(NULL, 0, LibCurlDownloadManager::downloadingThread, this, 0, NULL);
	mUploadingThread = CreateThread(NULL, 0, LibCurlDownloadManager::uploadingThread, this, 0, NULL);
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
	while (!mUploadFinish){}
	curl_global_cleanup();
}

void LibCurlDownloadManager::upload(const std::string& fullURL, const std::string& fileName, const std::string& uploadPath, ProgressCallback progress,
	DownloadingCallback downloading, StartCallback start, FinishCallback finish, void* userData)
{
	LOCK(mUploadListLock);
	DownloadInfo* info = TRACE_NEW(DownloadInfo, info);
	info->mURL = StringUtility::strReplaceAll(fullURL, " ", "%20");
	info->mFileName = fileName;
	info->mUploadPath = uploadPath;
	info->mProgressCallback = progress;
	info->mDownloadingCallback = downloading;
	info->mStartCallback = start;
	info->mFinishCallback = finish;
	info->mUserData = userData;
	mUploadList.push_back(info);
	UNLOCK(mUploadListLock);
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
			// 文件太大时会获取失败,太耗时,所以取消在下载前获取文件大小
			//int length = (int)downloadManager->getDownloadFileLenth(info->mURL.c_str());
			int length = -1;
			info->mStartCallback(info->mUserData, info->mFileName, length);
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
				mEditorCore->sendDelayEvent(CET_STOP_PROGRAM);
				downloadManager->mRunning = false;
			}
			else
			{
				EDITOR_CORE_INFO_DELAY("网络异常,错误码:%d", retCode);
				//EDITOR_CORE_INFO_DELAY("下载失败 : %d, %s", retCode, strError.c_str());
				mEditorCore->sendDelayEvent(CET_STOP_PROGRAM);
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

int LibCurlDownloadManager::debug_callback(CURL *handle, curl_infotype type, char *data, size_t size, void *userptr)
{
	std::string str = data;
	str += "\n";
	OutputDebugStringA(str.c_str());
	return 0;
}

DWORD WINAPI LibCurlDownloadManager::uploadingThread(LPVOID lpParameter)
{
	LibCurlDownloadManager* downloadManager = (LibCurlDownloadManager*)(lpParameter);
	downloadManager->mUploadFinish = false;
	while (downloadManager->mRunning)
	{
		LOCK(downloadManager->mUploadListLock);
		if (downloadManager->mUploadList.size() == 0)
		{
			UNLOCK(downloadManager->mUploadListLock);
			continue;
		}

		DownloadInfo* info = downloadManager->mUploadList[0];
		downloadManager->mUploadList.erase(downloadManager->mUploadList.begin());
		UNLOCK(downloadManager->mUploadListLock);
		if (info->mStartCallback != NULL)
		{
			info->mStartCallback(info->mUserData, info->mFileName, 0);
		}
		curl_httppost* formpost = NULL;
		curl_httppost* lastptr = NULL;
		// 上传文件的路径字段
		// curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "path", CURLFORM_FILE, info->mUploadPath.c_str(), CURLFORM_END);
		// 填写文件上传字段。这使得libcurl从中加载数据当调用curl_easy_perform（）时给定的文件名。
		curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "sendfile", CURLFORM_FILE, info->mFileName.c_str(), CURLFORM_END);
		// 同样是文件上传字段
		curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "upfile", CURLFORM_FILE, info->mFileName.c_str(), CURLFORM_END);
		//
		// 文件名字段
		curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "filename", CURLFORM_END);
		// 提交字段 (即使这很少需要)
		curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "submit", CURLFORM_COPYCONTENTS, "send", CURLFORM_END);

		//------------------------------------------------------------------------------------------------------------
		CURL* curl = curl_easy_init();
		if (curl == NULL)
		{
			continue;
		}
		// 拼接带路径的上传地址
		std::string mNewUrl = info->mURL +"?path=["+ info->mUploadPath + "]";
		curl_easy_setopt(curl, CURLOPT_HEADER, 0);
		curl_easy_setopt(curl, CURLOPT_URL, mNewUrl.c_str());
		curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, 60);
		//curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, debug_callback);	// 暂时不要debug 因为在上传较大文件的时候 会影响速度
		// 开始执行perform下载文件
		CURLcode retCode = curl_easy_perform(curl);
		std::string strError = curl_easy_strerror(retCode);
		curl_easy_cleanup(curl);
		if (retCode != CURLE_OK)
		{
			if (retCode == CURLE_ABORTED_BY_CALLBACK)
			{
				// 取消上传
				downloadManager->mRunning = false;
			}
			else if (retCode == CURLE_COULDNT_CONNECT)
			{
				EDITOR_CORE_INFO_DELAY("无法连接至服务器!");
				mEditorCore->sendDelayEvent(CET_STOP_PROGRAM);
				downloadManager->mRunning = false;
			}
			else
			{
				if (retCode == 23)
				{
					// 错误23 必出 Failed writing received data to disk/application 暂时忽略 不影响文件正常完整上传
				}
				else
				{
					EDITOR_CORE_INFO_DELAY("网络异常,错误码:%d", retCode);
					//EDITOR_CORE_INFO_DELAY("上传失败 : %d, %s", retCode, strError.c_str());
					mEditorCore->sendDelayEvent(CET_STOP_PROGRAM);
					downloadManager->mRunning = false;
				}
			}
		}
		//清理curl，和前面的初始化匹配  
		if (info->mFinishCallback != NULL)
		{
			info->mFinishCallback(info->mUserData, info->mFileName, retCode == CURLE_OK);
		}
		curl_formfree(formpost);
		TRACE_DELETE(info);
	}
	downloadManager->mUploadFinish = true;
	return 0;
}