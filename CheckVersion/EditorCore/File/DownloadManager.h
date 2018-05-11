#ifndef _DOWNLOAD_MANAGER_H_
#define _DOWNLOAD_MANAGER_H_

#include "EditorCoreBase.h"
#include "CommonDefine.h"
#include "txCommandReceiver.h"

const std::string VERSION = "Version";
const std::string FILELIST = "FileList";

class FileInfo
{
public:
	std::string mFileName;
	std::string mMD5;
	int mFileSize;
};

enum UPGRADE_STATE
{
	US_NONE,
	US_DOWNLOADING_VERSION,
	US_DOWNLOADING_FILE_LIST,
	US_PARSING_REMOTE_FILE_LIST,
	US_GENERATE_LOCAL_FILE,
	US_GENERATE_MODIFIED_FILE,
	US_DONE,
};

class File;
class DownloadManager : public EditorCoreBase, public txCommandReceiver
{
public :
	DownloadManager()
		:
	txCommandReceiver("DownloadManager")
	{
		mCurDownloading = NULL;
		mState = US_NONE;
		mIgnorePathList.insert("dndl4.4_Data/StreamingAssets/CustomSound/RaceSound");
	}
	virtual ~DownloadManager()
	{
		destroy();
	}
	void init();
	void destroy();
	void update(float elapsedTime){}
	void requestDownloadVersion();
	void initDownloadList(const std::string& remote);
	File* getCurDownloadingFile() { return mCurDownloading; }
	int getTotalCount() { return mModifiedList.size(); }
	static int downloading(void* pBuffer, size_t nSize, size_t nMemByte, void* pParam);
	static int progressing(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow);
	static void onStart(void* userData, const std::string& fileName, const int& fileLength);
	static void onFinish(void* userData, const std::string& fileName, const bool& downloadSuccess);
protected:
	void notifyVersionDownloaded(File* versionFile);
	void notifyFileListDownloaded(File* listFile);
	void parseRemoteFileList(const std::string& listContent);
	void getLocalFileList();
	void repuestDownload(const std::string& fileName);
	static void removeElement(txMap<std::string, FileInfo*>& list, const std::string& element);
	static txMap<std::string, FileInfo*> generateModifiedFile(txMap<std::string, FileInfo*>& remoteList, txMap<std::string, FileInfo*>& localList, txSet<std::string>& ignorePathList);
	static bool isIgnoreFile(const std::string& file, txSet<std::string>& ignorePathList);
protected:
	txMap<std::string, File*> mDownloadingFileList;		// 正在下载或已经下载完毕的更新文件
	txMap<std::string, FileInfo*> mRemoteFileList;		// 远端文件列表
	txMap<std::string, FileInfo*> mLocalFileList;		// 本地文件列表
	txMap<std::string, FileInfo*> mModifiedList;		// 需要更新的文件列表
	txSet<std::string> mIgnorePathList;
	std::string mRemoteVersion;
	std::string mLocalVersion;
	std::string mDataPath;
	File* mCurDownloading;
	volatile std::atomic<UPGRADE_STATE> mState;
};

#endif