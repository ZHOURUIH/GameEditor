#include "DownloadManager.h"
#include "File.h"
#include "Utility.h"
#include "EditorCore.h"
#include "LibCurlDownloadManager.h"
#include "CommandHeader.h"

#define CLEAR_MAP(list)\
{\
	auto iter = list.begin(); \
	auto iterEnd = list.end(); \
	for (; iter != iterEnd; ++iter)\
	{\
		TRACE_DELETE(iter->second); \
	}\
	list.clear();\
}

#define REMOVE_ELEMENT(list, element)\
{\
	auto iter = list.find(element); \
	if (iter != list.end())\
	{\
		list.erase(iter);\
	}\
}\

void DownloadManager::destroy()
{
	// 一定要将取消设置为true,否则可能无法关闭窗口,确保子线程
	mCancel = true;
	while (mState != US_DONE && mState != US_NONE){}
	CLEAR_MAP(mDownloadingFileList);
	CLEAR_MAP(mRemoteFileList);
	CLEAR_MAP(mLocalFileList);
}

void DownloadManager::init()
{
	;
}

void DownloadManager::update(float elapsedTime)
{
	if (mCurTimeCount >= 0.0f)
	{
		mCurTimeCount = mCurTimeCount + elapsedTime;
		if (mCurTimeCount >= 1.0f)
		{
			mCurTimeCount = 0.0f;
			mCurSpeed = mSpeedInSecond;
			mSpeedInSecond = 0.0f;
			if (!MathUtility::isFloatZero(mCurSpeed))
			{
				mRemainTime = (mTotalSize - mDownloadedSize) / mCurSpeed;
			}
			else
			{
				mRemainTime = 0.0f;
			}
		}
	}
	if (mLastDownloadingTime >= 0.0f)
	{
		mLastDownloadingTime = mLastDownloadingTime + elapsedTime;
		if (mLastDownloadingTime >= mDownloadTimeOut)
		{
			EDITOR_CORE_ERROR("网络异常!");
			mLastDownloadingTime = 0.0f;
		}
	}
}

void DownloadManager::requestDownloadVersion()
{
	mState = US_DOWNLOADING_VERSION;
	mEditorCore->sendDelayEvent(CET_START_DOWNLOAD_VERSION);
	// 开始下载版本文件
	repuestDownload(VERSION);
}

int DownloadManager::downloading(void* pBuffer, size_t nSize, size_t nMemByte, void* pParam)
{
	DownloadManager* manager = (DownloadManager*)pParam;
	size_t writeSize = nSize * nMemByte;
	manager->mCurDownloading->writeFile((char*)pBuffer, writeSize);
	manager->mSpeedInSecond = manager->mSpeedInSecond + writeSize;
	if (manager->mCurDownloading->getFileName() != VERSION && manager->mCurDownloading->getFileName() != FILELIST)
	{
		manager->mDownloadedSize = manager->mDownloadedSize + writeSize;
	}
	manager->mLastDownloadingTime = 0.0f;
	return writeSize;
}

int DownloadManager::progressing(void* clientp, double dltotal, double dlnow, double ultotal, double ulnow)
{
	DownloadManager* manager = (DownloadManager*)clientp;
	manager->mCurDownloading->setCurSize((int)dlnow);
	// 如果取消更新,则需要返回非0,中断下载
	if (manager->mCancel)
	{
		CommandDownloadManagerCancel* cmd = Command::createDelayCommand<CommandDownloadManagerCancel>(COMMAND_PARAM);
		mCommandSystem->pushDelayCommand(cmd, manager);
		return -1;
	}
	else
	{
		return 0;
	}
}

void DownloadManager::onStart(void* userData, const std::string& fileName, const int& fileLength)
{
	DownloadManager* manager = (DownloadManager*)userData;
	auto iter = manager->mDownloadingFileList.find(fileName);
	if (iter == manager->mDownloadingFileList.end())
	{
		return;
	}
	manager->mCurDownloading = iter->second;
	manager->mCurDownloading->startWrite(fileName);
	if (fileLength != -1)
	{
		manager->mCurDownloading->setTotalSize(fileLength);
	}
	else
	{
		if (manager->mRemoteFileList.find(fileName) != manager->mRemoteFileList.end())
		{
			manager->mCurDownloading->setTotalSize(manager->mRemoteFileList[fileName]->mFileSize);
		}
	}
	if (manager->mCurDownloading->getFileName() != VERSION && manager->mCurDownloading->getFileName() != FILELIST)
	{
		mEditorCore->sendDelayEvent(CET_START_DOWNLOAD, manager->mCurDownloading->getFileName());
	}
}

void DownloadManager::onFinish(void* userData, const std::string& fileName, const bool& downloadSuccess)
{
	DownloadManager* manager = (DownloadManager*)userData;
	auto iter = manager->mDownloadingFileList.find(fileName);
	if (iter == manager->mDownloadingFileList.end())
	{
		return;
	}
	// 如果没有下载成功,则不再继续执行后续更新逻辑
	if (!downloadSuccess)
	{
		manager->mState = US_NONE;
		return;
	}
	File* file = iter->second;
	if (fileName == VERSION)
	{
		manager->notifyVersionDownloaded(file);
	}
	else if (fileName == FILELIST)
	{
		manager->notifyFileListDownloaded(file);
	}
	else
	{
		std::string md5;
		auto iterRemote = manager->mRemoteFileList.find(file->getFileName());
		if (iterRemote != manager->mRemoteFileList.end())
		{
			md5 = iterRemote->second->mMD5;
		}
		// 如果失败,则不再继续更新
		if (!file->finishWrite(md5))
		{
			EDITOR_CORE_INFO_DELAY("文件下载失败,该文件可能被占用或文件内容错误 : %s", fileName.c_str());
			manager->mState = US_NONE;
		}
		else
		{
			// 加入下载完成的列表
			manager->mDownloadedList.insert(fileName, file);
			mEditorCore->sendDelayEvent(CET_FINISH_DOWNLOAD, file->getFileName());
			// 全部文件都下载完毕,更新所有文件,版本更新完毕
			if (manager->mDownloadedList.size() == manager->mModifiedList.size())
			{
				manager->notifyAllDownloaded();
				// 更新完成
				mEditorCore->sendDelayEvent(CET_UPDATE_DONE);
				// 更新版本号
				manager->done();
			}
		}
	}
}

void DownloadManager::setCancel(const bool& cancel)
{
	mCancel = cancel;
	// 如果没有正在下载文件,则直接发送命令退出
	if (mCancel && (mState == US_DONE || mState == US_NONE))
	{
		CommandDownloadManagerCancel* cmd = Command::createDelayCommand<CommandDownloadManagerCancel>(COMMAND_PARAM);
		mCommandSystem->pushDelayCommand(cmd, this);
	}
}

void DownloadManager::startUpdateGame(const bool& start)
{
	if (start)
	{
		mState = US_DOWNLOADING_FILE_LIST;
		mEditorCore->sendDelayEvent(CET_START_DOWNLOAD_LIST_FILE);
		// 首先从远端下载列表文件
		repuestDownload(FILELIST);
	}
	else
	{
		mState = US_NONE;
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------------
void DownloadManager::notifyVersionDownloaded(File* versionFile)
{
	// 版本文件下载完毕后,提示是否有新版本可以更新
	int length = 0;
	char* buffer = NULL;
	bool hasNewVersion = false;
	if (!FileUtility::openFile(VERSION, length, buffer, true))
	{
		hasNewVersion = true;
	}
	else
	{
		std::string remoteVersion = StringUtility::bufferToString(versionFile->getFileData(), versionFile->getCurSize());
		hasNewVersion = remoteVersion != std::string(buffer);
	}
	mEditorCore->sendDelayEvent(CET_NEW_VERSION, StringUtility::boolToString(hasNewVersion));
	if (hasNewVersion)
	{
		mState = US_WAIT_FOR_UPGRADE;
	}
	else
	{
		done();
	}
}

void DownloadManager::notifyFileListDownloaded(File* listFile)
{
	// 列表文件下载完毕,解析列表
	mState = US_PARSING_REMOTE_FILE_LIST;
	std::string listContent = StringUtility::bufferToString(listFile->getFileData(), listFile->getCurSize());
	parseRemoteFileList(listContent);
	mState = US_GENERATE_LOCAL_FILE;
	// 检查本地文件
	getLocalFileList();
	// 如果在检查本地文件过程中取消了,则直接返回
	if (mCancel)
	{
		mState = US_NONE;
		return;
	}

	// 判断出需要更新的文件
	mState = US_GENERATE_MODIFIED_FILE;
	// 然后下载修改过和新增的文件
	mModifiedList = generateModifiedFile(mRemoteFileList, mLocalFileList, mIgnorePathList);
	mState = US_DOWNLOADING_REMOTE_FILE;
	if (mModifiedList.size() == 0)
	{
		mEditorCore->sendDelayEvent(CET_NOTHING_UPDATE);
		done();
	}
	else
	{
		// 计算所有需要下载的文件的大小
		mTotalSize = 0.0f;
		auto iterModified = mModifiedList.begin();
		auto iterModifiedEnd = mModifiedList.end();
		for (; iterModified != iterModifiedEnd; ++iterModified)
		{
			mTotalSize = mTotalSize + iterModified->second->mFileSize;
		}
		// 开始下载所有文件,并且开始计时
		mCurTimeCount = 0.0f;
		iterModified = mModifiedList.begin();
		for (; iterModified != iterModifiedEnd; ++iterModified)
		{
			repuestDownload(iterModified->first);
		}
	}
}

void DownloadManager::notifyAllDownloaded()
{
	mEditorCore->sendDelayEvent(CET_UPDATING_FILE);
	mState = US_UPDATE_LOCAL_FILE;
	// 首先删除本地残存的无用文件,也就是本地有但是服务器没有的
	auto uselessFile = generateUselessFile(mRemoteFileList, mLocalFileList, mIgnorePathList);
	int uselessFileCount = uselessFile.size();
	int i = 0;
	auto iterUseless = uselessFile.begin();
	auto iterUselessEnd = uselessFile.end();
	for (; iterUseless != iterUselessEnd; ++iterUseless)
	{
		++i;
		FileUtility::deleteFile(iterUseless->first);
		// 删除文件占总进度的10%
		mEditorCore->sendDelayEvent(CET_UPDATING_PROGRESS, StringUtility::floatToString(i / (float)uselessFileCount * 0.1f, 3));
	}
	// 清除空文件夹
	FileUtility::deleteEmptyFolder("./");

	// 将下载的文件替换掉本地文件
	int modifyFileCount = mModifiedList.size();
	int j = 0;
	auto iterModify = mModifiedList.begin();
	auto iterModifyEnd = mModifiedList.end();
	FOR_STL(mModifiedList, ; iterModify != iterModifyEnd; ++iterModify)
	{
		++j;
		std::string sourceFile = iterModify->first;
		std::string destFile = sourceFile.substr(TEMP_PATH.length(), sourceFile.length() - TEMP_PATH.length());
		FileUtility::copyFile(sourceFile, destFile);
		// 替换文件占总进度的90%
		mEditorCore->sendDelayEvent(CET_UPDATING_PROGRESS, StringUtility::floatToString(j / (float)modifyFileCount * 0.9f + 0.1f, 3));
	}
	END_FOR_STL(mModifiedList);
	// 删除下载临时目录
	FileUtility::deleteFolder(TEMP_PATH);
}

void DownloadManager::parseRemoteFileList(const std::string& listContent)
{
	txVector<std::string> fileList;
	StringUtility::split(listContent, "\r\n", fileList);
	if (fileList.size() == 0)
	{
		EDITOR_CORE_ERROR_DELAY("列表文件错误!");
		return;
	}
	int readFileCount = StringUtility::stringToInt(fileList[0]);
	if (readFileCount == 0)
	{
		EDITOR_CORE_ERROR_DELAY("文件数量读取错误!");
		return;
	}
	fileList.erase(fileList.begin());
	int fileCount = fileList.size();
	if (fileCount != readFileCount)
	{
		EDITOR_CORE_ERROR_DELAY("文件数量不匹配, 文件头记录数量 : %d, 实际数量 : %d", readFileCount, fileCount);
		return;
	}
	// 生成远端文件列表
	for (int i = 0; i < fileCount; ++i)
	{
		txVector<std::string> contentList;
		StringUtility::split(fileList[i], "\t", contentList, false);
		if (contentList.size() != 3)
		{
			EDITOR_CORE_ERROR_DELAY("远端文件列表已损坏!行号 : ", i + 1);
			continue;
		}
		FileInfo* info = TRACE_NEW(FileInfo, info);
		info->mFileName = StringUtility::strReplaceAll(contentList[0], "%20", " ");
		info->mFileSize = StringUtility::stringToInt(contentList[1]);
		info->mMD5 = contentList[2];
		info->mBigFile = info->mFileSize > BIG_FILE_SIZE;
		mRemoteFileList.insert(info->mFileName, info);
	}
}

void DownloadManager::getLocalFileList()
{
	if (mLocalFileList.size() > 0)
	{
		EDITOR_CORE_ERROR_DELAY("local list is not empty!");
		return;
	}
	txVector<std::string> fileList;
	FileUtility::findFiles("./", fileList);
	int fileCount = fileList.size();
	for (int i = 0; i < fileCount; ++i)
	{
		FileInfo* info = TRACE_NEW(FileInfo, info);
		std::string newPath = fileList[i].substr(strlen("./"), fileList[i].length() - strlen("./"));
		mLocalFileList.insert(newPath, info);
	}
	// 排除版本文件,列表文件,程序自身
	char thisFileName[256];
	GetModuleFileNameA(NULL, thisFileName, sizeof(thisFileName));
	std::string strThisFile = StringUtility::getFileName(thisFileName);
	REMOVE_ELEMENT(mLocalFileList, strThisFile);
	REMOVE_ELEMENT(mLocalFileList, VERSION);
	REMOVE_ELEMENT(mLocalFileList, FILELIST);

	mEditorCore->sendDelayEvent(CET_START_GENERATE_LOCAL_FILE, StringUtility::intToString(mLocalFileList.size()));

	// 计算文件信息
	auto iter = mLocalFileList.begin();
	auto iterEnd = mLocalFileList.end();
	for (int i = 0; iter != iterEnd && !mCancel; ++iter, ++i)
	{
		iter->second->mFileName = iter->first;
		iter->second->mFileSize = FileUtility::getFileSize(iter->first);
		iter->second->mBigFile = iter->second->mFileSize > BIG_FILE_SIZE;
		// 如果是大文件,则不计算MD5
		if (!iter->second->mBigFile)
		{
			iter->second->mMD5 = FileUtility::generateFileMD5(iter->first);
		}
		txVector<std::string> params;
		params.push_back(StringUtility::intToString(mLocalFileList.size()));
		params.push_back(StringUtility::intToString(i + 1));
		mEditorCore->sendDelayEvent(CET_GENERATING_LOCAL_FILE, params);
	}
	mEditorCore->sendDelayEvent(CET_FINISH_GENERATE_LOCAL_FILE);
}

void DownloadManager::repuestDownload(const std::string& fileName)
{
	File* file = TRACE_NEW(File, file);
	mDownloadingFileList.insert(fileName, file);
	mLibCurlDownloadManager->download(REMOTE_URL + fileName, fileName, progressing, downloading, onStart, onFinish, this);
}

void DownloadManager::done()
{
	// 更新版本号
	if (mDownloadingFileList.find(VERSION) != mDownloadingFileList.end())
	{
		mDownloadingFileList[VERSION]->finishWrite("");
	}
	mState = US_DONE;
	mCurTimeCount = -1.0f;
	mCurSpeed = 0.0f;
	mRemainTime = 0.0f;
}

txMap<std::string, FileInfo*> DownloadManager::generateUselessFile(txMap<std::string, FileInfo*>& remoteList, txMap<std::string, FileInfo*>& localList, txSet<std::string>& ignorePathList)
{
	txMap<std::string, FileInfo*> uselessList;
	// 本地有但是远端没有的文件
	auto iterLocal = localList.begin();
	auto iterLocalEnd = localList.end();
	for (; iterLocal != iterLocalEnd; ++iterLocal)
	{
		if (isIgnoreFile(iterLocal->first, ignorePathList))
		{
			continue;
		}
		auto iterRemote = remoteList.find(iterLocal->first);
		if (iterRemote == remoteList.end())
		{
			uselessList.insert(iterLocal->first, iterLocal->second);
		}
	}
	return uselessList;
}

txMap<std::string, FileInfo*> DownloadManager::generateModifiedFile(txMap<std::string, FileInfo*>& remoteList, txMap<std::string, FileInfo*>& localList, txSet<std::string>& ignorePathList)
{
	txMap<std::string, FileInfo*> modifiedList;
	auto iterRemote = remoteList.begin();
	auto iterRemoteEnd = remoteList.end();
	for (; iterRemote != iterRemoteEnd; ++iterRemote)
	{
		if (isIgnoreFile(iterRemote->first, ignorePathList))
		{
			continue;
		}
		bool isModify = false;
		auto iterLocal = localList.find(iterRemote->first);
		// 远端有本地也有的
		if (iterLocal != localList.end())
		{
			// 文件大小不一致,或者文件大小一致,但是md5码不一致,如果是大文件,大小一致时就可以认为该文件没有修改
			if (iterRemote->second->mFileSize != iterLocal->second->mFileSize || 
				(!iterRemote->second->mBigFile && iterRemote->second->mMD5 != iterLocal->second->mMD5))
			{
				isModify = true;
			}
		}
		// 本地没有的
		else
		{
			isModify = true;	
		}
		if (isModify)
		{
			// 下载到临时目录TEMP_PATH
			modifiedList.insert(TEMP_PATH + iterRemote->first, iterRemote->second);
		}
	}
	return modifiedList;
}

bool DownloadManager::isIgnoreFile(const std::string& file, txSet<std::string>& ignorePathList)
{
	auto iter = ignorePathList.begin();
	auto iterEnd = ignorePathList.end();
	for (; iter != iterEnd; ++iter)
	{
		if (StringUtility::findSubstr(file, *iter, false))
		{
			return true;
		}
	}
	return false;
}