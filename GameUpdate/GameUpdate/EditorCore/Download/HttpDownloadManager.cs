using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;

public class DownloadInfo
{
	public string mURL;
	public string mFileName;
	public string mUploadPath;    // 上传后的路径
	public long mDownloadOffset;	// 用于断点续传的,指定下载时从文件的第多少个字节开始下载
	public DownloadingCallback mDownloadingCallback;
	public StartCallback mStartCallback;
	public FinishCallback mFinishCallback;
	public TimeoutCallback mTimeoutCallback;
};

public class HttpDownloadManager : FrameComponent
{
	protected ThreadLock mDownloadListLock;
	protected ThreadLock mUploadListLock;
	protected List<DownloadInfo> mDownloadList;
	protected List<DownloadInfo> mUploadList;
	protected CustomThread mDownloadingThread;
	protected CustomThread mUploadingThread;
	protected string mLocalPath;
	protected byte[] mDownloadBytes;    // 用于下载的临时缓冲区,16K
	public HttpDownloadManager(string name)
	:base(name)
	{
		mDownloadListLock = new ThreadLock();
		mUploadListLock = new ThreadLock();
		mDownloadList = new List<DownloadInfo>();
		mUploadList = new List<DownloadInfo>();
		mDownloadingThread = new CustomThread("download");
		mUploadingThread = new CustomThread("upload");
		mLocalPath = Process.GetCurrentProcess().MainModule.ModuleName;
		mDownloadBytes = new byte[1024 * 16];
	}
	public override void init()
	{
		mDownloadingThread.start(downloadingThread);
		mUploadingThread.start(uploadingThread);
	}
	public override void destroy()
	{
		mDownloadingThread.destroy();
		mUploadingThread.destroy();
	}
	public override void update(float elapsedTime) { }
	public void upload(string fullURL, string fileName, string uploadPath, StartCallback start, FinishCallback finish)
	{
		mUploadListLock.waitForUnlock();
		DownloadInfo info = new DownloadInfo();
		info.mURL = StringUtility.strReplaceAll(fullURL, " ", "%20");
		info.mFileName = fileName;
		info.mUploadPath = uploadPath;
		info.mStartCallback = start;
		info.mFinishCallback = finish;
		mUploadList.Add(info);
		mUploadListLock.unlock();
	}
	public void download(string fullURL, string fileName, long offset, DownloadingCallback downloading, TimeoutCallback timeout, StartCallback start, FinishCallback finish)
	{
		mDownloadListLock.waitForUnlock();
		DownloadInfo info = new DownloadInfo();
		// 下载地址里需要将空格替换为%20
		info.mURL = StringUtility.strReplaceAll(fullURL, " ", "%20");
		info.mFileName = fileName;
		info.mDownloadingCallback = downloading;
		info.mStartCallback = start;
		info.mFinishCallback = finish;
		info.mTimeoutCallback = timeout;
		info.mDownloadOffset = offset;
		mDownloadList.Add(info);
		mDownloadListLock.unlock();
	}
	//-----------------------------------------------------------------------------------------------------------------------------------------------------
	protected Stream startDownload(string url, StartCallback start, string fileName, long offset)
	{
		try
		{
			HttpWebRequest request = WebRequest.Create(url) as HttpWebRequest;
			request.MaximumAutomaticRedirections = 2;
			request.AddRange(offset);
			// 发送请求并获取相应回应数据
			HttpWebResponse response = request.GetResponse() as HttpWebResponse;
			// 直到request.GetResponse()程序才开始向目标网页发送Post请求
			Stream responseStream = response.GetResponseStream();
			// 设置5秒超时
			responseStream.ReadTimeout = 5000;
			if (start != null)
			{
				// response.ContentLength只是剩余需要下载的长度,需要加上下载起始偏移才是文件的真实大小
				start(fileName, response.ContentLength + offset);
			}
			return responseStream;
		}
		catch (Exception)
		{
			return null;
		}
	}
	protected bool downloadingThread()
	{
		mDownloadListLock.waitForUnlock();
		if (mDownloadList.Count == 0)
		{
			mDownloadListLock.unlock();
			return true;
		}
		DownloadInfo info = mDownloadList[0];
		mDownloadList.RemoveAt(0);
		mDownloadListLock.unlock();
		Stream responseStream = startDownload(info.mURL, info.mStartCallback, info.mFileName, info.mDownloadOffset);
		bool download = true;
		while (download)
		{
			try
			{
				int size = 0;
				do
				{
					//创建本地文件写入流
					size = responseStream.Read(mDownloadBytes, 0, mDownloadBytes.Length);
					if (size == 0)
					{
						break;
					}
					else if (info.mDownloadingCallback != null)
					{
						info.mDownloadingCallback(mDownloadBytes, size);
					}
				} while (true);
				responseStream.Close();
			}
			catch (Exception ex)
			{
				// 下载超时,如果设置了超时函数,则继续下载
				if (ex.Message == "操作已超时。" && info.mTimeoutCallback != null)
				{
					responseStream.Close();
					// 从上次下载的点重新开始下载
					long lastPos = info.mTimeoutCallback();
					// 此处不能再调用开始下载的回调
					responseStream = startDownload(info.mURL, null, info.mFileName, lastPos);
					continue;
				}
				else
				{
					download = false;
					if (info.mFinishCallback != null)
					{
						info.mFinishCallback(info.mFileName, false);
					}
					logError(info.mFileName + "下载失败 : " + ex.Message, true);
					pushDelayCommand<CommandDownloadManagerCancel>(mDownloadManager);
					return false;
				}
			}
			if (info.mFinishCallback != null)
			{
				info.mFinishCallback(info.mFileName, true);
			}
			download = false;
		}
		return true;
	}
	protected bool uploadingThread()
	{
		mUploadListLock.waitForUnlock();
		if (mUploadList.Count == 0)
		{
			mUploadListLock.unlock();
			return true;
		}

		DownloadInfo info = mUploadList[0];
		mUploadList.RemoveAt(0);
		mUploadListLock.unlock();
		string urlString = info.mURL + "?path=[" + info.mUploadPath + "]";
		// 创建WebClient实例 
		WebClient myWebClient = new WebClient();
		myWebClient.Credentials = CredentialCache.DefaultCredentials;
		// 要上传的文件 
		FileStream fs = new FileStream(info.mFileName, FileMode.Open, FileAccess.Read);
		if (info.mStartCallback != null)
		{
			info.mStartCallback(info.mFileName, 0);
		}
		BinaryReader r = new BinaryReader(fs);
		byte[] postArray = r.ReadBytes((int)fs.Length);
		Stream postStream = myWebClient.OpenWrite(urlString, "PUT");
		try
		{
			bool canWrite = postStream.CanWrite;
			if (canWrite)
			{
				postStream.Write(postArray, 0, postArray.Length);
			}
			postStream.Close();
			fs.Dispose();
			if (info.mFinishCallback != null)
			{
				info.mFinishCallback(info.mFileName, canWrite);
			}
			if (!canWrite)
			{
				logError(info.mFileName + "上传文件失败 : 文件不可写入!", true);
				pushDelayCommand<CommandDownloadManagerCancel>(mDownloadManager);
			}
			return true;
		}
		catch (Exception err)
		{
			postStream.Close();
			fs.Dispose();
			if (info.mFinishCallback != null)
			{
				info.mFinishCallback(info.mFileName, false);
				logError(info.mFileName + "上传文件失败 : " + err.Message, true);
			}
			pushDelayCommand<CommandDownloadManagerCancel>(mDownloadManager);
			return false;
		}
	}
}