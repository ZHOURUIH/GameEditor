#ifndef _TX_COMPONENT_FACTORY_MANAGER_H_
#define _TX_COMPONENT_FACTORY_MANAGER_H_

#include "txFactoryManager.h"

#define ADD_COMPONENT_FACTORY(t) addFactory<txComponentFactory<t> >(TOSTRING(t));

class txComponentFactoryBase;
class txComponentFactoryManager : public txFactoryManager<std::string, txComponentFactoryBase>
{
public:
	virtual void init();
};

#endif