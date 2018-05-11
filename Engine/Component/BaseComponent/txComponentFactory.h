#ifndef _TX_COMPONENT_FACTORY_BASE_H_
#define _TX_COMPONENT_FACTORY_BASE_H_

#include "Utility.h"
#include "txComponent.h"
#include "txEngineLog.h"

class txComponentFactoryBase
{
public:
	txComponentFactoryBase(const std::string& type)
		:
		mType(type),
		mCount(0),
		mNameCount(0)
	{}
	virtual ~txComponentFactoryBase()
	{
		if (mCount != 0)
		{
			ENGINE_ERROR("error : there are components left! count : %d, type : %s", mCount, mType.c_str());
		}
	}
	virtual txComponent* createComponent(const std::string& name) = 0;
	template <typename T>
	static txComponentFactoryBase* createFactory(const std::string& type)
	{
		T* newFacotry = TRACE_NEW(T, newFacotry, type);
		return newFacotry;
	}
	void destroyComponent(txComponent*& component)
	{
		if (component == NULL)
		{
			ENGINE_ERROR("error : can not destroy NULL character component!");
			return;
		}
		if (component->getType() != mType)
		{
			ENGINE_ERROR("error : destroy wrong type of character component! factory type : %s, component type : %s", mType.c_str(), component->getType().c_str());
		}
		TRACE_DELETE(component);
		--mCount;
	}
	const std::string& getType() { return mType; }
	std::string createUniqueName()
	{
		return mType + txStringUtility::intToString(mNameCount++);
	}
protected:
	std::string mType;
	int mCount;
	int mNameCount;	// 用于生成唯一名字
};

template<class T>
class txComponentFactory : public txComponentFactoryBase
{
public:
	txComponentFactory(const std::string& type)
	:
	txComponentFactoryBase(type)
	{}
	txComponent* createComponent(const std::string& name)
	{
		T* newComponent = NULL;
		if (name == EMPTY_STRING)
		{
			newComponent = TRACE_NEW(T, newComponent, mType, createUniqueName());
		}
		else
		{
			newComponent = TRACE_NEW(T, newComponent, mType, name);
		}
		++mCount;
		return newComponent;
	}
};

#endif