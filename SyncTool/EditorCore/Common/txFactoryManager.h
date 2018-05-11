#ifndef _TX_FACTORY_MANAGER_H_
#define _TX_FACTORY_MANAGER_H_

#include "txUtility.h"

template<typename T, typename Base>
class txFactoryManager
{
public:
	txFactoryManager(){}
	~txFactoryManager()
	{
		destory();
	}
	virtual void init() = 0;
	virtual void destory()
	{
		auto iter = mFactoryList.begin();
		auto iterEnd = mFactoryList.end();
		for (; iter != iterEnd; ++iter)
		{
			TRACE_DELETE(iter->second);
		}
		mFactoryList.clear();
	}
	Base* getFactory(T type)
	{
		auto iter = mFactoryList.find(type);
		if (iter != mFactoryList.end())
		{
			return iter->second;
		}
		return NULL;
	}
	txMap<T, Base*>& getFactoryList() { return mFactoryList; }
protected:
	template<typename O>
	Base* addFactory(T type)
	{
		Base* factory = Base::createFactory<O>(type);
		mFactoryList.insert(factory->getType(), factory);
		return factory;
	}
protected:
	txMap<T, Base*> mFactoryList;
};

#endif