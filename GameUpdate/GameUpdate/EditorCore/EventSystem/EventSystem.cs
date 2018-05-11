using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

public class EventBuffer
{
	public CORE_EVENT_TYPE mType;
	public List<string> mParams;
}

public class EventSystem : FrameComponent
{
	protected Dictionary<CORE_EVENT_TYPE, List<EventHandler>> mEventHandlerList;
	protected List<EventBuffer> mEventBuffer;
	protected Dictionary<CORE_EVENT_TYPE, int> mEventParamCountList; // 每个事件的参数列表长度
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
		if (mEventHandlerList.ContainsKey(evt))
		{
			mEventHandlerList[evt].Remove(handler);
		}
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
		if(!mEventHandlerList.ContainsKey(evt))
		{
			List<EventHandler> handlerList = new List<EventHandler>();
			mEventHandlerList.Add(evt, handlerList);
		}
		mEventHandlerList[evt].Add(handler);
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
		mEventParamCountList.Add(CORE_EVENT_TYPE.CET_ERROR_LOG, 1);
		mEventParamCountList.Add(CORE_EVENT_TYPE.CET_INFO_LOG, 1);
		mEventParamCountList.Add(CORE_EVENT_TYPE.CET_START_DOWNLOAD_VERSION, 0);
		mEventParamCountList.Add(CORE_EVENT_TYPE.CET_START_DOWNLOAD_LIST_FILE, 0);
		mEventParamCountList.Add(CORE_EVENT_TYPE.CET_DOWNLOADING_LIST_FILE, 1);
		mEventParamCountList.Add(CORE_EVENT_TYPE.CET_NEW_VERSION, 1);
		mEventParamCountList.Add(CORE_EVENT_TYPE.CET_START_DOWNLOAD, 1);
		mEventParamCountList.Add(CORE_EVENT_TYPE.CET_FINISH_DOWNLOAD, 1);
		mEventParamCountList.Add(CORE_EVENT_TYPE.CET_START_GENERATE_LOCAL_FILE, 1);
		mEventParamCountList.Add(CORE_EVENT_TYPE.CET_GENERATING_LOCAL_FILE, 2);
		mEventParamCountList.Add(CORE_EVENT_TYPE.CET_FINISH_GENERATE_LOCAL_FILE, 0);
		mEventParamCountList.Add(CORE_EVENT_TYPE.CET_UPDATING_FILE, 0);
		mEventParamCountList.Add(CORE_EVENT_TYPE.CET_UPDATE_DONE, 0);
		mEventParamCountList.Add(CORE_EVENT_TYPE.CET_CANCEL_UPDATE, 0);
		mEventParamCountList.Add(CORE_EVENT_TYPE.CET_NOTHING_UPDATE, 0);
		mEventParamCountList.Add(CORE_EVENT_TYPE.CET_START_UPLOAD, 1);
		mEventParamCountList.Add(CORE_EVENT_TYPE.CET_UPLOADING_FILE, 1);
		mEventParamCountList.Add(CORE_EVENT_TYPE.CET_FINISH_UPLOAD, 2);
		mEventParamCountList.Add(CORE_EVENT_TYPE.CET_ALL_UPLOADED, 0);
		mEventParamCountList.Add(CORE_EVENT_TYPE.CET_STOP_PROGRAM, 0);
		mEventParamCountList.Add(CORE_EVENT_TYPE.CET_UPDATING_PROGRESS, 1);
		if (mEventParamCountList.Count < (int)CORE_EVENT_TYPE.CET_MAX)
		{
			logError("error : all events must be assigned parameter count! cur count : " + mEventParamCountList.Count + ", max count : " + CORE_EVENT_TYPE.CET_MAX);
		}
	}
}