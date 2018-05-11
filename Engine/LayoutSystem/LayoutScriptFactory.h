#ifndef _SCRIPT_FACTORY_H_
#define _SCRIPT_FACTORY_H_

#include "LayoutScript.h"

class LayoutScriptFactoryBase
{
public:
	LayoutScriptFactoryBase(const int& type)
		:
		mType(type)
	{}
	virtual LayoutScript* createScript(Layout* layout, const std::string& name) = 0;
	template<typename T>
	static LayoutScriptFactoryBase* createFactory(const int& type)
	{
		T* factory = TRACE_NEW(T, factory, type);
		return factory;
	}
	const int& getType(){ return mType; }
protected:
	int mType;
};

template<typename T>
class LayoutScriptFactory : public LayoutScriptFactoryBase
{
public:
	LayoutScriptFactory(const int& type)
		:
		LayoutScriptFactoryBase(type)
	{}
	virtual LayoutScript* createScript(Layout* layout, const std::string& name)
	{
		LayoutScript* script = TRACE_NEW(T, script, name, layout);
		return script;
	}
protected:
};

#endif