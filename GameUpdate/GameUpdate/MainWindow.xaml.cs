using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace GameUpdate
{
	/// <summary>
	/// MainWindow.xaml 的交互逻辑
	/// </summary>
	public partial class MainWindow : Window, EventHandler
	{
		protected EditorCore mEditorCore;
		protected DispatcherTimer mTimer;
		protected DateTime mStartTime;
		protected OKDialog mOKDialog;
		protected YesNoDialog mYesNoDialog;
		protected BusyInfo mBusyInfo;
		protected bool mRunning;
		protected bool mUpdateDone;
		protected bool mAutoStartGame;    // 下载完毕后是否自动启动游戏
		protected bool mAutoClose;        // 上传完毕后是否自动关闭程序
		protected bool mFrameDone;
		public MainWindow()
		{
			Application.Current.ShutdownMode = ShutdownMode.OnExplicitShutdown;
			InitializeComponent();
			mFrameDone = true;
			mRunning = true;
			mUpdateDone = false;
			mAutoStartGame = true;
			mAutoClose = false;
			mOKDialog = new OKDialog();
			mYesNoDialog = new YesNoDialog();
			mBusyInfo = new BusyInfo();
			mEditorCore = new EditorCore();
			init();
		}
		public void init()
		{
			string gymID = SystemUtility.getRegistryValue("Software\\北京踏行天际科技发展有限公司\\电能动力4.0", "REGISTE_CODE");
			if (gymID == "")
			{
				mOKDialog.setInfo("找不到设备的注册信息!");
				mOKDialog.ShowDialog();
				exit();
				return;
			}
			// 如果游戏已经运行则直接退出
			if (SystemUtility.isProgressRunning(CommonDefine.GAME_NAME))
			{
				mOKDialog.setInfo("游戏已经运行!");
				mOKDialog.ShowDialog();
				exit();
				return;
			}
			mEditorCore.init();
			registeEventHandler();
			mStartTime = DateTime.Now;
			mTimer = new DispatcherTimer();
			mTimer.Interval = TimeSpan.FromMilliseconds(30);
			mTimer.Tick += Tick;
			mTimer.Start();
			// 默认隐藏窗口
			Hide();

			setAutoCloseUploaded(false);
			setAutoStartGame(true);

			// 判断当前目录下是否有需要上传的文件
			// 只上传崩溃产生的文件,文件夹名格式20XX-XX-XX_XXXXX
			List<string> folders = new List<string>();
			string path = "./";
			FileUtility.findDirectory(path, ref folders, false);
			for (int i = 0; i < folders.Count;)
			{
				// 移除./
				folders[i] = folders[i].Substring(path.Length, folders[i].Length - path.Length);
				// 符合条件的文件夹保留,其余的移除
				if (folders[i].Length > 11 && StringUtility.startWith(folders[i], "20") && folders[i][4] == '-' && folders[i][7] == '-' && folders[i][10] == '_')
				{
					++i;
				}
				else
				{
					folders.RemoveAt(i);
				}
			}
			List<string> files = new List<string>();
			int count = folders.Count;
			for (int i = 0; i < count; ++i)
			{
				FileUtility.findFiles(folders[i], ref files);
			}
			List<FileUploadInfo> uploadFiles = new List<FileUploadInfo>();
			int uploadCount = files.Count;
			for (int i = 0; i < uploadCount; ++i)
			{
				FileUploadInfo info = new FileUploadInfo();
				info.mFileName = files[i];
				info.mUploadPath = gymID + "/" + StringUtility.getFilePath(files[i]);
				uploadFiles.Add(info);
			}
			start(uploadFiles);
		}
		public void exit()
		{
			Application.Current.Shutdown();
			destroy();
		}
		void setAutoStartGame(bool autoStart) { mAutoStartGame = autoStart; }
		void setAutoCloseUploaded(bool autoClose) { mAutoClose = autoClose; }
		public void destroy()
		{
			mTimer.Stop();
			mTimer = null;
			unregisteEventHandler();
			mEditorCore.destroy();
			mEditorCore = null;
		}
		private void Tick(object sender, EventArgs e)
		{
			if (!mFrameDone)
			{
				return;
			}
			mFrameDone = false;
			if (!mRunning)
			{
				exit();
				return;
			}
			DateTime now = DateTime.Now;
			float elapsedTime = (float)(now - mStartTime).TotalMilliseconds / 1000.0f;
			mEditorCore.update(elapsedTime);
			update(elapsedTime);
			mStartTime = now;
			mFrameDone = true;	
		}
		public void update(float elapsedTime)
		{
			DownloadManager manager = EditorBase.mDownloadManager;
			FileWrap file = manager.getCurDownloadingFile();
			if (manager.getDownloading() && file != null && file.getTotalSize() > 0 && manager.getTotalCount() > 0)
			{
				updateState();
			}
		}
		public void registeEventHandler()
		{
			if (EditorBase.mEventSystem != null)
			{
				EditorBase.mEventSystem.registeAllEvent(this);
			}
		}
		public void unregisteEventHandler()
		{
			if(EditorBase.mEventSystem != null)
			{
				EditorBase.mEventSystem.unregisteAllEvent(this);
			}
		}
		public void notifyEvent(CORE_EVENT_TYPE type, List<string> param)
		{
			if (type == CORE_EVENT_TYPE.CET_ERROR_LOG)
			{
				mOKDialog.setTitle("错误!");
				mOKDialog.setInfo(param[0]);
				mOKDialog.ShowDialog();
			}
			else if (type == CORE_EVENT_TYPE.CET_INFO_LOG)
			{
				mOKDialog.setTitle("提示!");
				mOKDialog.setInfo(param[0]);
				mOKDialog.ShowDialog();
			}
			else if (type == CORE_EVENT_TYPE.CET_STOP_PROGRAM)
			{
				mRunning = false;
			}
			else if (type == CORE_EVENT_TYPE.CET_START_DOWNLOAD_VERSION)
			{
				mBusyInfo.setInfo("正在下载版本号...");
				mBusyInfo.Show();
			}
			else if (type == CORE_EVENT_TYPE.CET_START_DOWNLOAD_LIST_FILE)
			{
				mBusyInfo.setInfo("正在下载列表文件...0%");
				mBusyInfo.Show();
			}
			else if(type == CORE_EVENT_TYPE.CET_DOWNLOADING_LIST_FILE)
			{
				mBusyInfo.setInfo("正在下载列表文件..." + param[0] + "%");
				mBusyInfo.Show();
			}
			// 版本文件下载完毕
			else if (type == CORE_EVENT_TYPE.CET_NEW_VERSION)
			{
				if (StringUtility.stringToBool(param[0]))
				{
					mBusyInfo.Hide();
					Show();
					mYesNoDialog.setInfo("发现有新版本,是否要更新?");
					mYesNoDialog.setDownLoadInfo(param[1]);
					bool? ret = mYesNoDialog.ShowDialog();
					EditorBase.mDownloadManager.startUpdateGame(ret == true);
					if (ret != true)
					{
						downloadDone(false);
					}
				}
				else
				{
					finishDownload();
					mBusyInfo.Hide();
					mOKDialog.setInfo("未发现新版本!");
					mOKDialog.ShowDialog();
					downloadDone(false);
				}
			}
			// 版本文件和列表文件检查完毕,开始下载资源
			else if (type == CORE_EVENT_TYPE.CET_START_DOWNLOAD)
			{
				startDownload();
			}
			// 所有文件下载完毕,正在更新文件
			else if (type == CORE_EVENT_TYPE.CET_UPDATING_FILE)
			{
				mBusyInfo.Show();
				mBusyInfo.setInfo("正在更新文件...0%");
			}
			else if (type == CORE_EVENT_TYPE.CET_UPDATING_PROGRESS)
			{
				float progress = StringUtility.stringToFloat(param[0]);
				mBusyInfo.setInfo("正在更新文件..." + StringUtility.floatToString(progress * 100.0f, 3) + "%");
			}
			// 更新完成,所有资源与服务器上同步完成
			else if (type == CORE_EVENT_TYPE.CET_UPDATE_DONE)
			{
				mBusyInfo.Hide();
				finishDownload();
				mOKDialog.setInfo("更新完毕!");
				mOKDialog.ShowDialog();
				downloadDone(false);
			}
			// 列表文件下载完毕,开始对比本地文件
			else if (type == CORE_EVENT_TYPE.CET_START_GENERATE_LOCAL_FILE)
			{
				notifyStartGenerateLocalFile();
				mBusyInfo.Show();
				mBusyInfo.setInfo("正在检查本地文件...0/" + param[0]);
			}
			// 正在对比本地文件
			else if (type == CORE_EVENT_TYPE.CET_GENERATING_LOCAL_FILE)
			{
				mBusyInfo.setInfo("正在检查本地文件..." + param[1] + "/" + param[0]);
			}
			// 文件对比完成
			else if (type == CORE_EVENT_TYPE.CET_FINISH_GENERATE_LOCAL_FILE)
			{
				notifyFinishGenerateLocalFile();
				mBusyInfo.Hide();
			}
			// 取消下载更新
			else if (type == CORE_EVENT_TYPE.CET_CANCEL_UPDATE)
			{
				downloadDone(false);
			}
			// 对比所有文件后未发现需要更新的文件
			else if (type == CORE_EVENT_TYPE.CET_NOTHING_UPDATE)
			{
				notifyNothingUpdate();
				mOKDialog.setInfo("没有需要更新的文件!");
				mOKDialog.ShowDialog();
				downloadDone(false);
			}
			// 有文件上传完毕
			else if (type == CORE_EVENT_TYPE.CET_FINISH_UPLOAD)
			{
				;
			}
			// 所有文件上传完毕
			else if (type == CORE_EVENT_TYPE.CET_ALL_UPLOADED)
			{
				uploadDone();
			}
		}
		public void uploadDone()
		{
			// 上传完毕后如果自动关闭,则关闭程序
			if (mAutoClose)
			{
				mRunning = false;
			}
			// 不自动关闭,则开始下载版本文件判断是否需要更新
			else
			{
				EditorBase.mDownloadManager.requestDownloadVersion();
			}
		}
		public void downloadDone(bool launch)
		{
			mRunning = false;
			if (launch)
			{
				mUpdateDone = true;
				if (mAutoStartGame)
				{
					// 显示提示
					mBusyInfo.Show();
					mBusyInfo.setInfo("正在启动游戏...");
					// 启动游戏
					SystemUtility.launchExecutable(CommonDefine.GAME_NAME);
				}
			}
		}
		public void start(List<FileUploadInfo> uploadFileList)
		{
			if (uploadFileList.Count > 0)
			{
				// 请求开始上传
				EditorBase.mUploadManager.uploadFile(uploadFileList);
			}
			else
			{
				uploadDone();
			}
		}
		public void startDownload()
		{
			FileWrap file = EditorBase.mDownloadManager.getCurDownloadingFile();
			mCurDownloadingLabel.Content = "_" + file.getFileName();
			mCurFileProgress.Value = mCurFileProgress.Minimum;
		}
		public void finishDownload()
		{
			updateState();
			mCancelButton.Content = "完成";
		}
		public void notifyNothingUpdate()
		{
			mCancelButton.Content = "退出";
		}
		public void notifyStartGenerateLocalFile()
		{
			mCancelButton.IsEnabled = false;
		}
		public void notifyFinishGenerateLocalFile()
		{
			mCancelButton.IsEnabled = true;
		}
		public void updateState()
		{
			DownloadManager manager = EditorBase.mDownloadManager;
			FileWrap file = manager.getCurDownloadingFile();
			if (file != null && file.getFileName() != CommonDefine.VERSION && file.getFileName() != CommonDefine.FILELIST)
			{
				// 当前进度
				float percent = file.getTotalSize() > 0 ? (float)file.getCurSize() / (float)file.getTotalSize() : 0.0f;
				mCurFileProgress.Value = percent * mCurFileProgress.Maximum;
				mCurFilePercent.Content = StringUtility.floatToString((float)mCurFileProgress.Value, 2) + "%"; 
				float curSize = file.getCurSize() / 1024.0f;
				float totalSize = file.getTotalSize() / 1024.0f;
				mCurSizeLabel.Content = StringUtility.floatToString(MathUtility.KBToMBOrG(curSize), 3) + StringUtility.unitConversion(curSize) + "/" + StringUtility.floatToString(MathUtility.KBToMBOrG(totalSize), 3) + StringUtility.unitConversion(totalSize);
				// 总文件体积
				float downLoadTotalSize = manager.getTotalSize() / 1024.0f;
				mTotalSizeLabel.Content = StringUtility.floatToString(MathUtility.KBToMBOrG(downLoadTotalSize), 3) + StringUtility.unitConversion(downLoadTotalSize);
				// 当前下载速度
				float curDownLoadSpeed = manager.getCurSpeed() / 1024.0f;
				mCurSpeedLabel.Content = StringUtility.floatToString(MathUtility.KBToMBOrG(curDownLoadSpeed), 3) + StringUtility.unitConversion(curDownLoadSpeed) + "/S";
				// 总进度
				float totalPercent = (float)manager.getDownloadedSize() / manager.getTotalSize();
				mTotalProgress.Value = totalPercent * mTotalProgress.Maximum;
				mTotalFileCountText.Content = StringUtility.intToString(manager.getDownloadedCount()) + "/" + StringUtility.intToString(manager.getTotalCount());
				mTotalPercent.Content = StringUtility.floatToString((float)mTotalProgress.Value, 2) + "%";
				// 预计剩余时间
				int hours = 0, minutes = 0, seconds = 0;
				MathUtility.secondsToHoursMinutesSeconds((int)manager.getRemainTime(), ref hours, ref minutes, ref seconds);
				string timeString = StringUtility.intToString(hours, 2) + "小时" + StringUtility.intToString(minutes, 2) + "分" + StringUtility.intToString(seconds, 2) + "秒";
				mTotalTimeLabel.Content = timeString;
			}
		}
		private void cancelButton_Click(object sender, RoutedEventArgs e)
		{
			mEditorCore.getSystem<DownloadManager>().setCancel(true);
		}
		private void minButton_Click(object sender, RoutedEventArgs e)
		{
			this.WindowState = WindowState.Minimized; 
		}
		private void closeButton_Click(object sender, RoutedEventArgs e)
		{
			this.Close();
		}
		private void windowMove_Click(object sender, MouseEventArgs e)
		{
			if (e.LeftButton == MouseButtonState.Pressed)
			{
				this.DragMove();
			}
		}
	}
}
