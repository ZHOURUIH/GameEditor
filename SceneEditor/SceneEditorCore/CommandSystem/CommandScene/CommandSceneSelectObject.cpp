#include "txScene.h"
#include "txEntity.h"
#include "txEngineRoot.h"

#include "CoreCommandHeader.h"
#include "ECScene.h"
#include "EditorCoreRoot.h"
#include "ECAxis.h"
#include "CameraManager.h"
#include "GameCamera.h"
#include "ECGrid.h"

void CommandSceneSelectObject::execute()
{
	ECScene* scene = CMD_CAST<ECScene*>(mReceiver);
	txObject* object = scene->getScene()->getObject(mObjectName);
	if (object == NULL || object->getType() != OT_ENTITY)
	{
		return;
	}
	txEntity* entity = static_cast<txEntity*>(object);
	if (mFocus && entity != NULL)
	{
		GameCamera* camera = mEngineRoot->getCameraManager()->getMainCamera();
		const VECTOR3& cameraPos = camera->getPosition();
		const VECTOR3& objectPos = entity->GetParent()->GetWorldPosition();
		VECTOR3 dir = txMath::normalize(objectPos - cameraPos);
		camera->setPosition(objectPos + dir * 10.0f);
		camera->lookAt(objectPos);
	}
}

std::string CommandSceneSelectObject::showDebugInfo()
{
	COMMAND_DEBUG("object name : %s, focus : %s", mObjectName.c_str(), mFocus ? "true" : "false");
}