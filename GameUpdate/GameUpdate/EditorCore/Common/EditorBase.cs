using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

public class EditorBase
{
	public static EditorCore mEditorCore;
	public static CommandSystem mCommandSystem;
	public static HttpDownloadManager mHttpDownloadManager;
	public static DownloadManager mDownloadManager;
	public static UploadManager mUploadManager;
	public static EventSystem mEventSystem;
	public static void notifyConstructDone()
	{
		if(mEditorCore == null)
		{
			mEditorCore = EditorCore.getSingleton();
			mCommandSystem = mEditorCore.getSystem<CommandSystem>();
			mHttpDownloadManager = mEditorCore.getSystem<HttpDownloadManager>();
			mDownloadManager = mEditorCore.getSystem<DownloadManager>();
			mUploadManager = mEditorCore.getSystem<UploadManager>();
			mEventSystem = mEditorCore.getSystem<EventSystem>();
		}
	}
	public static void notifyFrameDestroy()
	{
		mEditorCore = null;
		mCommandSystem = null;
		mHttpDownloadManager = null;
		mDownloadManager = null;
		mUploadManager = null;
		mEventSystem = null;
	}
	// 方便书写代码添加的命令相关函数
	public static T newCmd<T>(out T cmd, bool show = true, bool delay = false) where T : Command, new()
	{
		cmd = null;
		return mCommandSystem.newCmd<T>(show, delay);
	}
	public static void pushCommand<T>(CommandReceiver cmdReceiver, bool show = true) where T : Command, new()
	{
		mCommandSystem.pushCommand<T>(cmdReceiver, show);
	}
	public static void pushCommand(Command cmd, CommandReceiver cmdReceiver)
	{
		mCommandSystem.pushCommand(cmd, cmdReceiver);
	}
	public static void pushDelayCommand<T>(CommandReceiver cmdReceiver, float delayExecute = 0.001f, bool show = true) where T : Command, new()
	{
		mCommandSystem.pushDelayCommand<T>(cmdReceiver, delayExecute);
	}
	public static void pushDelayCommand(Command cmd, CommandReceiver cmdReceiver, float delayExecute = 0.001f)
	{
		mCommandSystem.pushDelayCommand(cmd, cmdReceiver, delayExecute);
	}
	public static void logError(string info, bool delay = false)
	{
		// 如果还没有初始化完成,则先将消息放入事件缓冲中
		List<string> paramList = new List<string>();
		paramList.Add(info);
		mEditorCore.sendEvent(CORE_EVENT_TYPE.CET_ERROR_LOG, paramList, !delay);
		logConsole(info);
	}
	public static void logInfo(string info, bool delay = false)
	{
		// 如果还没有初始化完成,则先将消息放入事件缓冲中
		List<string> paramList = new List<string>();
		paramList.Add(info);
		mEditorCore.sendEvent(CORE_EVENT_TYPE.CET_INFO_LOG, paramList, !delay);
		logConsole(info);
	}
	public static void logConsole(string info)
	{
		Console.WriteLine(info);
	}
}