#ifndef _TX_EVENT_SYSTEM_BASE_H_
#define _TX_EVENT_SYSTEM_BASE_H_

#include <vector>
#include <set>
#include <map>
#include <string>

#include "txEventHandler.h"
#include "ToolCoreLog.h"

#ifndef ADD_PARAM_COUNT
#define ADD_PARAM_COUNT(t, c) mEventParamCountList.insert(std::make_pair(t, c));
#endif

template<typename T, class H>
class txEventSystemBase
{
public:
	txEventSystemBase(){}
	virtual ~txEventSystemBase(){}
	void update(float elapsedTime)
	{
		int bufferSize = mEventBuffer.size();
		if (bufferSize == 0)
		{
			return;
		}
		// 为了避免在多线程中出现问题,比如事件发出后还没有清空事件,就再次被更新,复制一个事件列表,然后再发送事件
		std::vector<std::pair<T, std::vector<std::string> > > eventList = mEventBuffer;
		mEventBuffer.clear();
		// 将事件缓冲里的事件发送出去
		for (int i = 0; i < bufferSize; ++i)
		{
			pushEvent(eventList[i].first, eventList[i].second, true);
		}
	}
	void unRegisterEventHandler(T event, H* handler)
	{
		std::map<T, std::set<H*> >::iterator iter = mEventHandlerList.find(event);
		if (iter != mEventHandlerList.end())
		{
			std::set<H*>::iterator iterHandler = iter->second.find(handler);
			if (iterHandler != iter->second.end())
			{
				iter->second.erase(iterHandler);
			}
		}
	}
	void unregisterAllEvent(H* handler)
	{
		std::map<T, int>::iterator iter = mEventParamCountList.begin();
		std::map<T, int>::iterator iterEnd = mEventParamCountList.end();
		for (; iter != iterEnd; ++iter)
		{
			unRegisterEventHandler(iter->first, handler);
		}
	}
	void registerAllEvent(H* handler)
	{
		std::map<T, int>::iterator iter = mEventParamCountList.begin();
		std::map<T, int>::iterator iterEnd = mEventParamCountList.end();
		for (; iter != iterEnd; ++iter)
		{
			registerEventHandler(iter->first, handler);
		}
	}
	void registerEventHandler(T event, H* handler)
	{
		std::map<T, std::set<H*> >::iterator iter = mEventHandlerList.find(event);
		if (iter != mEventHandlerList.end())
		{
			iter->second.insert(handler);
		}
		else
		{
			std::set<H*> handlerList;
			handlerList.insert(handler);
			mEventHandlerList.insert(std::pair<T, std::set<H*> >(event, handlerList));
		}
	}
	void pushEvent(T event, std::vector<std::string> paramList, bool sendImmediately = true)
	{
		// 如果立即发送事件,则遍历事件列表,将事件发给每个事件处理者
		if (sendImmediately)
		{
			std::map<T, std::set<H*> >::iterator iter = mEventHandlerList.find(event);
			std::map<T, int>::iterator iterEventDefine = mEventParamCountList.find(event);
			if (iterEventDefine == mEventParamCountList.end())
			{
				return;
			}
			int eventParamCount = iterEventDefine->second;
			if (iter != mEventHandlerList.end())
			{
				std::set<H*>::iterator iterHandler = iter->second.begin();
				std::set<H*>::iterator iterHandlerEnd = iter->second.end();
				for (; iterHandler != iterHandlerEnd; ++iterHandler)
				{
					if (eventParamCount == paramList.size())
					{
						(*iterHandler)->notifyEvent(event, paramList);
					}
					else
					{
						TOOL_CORE_ERROR("error : param list size error! should have param count : %d, cur param count : %d", eventParamCount, paramList.size());
					}
				}
			}
		}
		// 如果不立即发送事件,则将事件放入事件缓冲里面
		else
		{
			mEventBuffer.push_back(std::pair<T, std::vector<std::string> >(event, paramList));
		}
	}
protected:
	std::map<T, std::set<H*> > mEventHandlerList;
	std::vector<std::pair<T, std::vector<std::string> > > mEventBuffer;
	std::map<T, int> mEventParamCountList; // 每个事件的参数列表长度
};

#endif
