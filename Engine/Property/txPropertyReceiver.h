#ifndef _TX_PROPERTY_RECEIVER_H_
#define _TX_PROPERTY_RECEIVER_H_

#include "txEngineDefine.h"

class txProperty;
class txPropertyManager;
class txPropertyReceiver
{
public:
	txPropertyReceiver(){}
	virtual ~txPropertyReceiver()
	{
		mPropertyMap.clear();
		mPropertyList.clear();
	}
	virtual void initReceiver();
	virtual void initProperty() = 0;
	txProperty* getProperty(const std::string& propertyName);
	template<typename T>
	T* getProperty()
	{
		return static_cast<T*>(getProperty(T::getPropertyName<T>()));
	}
	void setPropertyValue(txProperty* prop, const std::string& value);
	void setPropertyValue(const std::string& propertyName, const std::string& propertyValue);
	txMap<std::string, txProperty*>& getPropertyMap() { return mPropertyMap; }
	txVector<txProperty*>& getPropertyList() { return mPropertyList; }
	// 通知所有的属性已经设置完成
	virtual void notifyPropertyDone(){}
protected:
	template<typename T>
	void addProperty()
	{
		static T tempProperty;
		addPropertyToList(tempProperty.getName(), tempProperty.getReceiverType());
	}
	bool addPropertyToList(const std::string& propertyName, const std::string& receiverType);
protected:
	txMap<std::string, txProperty*> mPropertyMap;
	txVector<txProperty*> mPropertyList;
};

#endif