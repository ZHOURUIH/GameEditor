using System;
using System.Collections.Generic;
using System.Windows.Threading;
using System.Windows;
using System.Windows.Input;

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
		protected BusyInfo mBusyInfo;
		protected bool mRunning;
		protected bool mUpdateDone;
		protected bool mFrameDone;
		public MainWindow()
		{
			Application.Current.ShutdownMode = ShutdownMode.OnExplicitShutdown;
			InitializeComponent();
			mFrameDone = true;
			mRunning = true;
			mUpdateDone = false;
			mOKDialog = new OKDialog();
			mBusyInfo = new BusyInfo();
			mEditorCore = new EditorCore();
			init();
		}
		public void init()
		{
			// 如果游戏已经运行则直接退出
			if (SystemUtility.isProgressRunning(GameDefine.GAME_NAME))
			{
				mOKDialog.setInfo("游戏已经运行,无法再次启动!");
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

			// 开始更新
			EditorBase.mDownloadManager.startUpdateGame();
		}
		public void destroy()
		{
			mTimer?.Stop();
			mTimer = null;
			unregisteEventHandler();
			mEditorCore.destroy();
			mEditorCore = null;
		}
		public void registeEventHandler()
		{
			EditorBase.mEventSystem?.registeAllEvent(this);
		}
		public void unregisteEventHandler()
		{
			EditorBase.mEventSystem?.unregisteAllEvent(this);
		}
		public void notifyEvent(CORE_EVENT_TYPE type, List<string> param)
		{
			if (type == CORE_EVENT_TYPE.ERROR_LOG)
			{
				mOKDialog.setTitle("错误!");
				mOKDialog.setInfo(param[0]);
				mOKDialog.ShowDialog();
			}
			else if (type == CORE_EVENT_TYPE.INFO_LOG)
			{
				mOKDialog.setTitle("提示!");
				mOKDialog.setInfo(param[0]);
				mOKDialog.ShowDialog();
			}
			else if (type == CORE_EVENT_TYPE.START_DOWNLOAD_LIST_FILE)
			{
				mBusyInfo.setInfo("正在下载列表文件...0%");
				mBusyInfo.Show();
			}
			else if(type == CORE_EVENT_TYPE.DOWNLOADING_LIST_FILE)
			{
				mBusyInfo.setInfo("正在下载列表文件..." + param[0] + "%");
				mBusyInfo.Show();
			}
			// 列表文件检查完毕,开始下载资源
			else if (type == CORE_EVENT_TYPE.START_DOWNLOAD)
			{
				startDownload();
			}
			// 所有文件下载完毕,正在更新文件
			else if (type == CORE_EVENT_TYPE.UPDATING_FILE)
			{
				mBusyInfo.Show();
				mBusyInfo.setInfo("正在更新文件...0%");
			}
			else if (type == CORE_EVENT_TYPE.UPDATING_PROGRESS)
			{
				float progress = StringUtility.SToF(param[0]);
				mBusyInfo.setInfo("正在更新文件..." + StringUtility.FToS(progress * 100.0f, 1) + "%");
			}
			// 更新完成,所有资源与服务器上同步完成
			else if (type == CORE_EVENT_TYPE.UPDATE_DONE)
			{
				mBusyInfo.Hide();
				finishDownload();
				downloadDone(true);
			}
			// 列表文件下载完毕,开始对比本地文件
			else if (type == CORE_EVENT_TYPE.START_GENERATE_LOCAL_FILE)
			{
				notifyStartGenerateLocalFile();
				mBusyInfo.Show();
				mBusyInfo.setInfo("正在检查本地文件...0/" + param[0]);
			}
			// 正在对比本地文件
			else if (type == CORE_EVENT_TYPE.GENERATING_LOCAL_FILE)
			{
				mBusyInfo.setInfo("正在检查本地文件..." + param[1] + "/" + param[0]);
			}
			// 文件对比完成
			else if (type == CORE_EVENT_TYPE.FINISH_GENERATE_LOCAL_FILE)
			{
				notifyFinishGenerateLocalFile();
				mBusyInfo.Hide();
			}
			// 取消下载更新
			else if (type == CORE_EVENT_TYPE.CANCEL_UPDATE)
			{
				downloadDone(false);
			}
			// 对比所有文件后未发现需要更新的文件
			else if (type == CORE_EVENT_TYPE.NOTHING_UPDATE)
			{
				notifyNothingUpdate();
				downloadDone(true);
			}
		}
		//-----------------------------------------------------------------------------------------------------------------------
		protected void exit()
		{
			Application.Current.Shutdown();
			destroy();
		}
		protected void Tick(object sender, EventArgs e)
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
			update((float)(now - mStartTime).TotalMilliseconds / 1000.0f);
			mStartTime = now;
			mFrameDone = true;	
		}
		protected void update(float elapsedTime)
		{
			mEditorCore.update(elapsedTime);
			DownloadManager manager = EditorBase.mDownloadManager;
			FileWrap file = manager.getCurDownloadingFile();
			if (manager.isDownloading() && file != null && file.getTotalSize() > 0 && manager.getTotalCount() > 0)
			{
				updateState();
			}
		}
		protected void downloadDone(bool launch)
		{
			mRunning = false;
			if (launch)
			{
				mUpdateDone = true;
				// 显示提示
				mBusyInfo.Show();
				mBusyInfo.setInfo("正在启动游戏...");
				// 启动游戏
				SystemUtility.launchExecutable(GameDefine.GAME_NAME);
			}
		}
		protected void startDownload()
		{
			FileWrap file = EditorBase.mDownloadManager.getCurDownloadingFile();
			mCurDownloadingLabel.Content = "_" + file.getFileName();
			mCurFileProgress.Value = mCurFileProgress.Minimum;
		}
		protected void finishDownload()
		{
			updateState();
			mCancelButton.Content = "完成";
		}
		protected void notifyNothingUpdate()
		{
			mCancelButton.Content = "退出";
		}
		protected void notifyStartGenerateLocalFile()
		{
			mCancelButton.IsEnabled = false;
		}
		protected void notifyFinishGenerateLocalFile()
		{
			mCancelButton.IsEnabled = true;
		}
		protected void updateState()
		{
			DownloadManager manager = EditorBase.mDownloadManager;
			FileWrap file = manager.getCurDownloadingFile();
			if (file != null && file.getFileName() != GameDefine.FILE_LIST)
			{
				// 当前进度
				mCurFileProgress.Value = (file.getTotalSize() > 0 ? (float)file.getCurSize() / file.getTotalSize() : 0.0f) * mCurFileProgress.Maximum;
				mCurFilePercent.Content = StringUtility.FToS((float)mCurFileProgress.Value, 1) + "%"; 
				mCurSizeLabel.Content = StringUtility.fileSizeString(file.getCurSize()) + "/" + StringUtility.fileSizeString(file.getTotalSize());
				// 总下载大小/总文件大小
				mTotalSizeLabel.Content = StringUtility.fileSizeString(manager.getDownloadedSize()) + "/" + StringUtility.fileSizeString(manager.getTotalSize());
				// 当前下载速度
				mCurSpeedLabel.Content = StringUtility.fileSizeString(manager.getCurSpeed()) + "/S";
				// 总进度
				mTotalProgress.Value = (float)manager.getDownloadedSize() / manager.getTotalSize() * mTotalProgress.Maximum;
				mTotalFileCountText.Content = StringUtility.IToS(manager.getDownloadedCount()) + "/" + StringUtility.IToS(manager.getTotalCount());
				mTotalPercent.Content = StringUtility.FToS((float)mTotalProgress.Value, 1) + "%";
				// 预计剩余时间
				MathUtility.secondsToHoursMinutesSeconds((int)manager.getRemainTime(), out int hours, out int minutes, out int seconds);
				mTotalTimeLabel.Content = StringUtility.IToS(hours, 2) + "小时" + StringUtility.IToS(minutes, 2) + "分" + StringUtility.IToS(seconds, 2) + "秒";
			}
		}
		protected void cancelButton_Click(object sender, RoutedEventArgs e)
		{
			mEditorCore.getSystem<DownloadManager>().setCancel(true);
		}
		protected void minButton_Click(object sender, RoutedEventArgs e)
		{
			WindowState = WindowState.Minimized; 
		}
		protected void closeButton_Click(object sender, RoutedEventArgs e)
		{
			Close();
		}
		protected void windowMove_Click(object sender, MouseEventArgs e)
		{
			if (e.LeftButton == MouseButtonState.Pressed)
			{
				DragMove();
			}
		}
	}
}
