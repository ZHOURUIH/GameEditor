#ifndef _TX_FACTORY_MANAGER_H_
#define _TX_FACTORY_MANAGER_H_

#include "Utility.h"

template<typename Key, typename FactoryBase>
class txFactoryManager
{
public:
	txFactoryManager(){}
	virtual ~txFactoryManager(){destory();}
	virtual void init() = 0;
	virtual void destory()
	{
		auto iter = mFactoryList.begin();
		auto iterEnd = mFactoryList.end();
		FOR_STL(mFactoryList, ; iter != iterEnd; ++iter)
		{
			TRACE_DELETE(iter->second);
		}
		END_FOR_STL(mFactoryList);
		mFactoryList.clear();
	}
	FactoryBase* getFactory(const Key& type)
	{
		auto iter = mFactoryList.find(type);
		if (iter != mFactoryList.end())
		{
			return iter->second;
		}
		return NULL;
	}
	txMap<Key, FactoryBase*>& getFactoryList() { return mFactoryList; }
	template<typename O>
	FactoryBase* addFactory(const Key& type)
	{
		FactoryBase* factory = FactoryBase::createFactory<O>(type);
		mFactoryList.insert(factory->getType(), factory);
		return factory;
	}
protected:
	txMap<Key, FactoryBase*> mFactoryList;
};

#endif