#include "txMeshProperty.h"

txMeshPropertyMeshPosition::txMeshPropertyMeshPosition()
{
	mPropertyName = "MeshPosition";
	mDescription = "模型的相对位置";
}
void txMeshPropertyMeshPosition::setRealValue(const VECTOR3& value, txComponentMesh* component)
{
	component->setPosition(value);
}
VECTOR3 txMeshPropertyMeshPosition::getRealValue(txComponentMesh* component)
{
	return component->getPosition();
}

//---------------------------------------------------------------------------------------------------------------
txMeshPropertyMeshRotation::txMeshPropertyMeshRotation()
{
	mPropertyName = "MeshRotation";
	mDescription = "模型的相对旋转";
}
void txMeshPropertyMeshRotation::setRealValue(const VECTOR3& value, txComponentMesh* component)
{
	component->setRotation(txMath::eulerAngleToMatrix3(value));
}
VECTOR3 txMeshPropertyMeshRotation::getRealValue(txComponentMesh* component)
{
	return txMath::matrix3ToEulerAngle(component->getRotation());
}

//---------------------------------------------------------------------------------------------------------------
txMeshPropertyMeshScale::txMeshPropertyMeshScale()
{
	mPropertyName = "MeshScale";
	mDescription = "模型的相对缩放";
}
void txMeshPropertyMeshScale::setRealValue(const VECTOR3& value, txComponentMesh* component)
{
	component->setScale(value);
}
VECTOR3 txMeshPropertyMeshScale::getRealValue(txComponentMesh* component)
{
	return component->getScale();
}