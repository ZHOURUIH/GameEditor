#ifndef _TX_EVENT_SYSTEM_BASE_H_
#define _TX_EVENT_SYSTEM_BASE_H_

#include "CommonDefine.h"
#include "txEventHandler.h"
#include "EditorCoreLog.h"

#ifndef ADD_PARAM_COUNT
#define ADD_PARAM_COUNT(t, c) mEventParamCountList.insert(t, c);
#endif

template<typename T, class H>
class txEventSystemBase
{
public:
	txEventSystemBase(){}
	virtual ~txEventSystemBase(){}
	void update(const float& elapsedTime)
	{
		int bufferSize = mEventBuffer.size();
		if (bufferSize == 0)
		{
			return;
		}
		// 为了避免在多线程中出现问题,比如事件发出后还没有清空事件,就再次被更新,复制一个事件列表,然后再发送事件
		txVector<std::pair<T, txVector<std::string> > > eventList = mEventBuffer;
		mEventBuffer.clear();
		// 将事件缓冲里的事件发送出去
		for (int i = 0; i < bufferSize; ++i)
		{
			pushEvent(eventList[i].first, eventList[i].second, true);
		}
	}
	void unRegisterEventHandler(T event, H* handler)
	{
		txMap<T, txSet<H*>>::iterator iter = mEventHandlerList.find(event);
		if (iter != mEventHandlerList.end())
		{
			txSet<H*>::iterator iterHandler = iter->second.find(handler);
			if (iterHandler != iter->second.end())
			{
				iter->second.erase(iterHandler);
			}
		}
	}
	void unregisterAllEvent(H* handler)
	{
		txMap<T, int>::iterator iter = mEventParamCountList.begin();
		txMap<T, int>::iterator iterEnd = mEventParamCountList.end();
		FOR_STL(mEventParamCountList, ; iter != iterEnd; ++iter)
		{
			unRegisterEventHandler(iter->first, handler);
		}
		END_FOR_STL(mEventParamCountList);
	}
	void registerAllEvent(H* handler)
	{
		txMap<T, int>::iterator iter = mEventParamCountList.begin();
		txMap<T, int>::iterator iterEnd = mEventParamCountList.end();
		FOR_STL(mEventParamCountList, ; iter != iterEnd; ++iter)
		{
			registerEventHandler(iter->first, handler);
		}
		END_FOR_STL(mEventParamCountList);
	}
	void registerEventHandler(const T& event, H* handler)
	{
		txMap<T, txSet<H*> >::iterator iter = mEventHandlerList.find(event);
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
	void pushEvent(const T& event, const txVector<std::string>& paramList, const bool& sendImmediately = true)
	{
		// 如果立即发送事件,则遍历事件列表,将事件发给每个事件处理者
		if (sendImmediately)
		{
			txMap<T, txSet<H*> >::iterator iter = mEventHandlerList.find(event);
			txMap<T, int>::iterator iterEventDefine = mEventParamCountList.find(event);
			if (iterEventDefine == mEventParamCountList.end())
			{
				return;
			}
			int eventParamCount = iterEventDefine->second;
			if (iter != mEventHandlerList.end())
			{
				txSet<H*>::iterator iterHandler = iter->second.begin();
				txSet<H*>::iterator iterHandlerEnd = iter->second.end();
				FOR_STL(iter->second, ; iterHandler != iterHandlerEnd; ++iterHandler)
				{
					if (eventParamCount == paramList.size())
					{
						(*iterHandler)->notifyEvent(event, paramList);
					}
					else
					{
						EDITOR_CORE_ERROR("error : param list size error! should have param count : %d, cur param count : %d", eventParamCount, paramList.size());
					}
				}
				END_FOR_STL(iter->second);
			}
		}
		// 如果不立即发送事件,则将事件放入事件缓冲里面
		else
		{
			mEventBuffer.push_back(std::pair<T, txVector<std::string>>(event, paramList));
		}
	}
protected:
	txMap<T, txSet<H*>> mEventHandlerList;
	txVector<std::pair<T, txVector<std::string>>> mEventBuffer;
	txMap<T, int> mEventParamCountList; // 每个事件的参数列表长度
};

#endif
