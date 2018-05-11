#include "txCollisionBoxProperty.h"

//---------------------------------------------------------------------------------------------------------
txCollisionBoxPropertyBoxWidth::txCollisionBoxPropertyBoxWidth()
{
	mReceiverType = TOSTRING(txComponentCollisionBox);
	mPropertyName = "BoxWidth";
	mDescription = "盒子宽度";
}
void txCollisionBoxPropertyBoxWidth::setRealValue(const float& value, txComponentCollisionBox* component)
{
	component->setWidth(value);
}
float txCollisionBoxPropertyBoxWidth::getRealValue(txComponentCollisionBox* component)
{
	return component->getWidth();
}

//---------------------------------------------------------------------------------------------------------
txCollisionBoxPropertyBoxHeight::txCollisionBoxPropertyBoxHeight()
{
	mReceiverType = TOSTRING(txComponentCollisionBox);
	mPropertyName = "BoxHeight";
	mDescription = "盒子高度";
}
void txCollisionBoxPropertyBoxHeight::setRealValue(const float& value, txComponentCollisionBox* component)
{
	component->setHeight(value);
}
float txCollisionBoxPropertyBoxHeight::getRealValue(txComponentCollisionBox* component)
{
	return component->getHeight();
}

//---------------------------------------------------------------------------------------------------------
txCollisionBoxPropertyBoxLength::txCollisionBoxPropertyBoxLength()
{
	mReceiverType = TOSTRING(txComponentCollisionBox);
	mPropertyName = "BoxLength";
	mDescription = "盒子长度";
}
void txCollisionBoxPropertyBoxLength::setRealValue(const float& value, txComponentCollisionBox* component)
{
	component->setLength(value);
}
float txCollisionBoxPropertyBoxLength::getRealValue(txComponentCollisionBox* component)
{
	return component->getLength();
}