using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Diagnostics;

public class DownloadFileInfo
{
	public string mFileName;	// 文件位置,以dndl_Data开头的相对路径
	public string mMD5;
	public long mFileSize;
	public bool mBigFile;
};

public enum UPGRADE_STATE
{
	US_NONE,
	US_DOWNLOADING_VERSION,
	US_WAIT_FOR_UPGRADE,
	US_DOWNLOADING_FILE_LIST,
	US_PARSING_REMOTE_FILE_LIST,
	US_GENERATE_LOCAL_FILE,
	US_GENERATE_MODIFIED_FILE,
	US_DOWNLOADING_REMOTE_FILE,
	US_UPDATE_LOCAL_FILE,
	US_DONE,
};

public class DownloadManager : FrameComponent
{
	protected Dictionary<string, FileWrap> mDownloadingFileList;     // 正在下载或已经下载完毕的更新文件
	protected Dictionary<string, FileWrap> mDownloadedList;          // 已经下载完毕的更新文件
	protected Dictionary<string, DownloadFileInfo> mRemoteFileList;  // 远端文件列表,Key为远端文件位置
	protected Dictionary<string, DownloadFileInfo> mLocalFileList;   // 本地文件列表
	protected Dictionary<string, DownloadFileInfo> mModifiedList;    // 需要更新的文件列表
	protected List<string> mIgnorePathList;
	protected string mRemoteVersion;
	protected string mLocalVersion;
	protected string mDataPath;
	protected FileWrap mCurDownloading;
	protected float mCurTimeCount;      // 当前计时,小于0表示还没有开始计时,大于等于0表示正在计时
	protected float mCurSpeed;
	protected float mRemainTime;                              // 剩余下载时间
	protected float mDownloadTimeOut;
	protected float mTotalSize;         // 当前需要下载的文件的总大小
	protected long mDownloadedSize;    // 已经下载的大小,但是不包括当前文件的已下载字节数
	protected float mSpeedInSecond;
	protected float mLastDownloadingTime;// 上次接收到下载数据到当前的时间,小于0表示未计时,大于等于0表示正在计时,在第一次接收到下载数据时开始计时
	protected UPGRADE_STATE mState;
	protected bool mCancel;
	public DownloadManager(string name)
		: base(name)
	{
		mDownloadingFileList = new Dictionary<string, FileWrap>();
		mDownloadedList = new Dictionary<string, FileWrap>();
		mRemoteFileList = new Dictionary<string, DownloadFileInfo>();
		mLocalFileList = new Dictionary<string, DownloadFileInfo>();
		mModifiedList = new Dictionary<string, DownloadFileInfo>();
		mIgnorePathList = new List<string>();
		mCurDownloading = null;
		mCancel = false;
		mState = UPGRADE_STATE.US_NONE;
		mIgnorePathList.Add("dndl_Data/StreamingAssets/CustomSound/RaceSound");
		mCurTimeCount = -1.0f;
		mCurSpeed = 0.0f;
		mSpeedInSecond = 0.0f;
		mTotalSize = 0.0f;
		mDownloadedSize = 0;
		mRemainTime = 0.0f;
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
				mSpeedInSecond = 0.0f;
				if (!MathUtility.isFloatZero(mCurSpeed))
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
				mState = UPGRADE_STATE.US_NONE;
				setCancel(true, false);
				mLastDownloadingTime = 0.0f;
			}
		}
	}
	public void requestDownloadVersion()
	{
		mState = UPGRADE_STATE.US_DOWNLOADING_VERSION;
		mEditorCore.sendDelayEvent(CORE_EVENT_TYPE.CET_START_DOWNLOAD_VERSION);
		// 开始下载版本文件
		repuestDownload(CommonDefine.VERSION, 0);
	}
	public long getDownloadedSize()
	{
		return mDownloadedSize + mCurDownloading.getCurSize();
	}
	public FileWrap getCurDownloadingFile() { return mCurDownloading; }
	public int getTotalCount() { return mModifiedList.Count(); }
	public int getDownloadedCount() { return mDownloadedList.Count(); }
	public float getCurSpeed() { return mCurSpeed; }
	public float getRemainTime() { return mRemainTime; }
	public float getTotalSize() { return mTotalSize; }
	public bool getDownloading() { return mState == UPGRADE_STATE.US_DOWNLOADING_REMOTE_FILE; }
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
	public void startUpdateGame(bool start)
	{
		if (start)
		{
			setState(UPGRADE_STATE.US_DOWNLOADING_FILE_LIST);
			mEditorCore.sendDelayEvent(CORE_EVENT_TYPE.CET_START_DOWNLOAD_LIST_FILE);
			// 首先从远端下载列表文件
			repuestDownload(CommonDefine.FILELIST, 0);
		}
		else
		{
			setState(UPGRADE_STATE.US_NONE);
		}
	}
	public void setState(UPGRADE_STATE state)
	{
		mState = state;
	}
	//----------------------------------------------------------------------------------------------------------------------------------------------------
	// 返回值表示是否继续下载
	protected bool downloading(byte[] pBuffer, int nSize)
	{
		if (!mCurDownloading.writeFile(pBuffer, nSize))
		{
			logError("文件写入失败! 文件 : " + mCurDownloading.getFileName(), true);
			return false;
		}
		mSpeedInSecond = mSpeedInSecond + nSize;
		mLastDownloadingTime = 0.0f;
		if (mCurDownloading.getFileName() == CommonDefine.FILELIST)
		{
			mEditorCore.sendDelayEvent(CORE_EVENT_TYPE.CET_DOWNLOADING_LIST_FILE, StringUtility.floatToString((float)mCurDownloading.getCurSize() / mCurDownloading.getTotalSize() * 100.0f, 2));
		}
		// 如果取消更新,则需要返回非0,中断下载
		if (mCancel)
		{
			pushDelayCommand<CommandDownloadManagerCancel>(this);
		}
		logConsole("已下载 : " + mCurDownloading.getCurSize() + "/" + mCurDownloading.getTotalSize() + "Bytes");
		return !mCancel;
	}
	protected long onTimeout()
	{
		return mCurDownloading.getCurSize();
	}
	protected void onStart(string fileName, long totalSize)
	{
		if (!mDownloadingFileList.ContainsKey(fileName))
		{
			return;
		}
		mCurDownloading = mDownloadingFileList[fileName];
		mCurDownloading.setTotalSize(totalSize);
		// 其他文件在接收到数据时就写入文件,并且先存放到临时目录中
		if (fileName != CommonDefine.VERSION && fileName != CommonDefine.FILELIST)
		{
			mCurDownloading.startWrite(fileName, CommonDefine.TEMP_PATH + fileName);
			mCurDownloading.setWriteMode(WRITE_MODE.WM_AUTO_WRITE);
			mEditorCore.sendDelayEvent(CORE_EVENT_TYPE.CET_START_DOWNLOAD, fileName);
		}
		else
		{
			mCurDownloading.startWrite(fileName);
			// 版本文件只在完成下载时才写入文件,标记着当前资源版本
			if (fileName == CommonDefine.VERSION)
			{
				mCurDownloading.setWriteMode(WRITE_MODE.WM_WRITE_FINISH);
			}
			// 列表文件不写入文件
			else if (fileName == CommonDefine.FILELIST)
			{
				mCurDownloading.setWriteMode(WRITE_MODE.WM_DONT_WRITE);
			}
		}
	}
	protected void onFinish(string fileName, bool downloadSuccess)
	{
		if (!mDownloadingFileList.ContainsKey(fileName))
		{
			return;
		}
		// 如果没有下载成功,则不再继续执行后续更新逻辑
		if (!downloadSuccess)
		{
			setState(UPGRADE_STATE.US_NONE);
			return;
		}
		mLastDownloadingTime = -1.0f;
		FileWrap file = mDownloadingFileList[fileName];
		if (fileName == CommonDefine.VERSION)
		{
			notifyVersionDownloaded(file);
		}
		else if (fileName == CommonDefine.FILELIST)
		{
			notifyFileListDownloaded(file);
		}
		else
		{
			string md5 = "";
			if (mRemoteFileList.ContainsKey(file.getFileName()))
			{
				md5 = mRemoteFileList[file.getFileName()].mMD5;
			}
			// 如果失败,则不再继续更新
			int ret = file.finishWrite(md5);
			if (ret != 0)
			{
				if (ret == 1)
				{
					logError("文件下载失败," + fileName + "文件校验失败, 可能是上次未更新完成且长时间没有更新,请重新更新", true);
				}
				else if (ret == 2)
				{
					logError("文件下载失败,文件可能被占用 : " + fileName, true);
				}
				setState(UPGRADE_STATE.US_NONE);
				pushDelayCommand<CommandDownloadManagerCancel>(mDownloadManager);
			}
			else
			{
				mDownloadedSize += file.getTotalSize();
				// 加入下载完成的列表
				mDownloadedList.Add(fileName, file);
				mEditorCore.sendDelayEvent(CORE_EVENT_TYPE.CET_FINISH_DOWNLOAD, file.getFileName());
				// 全部文件都下载完毕,则版本更新完毕
				if (mDownloadedList.Count() == mModifiedList.Count())
				{
					notifyAllDownloaded();
					// 更新完成
					mEditorCore.sendDelayEvent(CORE_EVENT_TYPE.CET_UPDATE_DONE);
					// 更新版本号
					done();
				}
			}
		}
	}
	protected void notifyVersionDownloaded(FileWrap versionFile)
	{
		// 版本文件下载完毕后,提示是否有新版本可以更新
		string localUpdateInfo = "";
		string localVersion = getVersionFromFile(FileUtility.openTxtFile(CommonDefine.VERSION), ref localUpdateInfo);
		string remoteUpdateInfo = "";
		string remoteVersion = getVersionFromFile(BinaryUtility.bytesToString(versionFile.getFileData()), ref remoteUpdateInfo);
		bool hasNewVersion = remoteVersion != localVersion;
		if (hasNewVersion)
		{
			setState(UPGRADE_STATE.US_WAIT_FOR_UPGRADE);
		}
		else
		{
			done();
		}
		List<string> paramList = new List<string>();
		paramList.Add(StringUtility.boolToString(hasNewVersion));
		paramList.Add(remoteUpdateInfo);
		mEditorCore.sendDelayEvent(CORE_EVENT_TYPE.CET_NEW_VERSION, paramList);
	}
	protected void notifyFileListDownloaded(FileWrap listFile)
	{
		// 列表文件下载完毕,解析列表
		setState(UPGRADE_STATE.US_PARSING_REMOTE_FILE_LIST);
		string listContent = BinaryUtility.bytesToString(listFile.getFileData());
		parseRemoteFileList(listContent);
		setState(UPGRADE_STATE.US_GENERATE_LOCAL_FILE);
		// 检查本地文件
		getLocalFileList();
		// 如果在检查本地文件过程中取消了,则直接返回
		if (mCancel)
		{
			setState(UPGRADE_STATE.US_NONE);
			return;
		}

		// 判断出需要更新的文件
		setState(UPGRADE_STATE.US_GENERATE_MODIFIED_FILE);
		// 然后下载修改过和新增的文件
		mModifiedList = generateModifiedFile(mRemoteFileList, mLocalFileList, mIgnorePathList);
		setState(UPGRADE_STATE.US_DOWNLOADING_REMOTE_FILE);
		if (mModifiedList.Count() == 0)
		{
			// 如果有临时目录，则将临时目录中的所有文件替换到游戏目录
			notifyAllDownloaded();
			mEditorCore.sendDelayEvent(CORE_EVENT_TYPE.CET_NOTHING_UPDATE);
			done();
		}
		else
		{
			// 计算所有需要下载的文件的大小
			mTotalSize = 0.0f;
			foreach (var item in mModifiedList)
			{
				mTotalSize = mTotalSize + item.Value.mFileSize;
			}
			// 开始下载所有文件,并且开始计时
			mCurTimeCount = 0.0f;
			foreach (var item in mModifiedList)
			{
				// 如果临时文件存在,表示之前没下载成功,暂时删除临时文件
				// 如果实现断点续传时需要获取临时文件的大小
				long offset = 0;
				if (FileUtility.isFileExist(CommonDefine.TEMP_PATH + item.Key + CommonDefine.TEMP_FILE_EXTENSION))
				{
					offset = FileUtility.getFileSize(CommonDefine.TEMP_PATH + item.Key + CommonDefine.TEMP_FILE_EXTENSION);
				}
				repuestDownload(item.Key, offset);
			}
		}
	}
	// 所有文件都已经下载完毕,更新所有文件
	protected void notifyAllDownloaded()
	{
		mEditorCore.sendDelayEvent(CORE_EVENT_TYPE.CET_UPDATING_FILE);
		setState(UPGRADE_STATE.US_UPDATE_LOCAL_FILE);
		// 首先删除本地残存的无用文件,也就是本地有但是服务器没有的
		Dictionary<string, DownloadFileInfo> uselessFile = generateUselessFile(mRemoteFileList, mLocalFileList, mIgnorePathList);
		int uselessFileCount = uselessFile.Count;
		int i = 0;
		foreach (var item in uselessFile)
		{
			FileUtility.deleteFile(item.Key);
			++i;
			// 删除文件占总进度的10%
			mEditorCore.sendDelayEvent(CORE_EVENT_TYPE.CET_UPDATING_PROGRESS, StringUtility.floatToString(i / (float)uselessFileCount * 0.1f));
		}
		// 清除空文件夹
		FileUtility.deleteEmptyFolder("./");
		List<string> temporaryFileList = new List<string>();
		FileUtility.findFiles(CommonDefine.TEMP_PATH, ref temporaryFileList);
		// 将临时目录中的文件替换掉本地文件
		int temporaryFilecount = temporaryFileList.Count;
		for (int j = 0; j < temporaryFilecount; ++j)
		{
			string sourceFile = temporaryFileList[j];
			string destFile = sourceFile.Substring(CommonDefine.TEMP_PATH.Length, sourceFile.Length - CommonDefine.TEMP_PATH.Length);
			// 检查是否有配置文件需要保存
			afterFileDownloaded(destFile);
			FileUtility.copyFile(sourceFile, destFile);
			// 替换文件占总进度的90%
			mEditorCore.sendDelayEvent(CORE_EVENT_TYPE.CET_UPDATING_PROGRESS, StringUtility.floatToString(j / (float)temporaryFilecount * 0.9f + 0.1f));
		}
		// 删除下载临时目录
		FileUtility.deleteFolder(CommonDefine.TEMP_PATH);
	}
	protected void afterFileDownloaded(string fileName)
	{
		ConfigBase config = null;
		if (fileName == CommonDefine.FILE_PATH + "ApplicationSetting.txt")
		{
			config = new ApplicationConfig(fileName);
		}
		else if (fileName == CommonDefine.FILE_PATH + "FrameFloatConfig.txt")
		{
			config = new FrameConfig(fileName);
		}
		else if (fileName == CommonDefine.FILE_PATH + "GameFloatConfig.txt")
		{
			config = new GameFloatConfig(fileName);
		}
		else if (fileName == CommonDefine.FILE_PATH + "GameStringConfig.txt")
		{
			config = new GameStringConfig(fileName);
		}
		if(config != null)
		{
			config.init();
			config.mergeConfig(fileName, CommonDefine.TEMP_PATH + fileName);
		}
	}
	protected void done()
	{
		// 更新版本号
		if (mDownloadingFileList.ContainsKey(CommonDefine.VERSION))
		{
			mDownloadingFileList[CommonDefine.VERSION].finishWrite("");
		}
		setState(UPGRADE_STATE.US_DONE);
		mCurTimeCount = -1.0f;
		mCurSpeed = 0.0f;
		mRemainTime = 0.0f;
	}
	protected void parseRemoteFileList(string listContent)
	{
		try
		{
			string[] fileList = StringUtility.split(listContent, true, "\r\n");
			if (fileList.Count() == 0)
			{
				logError("列表文件错误!", true);
				return;
			}
			int readFileCount = StringUtility.stringToInt(fileList[0]);
			if (readFileCount == 0)
			{
				logError("文件数量读取错误!", true);
				return;
			}
			int fileCount = fileList.Count();
			if (fileCount - 1 != readFileCount)
			{
				logError("文件数量不匹配, 文件头记录数量 : " + readFileCount + ", 实际数量 : " + (fileCount - 1), true);
				return;
			}
			// 生成远端文件列表
			for (int i = 1; i < fileCount; ++i)
			{
				string[] contentList = StringUtility.split(fileList[i], false, "\t");
				if (contentList.Count() != 3)
				{
					logError("远端文件列表已损坏!行号 : " + i, true);
					continue;
				}
				DownloadFileInfo info = new DownloadFileInfo();
				info.mFileName = StringUtility.strReplaceAll(contentList[0], "%20", " ");
				info.mFileSize = StringUtility.stringToInt(contentList[1]);
				info.mMD5 = contentList[2];
				info.mBigFile = info.mFileSize > CommonDefine.BIG_FILE_SIZE;
				mRemoteFileList.Add(info.mFileName, info);
			}
		}
		catch (Exception e)
		{
			logError("解析错误 : " + e.Message);
		}
	}
	protected void getLocalFileList()
	{
		if (mLocalFileList.Count() > 0)
		{
			logError("local list is not empty!", true);
			return;
		}

		// 先查找游戏目录
		string startPath = "./";
		List<string> fileList = new List<string>();
		FileUtility.findFiles(startPath, ref fileList);
		int fileCount = fileList.Count();
		for (int i = 0; i < fileCount; ++i)
		{
			// 排除临时文件夹
			if (!StringUtility.startWith(fileList[i], startPath + CommonDefine.TEMP_PATH))
			{
				DownloadFileInfo info = new DownloadFileInfo();
				string newPath = fileList[i].Substring(startPath.Length, fileList[i].Length - startPath.Length);
				mLocalFileList.Add(newPath, info);
			}
		}
		// 查找临时目录
		string tempPath = CommonDefine.TEMP_PATH;
		List<string> tempFile = new List<string>();
		FileUtility.findFiles(tempPath, ref tempFile);
		int tempCount = tempFile.Count();
		for (int i = 0; i < tempCount; ++i)
		{
			tempFile[i] = tempFile[i].Substring(tempPath.Length, tempFile[i].Length - tempPath.Length);
		}
		// 排除版本文件,列表文件,程序自身
		string thisFileName = Process.GetCurrentProcess().MainModule.FileName;
		string strThisFile = StringUtility.getFileName(thisFileName);
		strThisFile = strThisFile.Substring(0, strThisFile.IndexOf("."));
		// 移除所有以程序名开头的文件
		List<string> keyList = new List<string>(mLocalFileList.Keys);
		foreach (var item in keyList)
		{
			if (StringUtility.startWith(item, strThisFile + "."))
			{
				mLocalFileList.Remove(item);
			}
		}
		mLocalFileList.Remove(CommonDefine.VERSION);
		mLocalFileList.Remove(CommonDefine.FILELIST);
		int totalCount = mLocalFileList.Count + tempFile.Count;
		mEditorCore.sendDelayEvent(CORE_EVENT_TYPE.CET_START_GENERATE_LOCAL_FILE, StringUtility.intToString(totalCount));

		// 计算文件信息
		int index = 0;
		foreach (var item in mLocalFileList)
		{
			if (mCancel)
			{
				break;
			}
			DownloadFileInfo info = item.Value;
			info.mFileName = item.Key;
			info.mFileSize = FileUtility.getFileSize(item.Key);
			info.mBigFile = info.mFileSize > CommonDefine.BIG_FILE_SIZE;
			// 如果是大文件,则不计算MD5
			if (!info.mBigFile)
			{
				info.mMD5 = FileUtility.generateFileMD5(item.Key);
			}
			List<string> param = new List<string>();
			param.Add(StringUtility.intToString(totalCount));
			param.Add(StringUtility.intToString(index + 1));
			mEditorCore.sendDelayEvent(CORE_EVENT_TYPE.CET_GENERATING_LOCAL_FILE, param);
			++index;
		}
		foreach (var item in tempFile)
		{
			if (mCancel)
			{
				break;
			}
			string fileName = item;
			DownloadFileInfo info = null;
			if (mLocalFileList.ContainsKey(fileName))
			{
				info = mLocalFileList[fileName];
			}
			else
			{
				info = new DownloadFileInfo();
				mLocalFileList.Add(fileName, info);
			}
			info.mFileName = fileName;
			info.mFileSize = FileUtility.getFileSize(CommonDefine.TEMP_PATH + fileName);
			info.mBigFile = info.mFileSize > CommonDefine.BIG_FILE_SIZE;
			// 如果是大文件,则不计算MD5
			if (!info.mBigFile)
			{
				info.mMD5 = FileUtility.generateFileMD5(CommonDefine.TEMP_PATH + fileName);
			}
			List<string> param = new List<string>();
			param.Add(StringUtility.intToString(totalCount));
			param.Add(StringUtility.intToString(index + 1));
			mEditorCore.sendDelayEvent(CORE_EVENT_TYPE.CET_GENERATING_LOCAL_FILE, param);
			++index;
		}
		mEditorCore.sendDelayEvent(CORE_EVENT_TYPE.CET_FINISH_GENERATE_LOCAL_FILE);
	}
	protected void repuestDownload(string fileName, long offset)
	{
		mDownloadingFileList.Add(fileName, new FileWrap());
		mHttpDownloadManager.download(CommonDefine.REMOTE_URL + fileName, fileName, offset, downloading, onTimeout, onStart, onFinish);
	}
	protected static Dictionary<string, DownloadFileInfo> generateUselessFile(Dictionary<string, DownloadFileInfo> remoteList,
	Dictionary<string, DownloadFileInfo> localList, List<string> ignorePathList)
	{
		Dictionary<string, DownloadFileInfo> uselessList = new Dictionary<string, DownloadFileInfo>();
		// 本地有但是远端没有的文件
		foreach (var local in localList)
		{
			if (isIgnoreFile(local.Key, ignorePathList))
			{
				continue;
			}
			if (!remoteList.ContainsKey(local.Key))
			{
				uselessList.Add(local.Key, local.Value);
			}
		}
		return uselessList;
	}
	protected static Dictionary<string, DownloadFileInfo> generateModifiedFile(Dictionary<string, DownloadFileInfo> remoteList,
													Dictionary<string, DownloadFileInfo> localList, List<string> ignorePathList)
	{
		Dictionary<string, DownloadFileInfo> modifiedList = new Dictionary<string, DownloadFileInfo>();
		foreach (var itemRemote in remoteList)
		{
			if (isIgnoreFile(itemRemote.Key, ignorePathList))
			{
				continue;
			}
			bool isModify = false;
			// 远端有本地也有的
			if (localList.ContainsKey(itemRemote.Key))
			{
				// 文件大小不一致,或者文件大小一致,但是md5码不一致,如果是大文件,大小一致时就可以认为该文件没有修改
				if (itemRemote.Value.mFileSize != localList[itemRemote.Key].mFileSize ||
					(!itemRemote.Value.mBigFile && itemRemote.Value.mMD5 != localList[itemRemote.Key].mMD5))
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
				modifiedList.Add(itemRemote.Key, itemRemote.Value);
			}
		}
		return modifiedList;
	}
	protected static bool isIgnoreFile(string file, List<string> ignorePathList)
	{
		foreach (var item in ignorePathList)
		{
			if (StringUtility.findSubstr(file, item, false))
			{
				return true;
			}
		}
		return false;
	}
	protected string getVersionFromFile(string fileString, ref string updateInfo)
	{
		string version = "";
		string nextLineString = "\r\n\r\n"; // 两个回车换行分隔版本号和版本内容
		int nextLinePos = fileString.IndexOf(nextLineString);
		if (nextLinePos != -1)
		{
			version = fileString.Substring(0, nextLinePos);
			updateInfo = fileString.Substring(nextLinePos + nextLineString.Length, fileString.Length - nextLinePos - nextLineString.Length);
		}
		else
		{
			version = fileString;
		}
		return version;
	}
}