using System;
using System.Collections.Generic;
using System.IO;
using System.Net;

public class DownloadInfo
{
	public string mURL;
	public string mFileName;
	public string mUploadPath;		// 上传后的路径
	public long mDownloadOffset;	// 用于断点续传的,指定下载时从文件的第多少个字节开始下载
	public DownloadingCallback mDownloadingCallback;
	public StartCallback mStartCallback;
	public FinishCallback mFinishCallback;
	public TimeoutCallback mTimeoutCallback;
};

public class HttpDownloadManager : FrameComponent
{
	protected ThreadLock mDownloadListLock;
	protected List<DownloadInfo> mDownloadList;
	protected CustomThread mDownloadingThread;
	protected byte[] mDownloadBytes;			// 用于下载的临时缓冲区,1MB
	public HttpDownloadManager(string name)
	:base(name)
	{
		mDownloadListLock = new ThreadLock();
		mDownloadList = new List<DownloadInfo>();
		mDownloadingThread = new CustomThread("download");
		mDownloadBytes = new byte[1024 * 1024];
	}
	public override void init()
	{
		mDownloadingThread.start(downloadingThread);
	}
	public override void destroy()
	{
		mDownloadingThread.destroy();
	}
	public void download(string fullURL, string fileName, long offset, DownloadingCallback downloading, TimeoutCallback timeout, StartCallback start, FinishCallback finish)
	{
		mDownloadListLock.waitForUnlock();
		DownloadInfo info = new DownloadInfo();
		// 下载地址里需要将空格替换为%20
		info.mURL = strReplaceAll(fullURL, " ", "%20");
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
	protected Stream startDownload(string url, StartCallback startCallback, string fileName, long offset)
	{
		try
		{
			var request = WebRequest.Create(url) as HttpWebRequest;
			request.MaximumAutomaticRedirections = 2;
			request.AddRange(offset);
			// 发送请求并获取相应回应数据
			var response = request.GetResponse() as HttpWebResponse;
			// 直到request.GetResponse()程序才开始向目标网页发送Post请求
			Stream responseStream = response.GetResponseStream();
			// 设置10秒超时
			responseStream.ReadTimeout = 10000;
			// response.ContentLength只是剩余需要下载的长度,需要加上下载起始偏移才是文件的真实大小
			startCallback?.Invoke(fileName, response.ContentLength + offset);
			return responseStream;
		}
		catch (Exception e)
		{
			logConsole("异常:" + e.Message + ", url:" + url + ", fileName:" + fileName);
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
		if(responseStream == null)
		{
			logError(getFileName(info.mFileName) + "下载失败 : 网络异常!", true);
			pushDelayCommand<CommandDownloadManagerCancel>(mDownloadManager);
			// 遇到下载失败的情况,则不会再继续下载了,需要重新启动才能下载
			return false;
		}
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
					info.mDownloadingCallback?.Invoke(mDownloadBytes, size);
				} while (true);
				responseStream.Close();
			}
			catch (Exception ex)
			{
				// 下载超时,如果设置了超时函数,则继续下载
				if (ex.Message.Contains("操作已超时。") && info.mTimeoutCallback != null)
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
					info.mFinishCallback?.Invoke(info.mFileName, false);
					logError(getFileName(info.mFileName) + "下载失败 : " + ex.Message, true);
					pushDelayCommand<CommandDownloadManagerCancel>(mDownloadManager);
					return false;
				}
			}
			info.mFinishCallback?.Invoke(info.mFileName, true);
			download = false;
		}
		return true;
	}
}