#ifndef _TX_PROPERTY_MANAGER_H_
#define _TX_PROPERTY_MANAGER_H_

#include "txEngineDefine.h"
#include "Utility.h"

class txProperty;
class txPropertyManager
{
public:
	txPropertyManager() {}
	virtual ~txPropertyManager();
	virtual void init();
	void destroy();
	const txMap<std::string, txMap<std::string, txProperty*> >& getPropertyList() { return mPropertyList; }
	txProperty* getProperty(const std::string& name, const std::string& receiverType);
	PROPERTY_TYPE getPropertyType(const std::string& name, const std::string& receiverType);
	template<typename T>
	void registeProperty()
	{
		T* prop = TRACE_NEW(T, prop);
		if (prop == NULL)
		{
			return;
		}
		if (!addPropertyToList(prop, prop->getReceiverType()))
		{
			TRACE_DELETE(prop);
		}
	}
protected:
	bool addPropertyToList(txProperty* prop, const std::string& receiverType);
protected:
	txMap<std::string, txMap<std::string, txProperty*> > mPropertyList;
};

#endif