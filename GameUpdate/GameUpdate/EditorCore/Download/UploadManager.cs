using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

public class FileUploadInfo
{
	public string mFileName;
	public string mUploadPath;
}

public class UploadFile
{
	public FileStream mFileHandle;
	public bool mUploaded;
};

public class UploadManager : FrameComponent
{
	protected Dictionary<string, UploadFile> mUploadingFileList;     //正在上传或已经完毕的更新文件
	protected UploadFile mCurUploading;
	public UploadManager(string name)
		:base(name)
	{
		mUploadingFileList = new Dictionary<string, UploadFile>();
		mCurUploading = null;
	}
	public override void init() { }
	public override void destroy()
	{
		mUploadingFileList.Clear();
		base.destroy();
	}
	public override void update(float elapsedTime) { }
	public void uploadFile(List<FileUploadInfo> fileList)
	{
		int count = fileList.Count();
		for (int i = 0; i < count; ++i)
		{
			UploadFile file = new UploadFile();
			file.mFileHandle = null;
			file.mUploaded = false;
			mUploadingFileList.Add(fileList[i].mFileName, file);
			mHttpDownloadManager.upload(CommonDefine.UPLOAD_URL, fileList[i].mFileName, fileList[i].mUploadPath, onStart, onFinish);
		}
	}
	public UploadFile getCurUploadingFile() { return mCurUploading; }
	public void onStart(string fileName, long totalSize)
	{
		//FileStream pFile = new FileStream(fileName, FileMode.Open);
		mCurUploading = mUploadingFileList[fileName];
		// 发送延迟事件通知界面
		mEditorCore.sendDelayEvent(CORE_EVENT_TYPE.CET_UPLOADING_FILE, fileName);
	}
	public void onFinish(string fileName, bool uploadSuccess)
	{
		//mCurUploading.mFileHandle.Close(); /* close the local file */
		mCurUploading.mFileHandle = null;
		mCurUploading.mUploaded = true;
		// 发送延迟事件通知界面
		List<string> param = new List<string>();
		param.Add(fileName);
		param.Add(StringUtility.boolToString(uploadSuccess));
		mEditorCore.sendDelayEvent(CORE_EVENT_TYPE.CET_FINISH_UPLOAD, param);

		// 如果全部都上传完了,则发送事件通知界面
		bool allUploaded = true;
		foreach(var item in mUploadingFileList)
		{
			if (!item.Value.mUploaded)
			{
				allUploaded = false;
				break;
			}
		}
		if (allUploaded)
		{
			mEditorCore.sendDelayEvent(CORE_EVENT_TYPE.CET_ALL_UPLOADED);
		}
	}
}