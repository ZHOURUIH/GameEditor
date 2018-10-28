#ifndef _TX_EVENT_SYSTEM_BASE_H_
#define _TX_EVENT_SYSTEM_BASE_H_

#include "txEventHandler.h"
#include "ThreadLock.h"
#include "Utility.h"

#ifndef ADD_PARAM_COUNT
#define ADD_PARAM_COUNT(t, c) mEventParamCountList.insert(t, c);
#endif

template<typename T, class H>
class txEventSystemBase
{
public:
	txEventSystemBase(){}
	virtual ~txEventSystemBase(){}
	virtual void update(float elapsedTime)
	{
		LOCK(mEventLock);
		int bufferSize = mEventBuffer.size();
		if (bufferSize == 0)
		{
			UNLOCK(mEventLock);
			return;
		}
		// 为了避免在多线程中出现问题,比如事件发出后还没有清空事件,就再次被更新,复制一个事件列表,然后再发送事件
		auto eventList = mEventBuffer;
		mEventBuffer.clear();
		UNLOCK(mEventLock);
		// 将事件缓冲里的事件发送出去
		FOR_STL(eventList, int i = 0; i < bufferSize; ++i)
		{
			pushEvent(eventList[i].first, eventList[i].second, true);
		}
		END_FOR_STL(eventList);
	}
	void unregisteEvent(const T& event, H* handler)
	{
		auto iter = mEventHandlerList.find(event);
		if (iter != mEventHandlerList.end())
		{
			auto iterHandler = iter->second.find(handler);
			if (iterHandler != iter->second.end())
			{
				iter->second.erase(iterHandler);
			}
		}
	}
	void unregisteAllEvent(H* handler)
	{
		auto iter = mEventParamCountList.begin();
		auto iterEnd = mEventParamCountList.end();
		FOR_STL(mEventParamCountList, ; iter != iterEnd; ++iter)
		{
			unregisteEvent(iter->first, handler);
		}
		END_FOR_STL(mEventParamCountList);
	}
	void registeAllEvent(H* handler)
	{
		auto iter = mEventParamCountList.begin();
		auto iterEnd = mEventParamCountList.end();
		FOR_STL(mEventParamCountList, ; iter != iterEnd; ++iter)
		{
			registeEvent(iter->first, handler);
		}
		END_FOR_STL(mEventParamCountList);
	}
	void registeEvent(const T& event, H* handler)
	{
		auto iter = mEventHandlerList.find(event);
		if (iter != mEventHandlerList.end())
		{
			iter->second.insert(handler);
		}
		else
		{
			txSet<H*> handlerList;
			handlerList.insert(handler);
			mEventHandlerList.insert(event, handlerList);
		}
	}
	void pushEvent(const T& event, txVector<std::string>& paramList = txVector<std::string>(), const bool& sendImmediately = true)
	{
		// 如果立即发送事件,则遍历事件列表,将事件发给每个事件处理者
		if (sendImmediately)
		{
			auto iter = mEventHandlerList.find(event);
			auto iterEventDefine = mEventParamCountList.find(event);
			if (iterEventDefine == mEventParamCountList.end())
			{
				return;
			}
			int eventParamCount = iterEventDefine->second;
			if (iter != mEventHandlerList.end())
			{
				auto iterHandler = iter->second.begin();
				auto iterHandlerEnd = iter->second.end();
				FOR_STL(iter->second, ; iterHandler != iterHandlerEnd; ++iterHandler)
				{
					if (eventParamCount == (int)paramList.size())
					{
						(*iterHandler)->notifyEvent(event, paramList);
					}
					else
					{
						TOOL_CORE_ERROR("error : param list size error! should have param count : %d, cur param count : %d", eventParamCount, paramList.size());
					}
				}
				END_FOR_STL(iter->second);
			}
		}
		// 如果不立即发送事件,则将事件放入事件缓冲里面
		else
		{
			LOCK(mEventLock);
			mEventBuffer.push_back(std::pair<T, txVector<std::string> >(event, paramList));
			UNLOCK(mEventLock);
		}
	}
protected:
	txMap<T, txSet<H*> > mEventHandlerList;
	txVector<std::pair<T, txVector<std::string> > > mEventBuffer;
	txMap<T, int> mEventParamCountList; // 每个事件的参数列表长度
	ThreadLock mEventLock;
};

#endif