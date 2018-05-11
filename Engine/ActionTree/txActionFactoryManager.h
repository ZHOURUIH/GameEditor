#ifndef _TX_ACTION_FACTORY_MANAGER_H_
#define _TX_ACTION_FACTORY_MANAGER_H_

#include "txFactoryManager.h"

#define ADD_ACTION_FACTORY(t) addFactory<txActionFactory<t> >(TOSTRING(t));

class txActionFactoryBase;
class txActionFactoryManager : public txFactoryManager<std::string, txActionFactoryBase>
{
public:
	virtual void init();
};

#endif