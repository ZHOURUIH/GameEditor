using System;
using System.Collections.Generic;
using System.Linq;
using System.Diagnostics;

public class DownloadManager : FrameComponent
{
	protected Dictionary<string, FileWrap> mDownloadingFileList; // 正在下载或已经下载完毕的更新文件
	protected Dictionary<string, FileWrap> mDownloadedList;      // 已经下载完毕的更新文件
	protected Dictionary<string, GameFileInfo> mRemoteFileList;  // 远端文件列表,Key为远端文件位置
	protected Dictionary<string, GameFileInfo> mLocalFileList;   // 本地文件列表
	protected Dictionary<string, GameFileInfo> mModifiedList;    // 需要更新的文件列表
	protected FileWrap mCurDownloading;
	protected string mRemoteVersion;
	protected string mLocalVersion;
	protected string mDataPath;
	protected float mCurTimeCount;						// 当前计时,小于0表示还没有开始计时,大于等于0表示正在计时
	protected float mRemainTime;                        // 剩余下载时间
	protected float mDownloadTimeOut;
	protected float mLastDownloadingTime;				// 上次接收到下载数据到当前的时间,小于0表示未计时,大于等于0表示正在计时,在第一次接收到下载数据时开始计时
	protected long mDownloadedSize;						// 已经下载的大小,但是不包括当前文件的已下载字节数
	protected long mSpeedInSecond;
	protected long mTotalSize;							// 当前需要下载的文件的总大小
	protected long mCurSpeed;
	protected bool mCancel;
	protected UPGRADE_STATE mState;
	public DownloadManager(string name)
		: base(name)
	{
		mDownloadingFileList = new Dictionary<string, FileWrap>();
		mDownloadedList = new Dictionary<string, FileWrap>();
		mRemoteFileList = new Dictionary<string, GameFileInfo>();
		mLocalFileList = new Dictionary<string, GameFileInfo>();
		mModifiedList = new Dictionary<string, GameFileInfo>();
		mCurTimeCount = -1.0f;
		mDownloadTimeOut = 120.0f;
		mLastDownloadingTime = -1.0f;
	}
	public override void init(){}
	public override void destroy()
	{
		base.destroy();
		mDownloadingFileList.Clear();
		mRemoteFileList.Clear();
		mLocalFileList.Clear();
	}
	public override void update(float elapsedTime)
	{
		if (mCurTimeCount >= 0.0f)
		{
			mCurTimeCount = mCurTimeCount + elapsedTime;
			if (mCurTimeCount >= 1.0f)
			{
				mCurTimeCount = 0.0f;
				mCurSpeed = mSpeedInSecond;
				mSpeedInSecond = 0;
				if (!isFloatZero(mCurSpeed))
				{
					mRemainTime = (mTotalSize - getDownloadedSize()) / mCurSpeed;
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
				logError("网络异常!");
				mState = UPGRADE_STATE.NONE;
				setCancel(true, false);
				mLastDownloadingTime = 0.0f;
			}
		}
	}
	public long getDownloadedSize() { return mDownloadedSize + mCurDownloading.getCurSize(); }
	public FileWrap getCurDownloadingFile() { return mCurDownloading; }
	public int getTotalCount() { return mModifiedList.Count(); }
	public int getDownloadedCount() { return mDownloadedList.Count(); }
	public long getCurSpeed() { return mCurSpeed; }
	public float getRemainTime() { return mRemainTime; }
	public long getTotalSize() { return mTotalSize; }
	public bool isDownloading() { return mState == UPGRADE_STATE.DOWNLOADING_REMOTE_FILE; }
	public void setCancel(bool cancel, bool delay = true)
	{
		if (mCancel == cancel)
		{
			return;
		}
		mCancel = cancel;
		if (mCancel)
		{
			if (delay)
			{
				pushDelayCommand<CommandDownloadManagerCancel>(this);
			}
			else
			{
				pushCommand<CommandDownloadManagerCancel>(this);
			}
		}
	}
	public void startUpdateGame()
	{
		setState(UPGRADE_STATE.DOWNLOADING_FILE_LIST);
		mEditorCore.sendDelayEvent(CORE_EVENT_TYPE.START_DOWNLOAD_LIST_FILE);
		// 从远端获得Windows启动器文件列表
		setState(UPGRADE_STATE.PARSING_REMOTE_FILE_LIST);
		mRemoteFileList = GameUtility.getFileList(GameDefine.REMOTE_FOLDER, true);

		// 列表文件下载完毕,解析列表
		setState(UPGRADE_STATE.GENERATE_LOCAL_FILE);
		// 检查本地文件
		generateLocalFileList();
		// 如果在检查本地文件过程中取消了,则直接返回
		if (mCancel)
		{
			setState(UPGRADE_STATE.NONE);
			return;
		}

		// 判断出需要更新的文件
		setState(UPGRADE_STATE.GENERATE_MODIFIED_FILE);
		// 然后下载修改过和新增的文件
		mModifiedList = generateModifiedFile(mRemoteFileList, mLocalFileList);
		setState(UPGRADE_STATE.DOWNLOADING_REMOTE_FILE);

		if (mModifiedList.Count() == 0)
		{
			// 如果有临时目录，则将临时目录中的所有文件替换到游戏目录
			notifyAllDownloaded();
			mEditorCore.sendDelayEvent(CORE_EVENT_TYPE.NOTHING_UPDATE);
			done();
			return;
		}

		// 计算所有需要下载的文件的大小
		mTotalSize = 0;
		foreach (var item in mModifiedList)
		{
			mTotalSize += item.Value.mFileSize;
		}
		// 开始下载所有文件,并且开始计时
		mCurTimeCount = 0.0f;
		foreach (var item in mModifiedList)
		{
			// 如果临时文件存在,表示之前没下载成功,暂时删除临时文件
			// 如果实现断点续传时需要获取临时文件的大小
			long offset = 0;
			string fullTemp = GameDefine.TEMP_PATH + item.Key + GameDefine.TEMP_FILE_EXTENSION;
			if (isFileExist(fullTemp))
			{
				offset = getFileSize(fullTemp);
			}
			repuestDownload(item.Key, offset);
		}
	}
	public void setState(UPGRADE_STATE state) { mState = state; }
	//----------------------------------------------------------------------------------------------------------------------------------------------------
	// 返回值表示是否继续下载
	protected bool downloading(byte[] pBuffer, int nSize)
	{
		if (!mCurDownloading.writeFile(pBuffer, nSize))
		{
			logError("文件写入失败! 文件 : " + getFileName(mCurDownloading.getFileName()), true);
			return false;
		}
		mSpeedInSecond += nSize;
		mLastDownloadingTime = 0.0f;
		if (mCurDownloading.getFileName() == GameDefine.FILE_LIST)
		{
			mEditorCore.sendDelayEvent(CORE_EVENT_TYPE.DOWNLOADING_LIST_FILE, FToS((float)mCurDownloading.getCurSize() / mCurDownloading.getTotalSize() * 100.0f, 2));
		}
		// 如果取消更新,则需要返回非0,中断下载
		if (mCancel)
		{
			pushDelayCommand<CommandDownloadManagerCancel>(this);
		}
		return !mCancel;
	}
	protected long onTimeout()
	{
		return mCurDownloading.getCurSize();
	}
	// 有文件开始下载
	protected void onStart(string fileName, long totalSize)
	{
		if (!mDownloadingFileList.ContainsKey(fileName))
		{
			return;
		}

		mCurDownloading = mDownloadingFileList[fileName];
		mCurDownloading.setTotalSize(totalSize);
		// 其他文件在接收到数据时就写入文件,并且先存放到临时目录中
		if (fileName != GameDefine.FILE_LIST)
		{
			mCurDownloading.startWrite(fileName, GameDefine.TEMP_PATH + fileName);
			mCurDownloading.setWriteMode(WRITE_MODE.AUTO_WRITE);
			mEditorCore.sendDelayEvent(CORE_EVENT_TYPE.START_DOWNLOAD, fileName);
		}
		else
		{
			mCurDownloading.startWrite(fileName);
			// 列表文件不写入文件
			if (fileName == GameDefine.FILE_LIST)
			{
				mCurDownloading.setWriteMode(WRITE_MODE.DONT_WRITE);
			}
		}
	}
	// 有文件下载结束
	protected void onFinish(string fileName, bool downloadSuccess)
	{
		if (!mDownloadingFileList.TryGetValue(fileName, out FileWrap file))
		{
			return;
		}
		// 如果没有下载成功,则不再继续执行后续更新逻辑
		if (!downloadSuccess)
		{
			setState(UPGRADE_STATE.NONE);
			return;
		}

		mLastDownloadingTime = -1.0f;
		// 如果失败,则不再继续更新
		WRITE_RESULT ret = file.finishWrite(null);
		if (ret == WRITE_RESULT.SUCCESS)
		{
			mDownloadedSize += file.getTotalSize();
			// 加入下载完成的列表
			mDownloadedList.Add(fileName, file);
			mEditorCore.sendDelayEvent(CORE_EVENT_TYPE.FINISH_DOWNLOAD, file.getFileName());
			// 全部文件都下载完毕,则版本更新完毕
			if (mDownloadedList.Count() == mModifiedList.Count())
			{
				notifyAllDownloaded();
				// 更新完成
				mEditorCore.sendDelayEvent(CORE_EVENT_TYPE.UPDATE_DONE);
				// 更新版本号
				done();
			}
		}
		else
		{
			if (ret == WRITE_RESULT.MD5_ERROR)
			{
				logError("文件下载失败," + fileName + "文件校验失败, 可能是上次未更新完成且长时间没有更新,请重新更新", true);
			}
			else if (ret == WRITE_RESULT.RENAME_ERROR)
			{
				logError("文件下载失败,文件重命名失败,文件可能被占用 : " + fileName, true);
			}
			else if (ret == WRITE_RESULT.WRITE_ERROR)
			{
				logError("文件下载失败,文件写入失败,文件可能被占用 : " + fileName, true);
			}
			setState(UPGRADE_STATE.NONE);
			pushDelayCommand<CommandDownloadManagerCancel>(mDownloadManager);
		}
	}
	// 所有文件都已经下载完毕,更新所有文件
	protected void notifyAllDownloaded()
	{
		mEditorCore.sendDelayEvent(CORE_EVENT_TYPE.UPDATING_FILE);
		setState(UPGRADE_STATE.UPDATE_LOCAL_FILE);
		// 首先删除本地残存的无用文件,也就是本地有但是服务器没有的
		var uselessFile = generateUselessFile(mRemoteFileList, mLocalFileList);
		int uselessFileCount = uselessFile.Count;
		int i = 0;
		foreach (var item in uselessFile)
		{
			deleteFile(item.Key);
			++i;
			// 删除文件占总进度的10%
			mEditorCore.sendDelayEvent(CORE_EVENT_TYPE.UPDATING_PROGRESS, FToS(i / (float)uselessFileCount * 0.1f));
		}
		// 清除空文件夹
		deleteEmptyFolder("./");
		List<string> temporaryFileList = new List<string>();
		findFiles(GameDefine.TEMP_PATH, temporaryFileList);
		// 将临时目录中的文件替换掉本地文件
		int temporaryFilecount = temporaryFileList.Count;
		for (int j = 0; j < temporaryFilecount; ++j)
		{
			string sourceFile = temporaryFileList[j];
			string destFile = sourceFile.Substring(GameDefine.TEMP_PATH.Length);
			copyFile(sourceFile, destFile);
			// 替换文件占总进度的90%
			mEditorCore.sendDelayEvent(CORE_EVENT_TYPE.UPDATING_PROGRESS, FToS(j / (float)temporaryFilecount * 0.9f + 0.1f));
		}
		// 删除下载临时目录
		deleteFolder(GameDefine.TEMP_PATH);
	}
	protected void done()
	{
		setState(UPGRADE_STATE.US_DONE);
		mCurTimeCount = -1.0f;
		mCurSpeed = 0;
		mRemainTime = 0.0f;
	}
	protected void generateLocalFileList()
	{
		if (mLocalFileList.Count() > 0)
		{
			logError("local list is not empty!", true);
			return;
		}

		// 排除列表文件,程序自身
		string strThisFile = getFileName(Process.GetCurrentProcess().MainModule.FileName);
		strThisFile = strThisFile.Substring(0, strThisFile.IndexOf("."));
		// 先查找游戏目录
		string startPath = "./";
		List<string> fileList = new List<string>();
		findFiles(startPath, fileList);
		int fileCount = fileList.Count;
		for (int i = 0; i < fileCount; ++i)
		{
			// 排除临时文件夹,排除StreamingAssets目录,排除所有以程序名开头的文件
			string fullPath = fileList[i];
			string fileName = fullPath.Substring(startPath.Length);
			if (!startWith(fullPath, startPath + GameDefine.TEMP_PATH) && 
				!fullPath.Contains("/StreamingAssets/") && 
				!startWith(fileName, strThisFile + "."))
			{
				var info = new GameFileInfo();
				info.mFileName = fileName;
				info.mFullPath = fullPath;
				info.mFileSize = (int)getFileSize(fullPath);
				mLocalFileList.Add(info.mFileName, info);
			}
		}

		// 最后再查找临时目录,因为临时目录中的文件需要覆盖其他目录的文件
		List<string> tempFile = new List<string>();
		findFiles(GameDefine.TEMP_PATH, tempFile);
		foreach (var item in tempFile)
		{
			string fileName = item.Substring(GameDefine.TEMP_PATH.Length);
			if (fileName.EndsWith(GameDefine.TEMP_FILE_EXTENSION))
			{
				fileName = fileName.Substring(0, fileName.Length - GameDefine.TEMP_FILE_EXTENSION.Length);
			}
			if (!mLocalFileList.TryGetValue(fileName, out GameFileInfo info))
			{
				info = new GameFileInfo();
				mLocalFileList.Add(fileName, info);
			}
			info.mFullPath = item;
			info.mFileName = fileName;
			info.mFileSize = (int)getFileSize(item);
		}

		mEditorCore.sendDelayEvent(CORE_EVENT_TYPE.FINISH_GENERATE_LOCAL_FILE);
	}
	protected void repuestDownload(string fileName, long offset = 0)
	{
		mDownloadingFileList.Add(fileName, new FileWrap());
		mHttpDownloadManager.download(GameDefine.DOWNLOAD_URL + fileName, fileName, offset, downloading, onTimeout, onStart, onFinish);
	}
	protected static Dictionary<string, GameFileInfo> generateUselessFile(Dictionary<string, GameFileInfo> remoteList, Dictionary<string, GameFileInfo> localList)
	{
		var uselessList = new Dictionary<string, GameFileInfo>();
		// 本地有但是远端没有的文件
		foreach (var local in localList)
		{
			if (!remoteList.ContainsKey(local.Key))
			{
				uselessList.Add(local.Key, local.Value);
			}
		}
		return uselessList;
	}
	protected static Dictionary<string, GameFileInfo> generateModifiedFile(Dictionary<string, GameFileInfo> remoteList, Dictionary<string, GameFileInfo> localList)
	{
		var modifiedList = new Dictionary<string, GameFileInfo>();
		foreach (var itemRemote in remoteList)
		{
			// 远端有本地没有的,文件不一致的都需要下载
			// 文件大小不一致就认为此文件有修改,文件一致即使文件内容不同也认为没有修改,这样是为了提升效率
			localList.TryGetValue(itemRemote.Key, out GameFileInfo localFile);
			if (localFile == null || itemRemote.Value.mFileSize != localFile.mFileSize)
			{
				modifiedList.Add(itemRemote.Key, itemRemote.Value);
			}
			else
			{
				// 文件大小一致,也要算一下md5码
				string localMD5 = localFile.mMD5;
				if (isEmpty(localMD5))
				{
					localMD5 = generateFileMD5(localFile.mFullPath, false);
				}
				if (localMD5 != itemRemote.Value.mMD5)
				{
					modifiedList.Add(itemRemote.Key, itemRemote.Value);
				}
			}
		}
		return modifiedList;
	}
}