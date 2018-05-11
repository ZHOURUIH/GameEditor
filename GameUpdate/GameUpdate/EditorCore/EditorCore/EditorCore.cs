using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

public class EditorCore : EditorBase
{
	protected Dictionary<string, FrameComponent> mFrameComponentMap;    // 存储框架组件,用于查找
	protected List<FrameComponent> mFrameComponentList;             // 存储框架组件,用于初始化,更新,销毁
	protected bool mInitFlag;                             // 初始化标记,为false表示还未初始化完成,由外部进行标记
	protected static EditorCore mSingleton;
	public EditorCore()
	{
		if(mSingleton != null)
		{
			return;
		}
		mSingleton = this;
		mFrameComponentMap = new Dictionary<string, FrameComponent>();
		mFrameComponentList = new List<FrameComponent>();
	}
	public static EditorCore getSingleton() { return mSingleton; }
	public void init()
	{
		registeAllFrameComponents();
		EditorBase.notifyConstructDone();
		int count = mFrameComponentList.Count;
		for (int i = 0; i < count; ++i)
		{
			mFrameComponentList[i].init();
		}
	}
	public void update(float elapsedTime)
	{
		int count = mFrameComponentList.Count;
		for (int i = 0; i < count; ++i)
		{
			mFrameComponentList[i].update(elapsedTime);
		}
	}
	public void destroy()
	{
		int count = mFrameComponentList.Count;
		for (int i = 0; i < count; ++i)
		{
			mFrameComponentList[i].destroy();
			mFrameComponentList[i] = null;
		}
		mFrameComponentList.Clear();
		mFrameComponentMap.Clear();
		EditorBase.notifyFrameDestroy();
	}
	public void notifyInitDone() { mInitFlag = true; }
	public void sendEvent(CORE_EVENT_TYPE type, string param, bool sendImmediately = true)
	{
		List<string> paramList = new List<string>();
		paramList.Add(param);
		sendEvent(type, paramList, sendImmediately);
	}
	public void sendEvent(CORE_EVENT_TYPE type, List<string> param = null, bool sendImmediately = true)
	{
		// 如果是立即发送的事件,则需要根据初始化标记判断是否应该立即发送
		if (sendImmediately)
		{
			sendImmediately = mInitFlag;
		}
		if (EditorBase.mEventSystem != null)
		{
			EditorBase.mEventSystem.pushEvent(type, param, sendImmediately);
		}
	}
	public void sendDelayEvent(CORE_EVENT_TYPE type, string param)
	{
		sendEvent(type, param, false);
	}
	public void sendDelayEvent(CORE_EVENT_TYPE type, List<string> param = null)
	{
		sendEvent(type, param, false);
	}
	public T getSystem<T>() where T : FrameComponent
	{
		string name = typeof(T).ToString();
		if (mFrameComponentMap.ContainsKey(name))
		{
			return mFrameComponentMap[name] as T;
		}
		return null;
	}
	//------------------------------------------------------------------------------------------------------------------------
	protected virtual void registeAllFrameComponents()
	{
		registeComponent<CommandSystem>();
		registeComponent<DownloadManager>();
		registeComponent<UploadManager>();
		registeComponent<HttpDownloadManager>();
		registeComponent<EventSystem>();
	}
	protected void registeComponent<T>() where T : FrameComponent
	{
		string name = typeof(T).ToString();
		T component = Activator.CreateInstance(typeof(T), name) as T;
		mFrameComponentMap.Add(name, component);
		mFrameComponentList.Add(component);
	}
}
