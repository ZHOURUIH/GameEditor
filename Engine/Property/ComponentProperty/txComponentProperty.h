#ifndef _TX_COMPONENT_PROPERTY_H_
#define _TX_COMPONENT_PROPERTY_H_

#include "txBaseProperty.h"
#include "txComponentHeader.h"

DEFINE_BASE_PROPERTY(txComponent);

// ¼¤»î×´Ì¬
class txComponentPropertyActive : public  txComponentProperty<txBoolProperty, bool>
{
public:
	txComponentPropertyActive();
	virtual void setRealValue(const bool& value, txComponent* component);
	virtual bool getRealValue(txComponent* component);
};

#endif