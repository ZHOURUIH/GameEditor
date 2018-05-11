#include "txComponentFactoryManager.h"
#include "txComponentFactory.h"
#include "txComponentHeader.h"

void txComponentFactoryManager::init()
{
	// 基本组件类型
	ADD_COMPONENT_FACTORY(txComponentAudio);
	ADD_COMPONENT_FACTORY(txComponentCollisionBox);
	ADD_COMPONENT_FACTORY(txComponentCollisionCapsule);
	ADD_COMPONENT_FACTORY(txComponentCollisionCylinder);
	ADD_COMPONENT_FACTORY(txComponentCollisionPlane);
	ADD_COMPONENT_FACTORY(txComponentCollisionSphere);
	ADD_COMPONENT_FACTORY(txComponentEffect);
	ADD_COMPONENT_FACTORY(txComponentAlpha);
	ADD_COMPONENT_FACTORY(txComponentMove);
	ADD_COMPONENT_FACTORY(txComponentPhysics);
	ADD_COMPONENT_FACTORY(txComponentRotateSpeed);
	ADD_COMPONENT_FACTORY(txComponentRotate);
	ADD_COMPONENT_FACTORY(txComponentScale);
	ADD_COMPONENT_FACTORY(txComponentTransform);
	ADD_COMPONENT_FACTORY(txComponentStaticMesh);
	ADD_COMPONENT_FACTORY(txComponentDynamicMesh);
	ADD_COMPONENT_FACTORY(WindowComponentAlpha);
	ADD_COMPONENT_FACTORY(WindowComponentColour);
	ADD_COMPONENT_FACTORY(WindowComponentHSL);
	ADD_COMPONENT_FACTORY(WindowComponentMove);
	ADD_COMPONENT_FACTORY(WindowComponentRotateSpeed);
	ADD_COMPONENT_FACTORY(WindowComponentRotate);
	ADD_COMPONENT_FACTORY(WindowComponentScale);
	ADD_COMPONENT_FACTORY(WindowComponentSelffitScale);
	ADD_COMPONENT_FACTORY(WindowComponentMoveRelative);
	ADD_COMPONENT_FACTORY(CameraLinkerAcceleration);
	ADD_COMPONENT_FACTORY(CameraLinkerFixed);
	ADD_COMPONENT_FACTORY(CameraLinkerFixedTime);
	ADD_COMPONENT_FACTORY(CameraLinkerSpring);
	ADD_COMPONENT_FACTORY(CameraTrembling);
}