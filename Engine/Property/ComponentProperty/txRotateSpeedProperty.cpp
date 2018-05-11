#include "txRotateSpeedProperty.h"

txRotateSpeedPropertyRotateSpeed::txRotateSpeedPropertyRotateSpeed()
{
	mPropertyName = "RotateSpeed";
	mDescription = "旋转速度";
}
void txRotateSpeedPropertyRotateSpeed::setRealValue(const VECTOR3& value, txComponentRotateSpeed* component)
{
	component->setRotateSpeed(value);
}
VECTOR3 txRotateSpeedPropertyRotateSpeed::getRealValue(txComponentRotateSpeed* component)
{
	return component->getRotateSpeed();
}

//-------------------------------------------------------------------------------------------------------------
txRotateSpeedPropertyRotateAcceleration::txRotateSpeedPropertyRotateAcceleration()
{
	mPropertyName = "RotateAcceleration";
	mDescription = "旋转加速度";
}
void txRotateSpeedPropertyRotateAcceleration::setRealValue(const VECTOR3& value, txComponentRotateSpeed* component)
{
	component->setRotateAcceleration(value);
}
VECTOR3 txRotateSpeedPropertyRotateAcceleration::getRealValue(txComponentRotateSpeed* component)
{
	return component->getRotateAcceleration();
}

//-------------------------------------------------------------------------------------------------------------
txRotateSpeedPropertyRotateSpeedPlayState::txRotateSpeedPropertyRotateSpeedPlayState()
{
	mPropertyName = "RotatSpeedPlayState";
	mDescription = "旋转状态";
}
void txRotateSpeedPropertyRotateSpeedPlayState::setRealValue(const PLAY_STATE& value, txComponentRotateSpeed* component)
{
	component->setPlayState(value);
}
PLAY_STATE txRotateSpeedPropertyRotateSpeedPlayState::getRealValue(txComponentRotateSpeed* component)
{
	return component->getPlayState();
}