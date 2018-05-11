#ifndef _WINDOW_FACTORY_MANAGER_H_
#define _WINDOW_FACTORY_MANAGER_H_

#include "txFactoryManager.h"

class WindowFactoryBase;
class WindowFactoryManager : public txFactoryManager<WINDOW_TYPE, WindowFactoryBase>
{
public:
	virtual void init();	
};

#endif