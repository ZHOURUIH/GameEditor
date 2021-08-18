using System;
using System.Collections.Generic;

public class EventBuffer
{
	public CORE_EVENT_TYPE mType;
	public List<string> mParams;
}

public class EventSystem : FrameComponent
{
	protected Dictionary<CORE_EVENT_TYPE, List<EventHandler>> mEventHandlerList;
	protected Dictionary<CORE_EVENT_TYPE, int> mEventParamCountList; // 每个事件的参数列表长度
	protected List<EventBuffer> mEventBuffer;
	protected ThreadLock mEventLock;
	public EventSystem(string name)
	:base(name)
	{
		mEventHandlerList = new Dictionary<CORE_EVENT_TYPE, List<EventHandler>>();
		mEventBuffer = new List<EventBuffer>();
		mEventParamCountList = new Dictionary<CORE_EVENT_TYPE, int>();
		mEventLock = new ThreadLock();
	}
	public override void init()
	{
		base.init();
		registeAllEventParam();
	}
	public override void destroy()
	{
		mEventHandlerList.Clear();
		mEventBuffer.Clear();
		mEventParamCountList.Clear();
		mEventLock = null;
		base.destroy();
	}
	public override void update(float elapsedTime)
	{
		mEventLock.waitForUnlock();
		int bufferSize = mEventBuffer.Count;
		if (bufferSize == 0)
		{
			mEventLock.unlock();
			return;
		}
		// 为了避免在多线程中出现问题,比如事件发出后还没有清空事件,就再次被更新,复制一个事件列表,然后再发送事件
		List<EventBuffer> eventList = new List<EventBuffer>(mEventBuffer);
		mEventBuffer.Clear();
		mEventLock.unlock();
		// 将事件缓冲里的事件发送出去
		for (int i = 0; i < bufferSize; ++i)
		{
			pushEvent(eventList[i].mType, eventList[i].mParams, true);
		}
	}
	public void unregisteEventHandler(CORE_EVENT_TYPE evt, EventHandler handler)
	{
		mEventHandlerList.TryGetValue(evt, out List<EventHandler> list);
		list?.Remove(handler);
	}
	public void unregisteAllEvent(EventHandler handler)
	{
		foreach(var item in mEventParamCountList)
		{
			unregisteEventHandler(item.Key, handler);
		}
	}
	public void registeAllEvent(EventHandler handler)
	{
		foreach (var item in mEventParamCountList)
		{
			registeEventHandler(item.Key, handler);
		}
	}
	public void registeEventHandler(CORE_EVENT_TYPE evt, EventHandler handler)
	{
		if(!mEventHandlerList.TryGetValue(evt, out List<EventHandler> list))
		{
			list = new List<EventHandler>();
			mEventHandlerList.Add(evt, list);
		}
		list.Add(handler);
	}
	public void pushEvent(CORE_EVENT_TYPE evt, List<string> paramList, bool sendImmediately = true)
	{
		// 如果立即发送事件,则遍历事件列表,将事件发给每个事件处理者
		if (sendImmediately)
		{
			if(!mEventParamCountList.ContainsKey(evt))
			{
				return;
			}
			
			int eventParamCount = mEventParamCountList[evt];
			if (mEventHandlerList.ContainsKey(evt))
			{
				foreach(var item in mEventHandlerList[evt])
				{
					if ((eventParamCount == 0 && paramList == null) || (eventParamCount == paramList.Count))
					{
						item.notifyEvent(evt, paramList);
					}
					else
					{
						logError("error : param list size error! should have param count : " + eventParamCount + ", cur param count : " + paramList.Count + ", event : %d" + evt);
					}
				}
			}
		}
		// 如果不立即发送事件,则将事件放入事件缓冲里面
		else
		{
			mEventLock.waitForUnlock();
			EventBuffer buffer = new EventBuffer();
			buffer.mType = evt;
			buffer.mParams = paramList;
			mEventBuffer.Add(buffer);
			mEventLock.unlock();
		}
	}
	protected void registeAllEventParam()
	{
		mEventParamCountList.Add(CORE_EVENT_TYPE.ERROR_LOG, 1);
		mEventParamCountList.Add(CORE_EVENT_TYPE.INFO_LOG, 1);
		mEventParamCountList.Add(CORE_EVENT_TYPE.START_DOWNLOAD_LIST_FILE, 0);
		mEventParamCountList.Add(CORE_EVENT_TYPE.DOWNLOADING_LIST_FILE, 1);
		mEventParamCountList.Add(CORE_EVENT_TYPE.START_DOWNLOAD, 1);
		mEventParamCountList.Add(CORE_EVENT_TYPE.FINISH_DOWNLOAD, 1);
		mEventParamCountList.Add(CORE_EVENT_TYPE.START_GENERATE_LOCAL_FILE, 1);
		mEventParamCountList.Add(CORE_EVENT_TYPE.GENERATING_LOCAL_FILE, 2);
		mEventParamCountList.Add(CORE_EVENT_TYPE.FINISH_GENERATE_LOCAL_FILE, 0);
		mEventParamCountList.Add(CORE_EVENT_TYPE.UPDATING_FILE, 0);
		mEventParamCountList.Add(CORE_EVENT_TYPE.UPDATE_DONE, 0);
		mEventParamCountList.Add(CORE_EVENT_TYPE.CANCEL_UPDATE, 0);
		mEventParamCountList.Add(CORE_EVENT_TYPE.NOTHING_UPDATE, 0);
		mEventParamCountList.Add(CORE_EVENT_TYPE.UPDATING_PROGRESS, 1);
		if (mEventParamCountList.Count < (int)CORE_EVENT_TYPE.MAX)
		{
			logError("error : all events must be assigned parameter count! cur count : " + mEventParamCountList.Count + ", max count : " + CORE_EVENT_TYPE.MAX);
		}
	}
}