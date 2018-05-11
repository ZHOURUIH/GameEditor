#include "txComponentProperty.h"

//---------------------------------------------------------------------------------------------------------------------------------------------
txComponentPropertyActive::txComponentPropertyActive()
{
	mPropertyName = "Active";
	mDescription = "该组件是否激活";
}
void txComponentPropertyActive::setRealValue(const bool& value, txComponent* component)
{
	component->setActive(value);
}
bool txComponentPropertyActive::getRealValue(txComponent* component)
{
	return component->isActive();
}