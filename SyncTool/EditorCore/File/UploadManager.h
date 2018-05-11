#ifndef _UPLOAD_MANAGER_H_
#define _UPLOAD_MANAGER_H_

#include "EditorCoreBase.h"
#include "CommonDefine.h"
#include "CommandReceiver.h"

class File;
class UploadFile
{
public:
	FILE* mFileHandle;
	File* mFile;
	bool mUploaded;
};

class UploadManager : public EditorCoreBase, public CommandReceiver
{
public :
	UploadManager()
		:
	CommandReceiver("UploadManager")
	{
		mCurUploading = NULL;
	}
	virtual ~UploadManager()
	{
		destroy();
	}
	void init();
	void destroy();
	void update(float elapsedTime){}
	void uploadFile(const txVector<std::pair<std::string, std::string>>& fileList);
	UploadFile* getCurUploadingFile() { return mCurUploading; }
	static void onStart(void* userData, const std::string& fileName, const int& fileLength);
	static void onFinish(void* userData, const std::string& fileName, const bool& uploadSuccess);
protected:
	txMap<std::string, UploadFile*> mUploadingFileList;		//正在上传或已经完毕的更新文件
	UploadFile* mCurUploading;
};

#endif