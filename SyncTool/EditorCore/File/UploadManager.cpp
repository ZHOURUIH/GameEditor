#include "UploadManager.h"
#include "File.h"
#include "Utility.h"
#include "EditorCore.h"
#include "LibCurlDownloadManager.h"
#include "CommandHeader.h"

void UploadManager::destroy()
{
	auto iter = mUploadingFileList.begin();
	auto iterEnd = mUploadingFileList.end();
	FOR_STL(mUploadingFileList, ;  iter != iterEnd; ++iter)
	{
		TRACE_DELETE(iter->second);
	}
	END_FOR_STL(mUploadingFileList)
}

void UploadManager::init()
{
	;
}

void UploadManager::uploadFile(const txVector<std::pair<std::string, std::string>>& fileList)
{
	int count = fileList.size();
	for (int i = 0; i < count; ++i)
	{
		UploadFile* file = TRACE_NEW(UploadFile, file);
		file->mFile = NULL;
		file->mFileHandle = NULL;
		file->mUploaded = false;
		mUploadingFileList.insert(fileList[i].first, file);
		mLibCurlDownloadManager->upload(UPLOAD_URL, fileList[i].first, fileList[i].second, NULL, NULL, onStart, onFinish, this);
	}
}

void UploadManager::onStart(void* userData, const std::string& fileName, const int& fileLength)
{
	UploadManager* manager = (UploadManager*)userData;
	FILE* pFile = NULL;
	fopen_s(&pFile, fileName.c_str(), "rb");
	manager->mCurUploading = manager->mUploadingFileList[fileName];
	manager->mCurUploading->mFileHandle = pFile;
	// 发送延迟事件通知界面
	mEditorCore->sendDelayEvent(CET_UPLOADING_FILE, fileName);
}

void UploadManager::onFinish(void* userData, const std::string& fileName, const bool& uploadSuccess)
{
	UploadManager* manager = (UploadManager*)userData;
	fclose(manager->mCurUploading->mFileHandle); /* close the local file */
	manager->mCurUploading->mFileHandle = NULL;
	manager->mCurUploading->mUploaded = true;
	// 发送延迟事件通知界面
	txVector<std::string> params;
	params.push_back(fileName);
	params.push_back(StringUtility::boolToString(uploadSuccess));
	mEditorCore->sendDelayEvent(CET_FINISH_UPLOAD, params);

	// 如果全部都上传完了,则发送事件通知界面
	bool allUploaded = true;
	auto iter = manager->mUploadingFileList.begin();
	auto iterEnd = manager->mUploadingFileList.end();
	for (; iter != iterEnd; ++iter)
	{
		if (!iter->second->mUploaded)
		{
			allUploaded = false;
			break;
		}
	}
	if (allUploaded)
	{
		mEditorCore->sendDelayEvent(CET_ALL_UPLOADED);
	}
}