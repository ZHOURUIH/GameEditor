#ifndef _TX_ACTION_FACTORY_H_
#define _TX_ACTION_FACTORY_H_

#include "Utility.h"
#include "txAction.h"
#include "txEngineLog.h"

class txActionFactoryBase
{
public:
	txActionFactoryBase(const std::string& type)
		:
		mType(type),
		mNameCount(0)
	{}
	virtual txAction* createAction(const std::string& name) = 0;
	template <typename T>
	static txActionFactoryBase* createFactory(const std::string& type)
	{
		T* newFacotry = TRACE_NEW(T, newFacotry, type);
		return newFacotry;
	}
	const std::string& getType() { return mType; }
	std::string createUniqueName()
	{
		return mType + txStringUtility::intToString(mNameCount++);
	}
protected:
	std::string mType;
	int mNameCount;	// 用于生成唯一名字
};

template<class T>
class txActionFactory : public txActionFactoryBase
{
public:
	txActionFactory(const std::string& type)
		:
		txActionFactoryBase(type)
	{}
	txAction* createAction(const std::string& name)
	{
		T* action = NULL;
		if (name == EMPTY_STRING)
		{
			action = TRACE_NEW(T, action, mType, createUniqueName());
		}
		else
		{
			action = TRACE_NEW(T, action, mType, name);
		}
		return action;
	}
};

#endif