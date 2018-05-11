#include "txPropertyReceiver.h"
#include "txProperty.h"
#include "txPropertyManager.h"
#include "txEngineRoot.h"
#include "txEngineDefine.h"
#include "txEngineLog.h"

void txPropertyReceiver::initReceiver()
{
	initProperty();
}

txProperty* txPropertyReceiver::getProperty(const std::string& propertyName)
{
	auto iter = mPropertyMap.find(propertyName);
	if (iter == mPropertyMap.end())
	{
		ENGINE_ERROR("error : can not find property! name : %s", propertyName.c_str());
		return NULL;
	}
	return iter->second;
}

void txPropertyReceiver::setPropertyValue(txProperty* prop, const std::string& value)
{
	if (prop == NULL)
	{
		return;
	}
	prop->set(value, this);
}

void txPropertyReceiver::setPropertyValue(const std::string& propertyName, const std::string& propertyValue)
{
	setPropertyValue(getProperty(propertyName), propertyValue);
}

bool txPropertyReceiver::addPropertyToList(const std::string& propertyName, const std::string& receiverType)
{
	txProperty* prop = txEngineBase::mPropertyManager->getProperty(propertyName, receiverType);
	if (prop == NULL)
	{
		ENGINE_ERROR("error : can not get property in property manager, property name : %s", propertyName.c_str());
		return false;
	}
	if (mPropertyMap.find(prop->getName()) != mPropertyMap.end())
	{
		ENGINE_ERROR("error : there is a property! can not add it again, property name : %s", propertyName.c_str());
		return false;
	}
	mPropertyMap.insert(prop->getName(), prop);
	mPropertyList.push_back(prop);
	return true;
}