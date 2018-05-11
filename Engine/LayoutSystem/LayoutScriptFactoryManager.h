#ifndef _SCRIPT_FACTORY_MANAGER_H_
#define _SCRIPT_FACTORY_MANAGER_H_

#include "txFactoryManager.h"

#ifndef LAYOUT_SCRIPT_FACTORY
#define LAYOUT_SCRIPT_FACTORY(C, T) addFactory<LayoutScriptFactory<C> >(T)
#endif

class LayoutScriptFactoryBase;
class LayoutScriptFactoryManager : public txFactoryManager<int, LayoutScriptFactoryBase>
{
public:
	virtual void init(){}
};

#endif