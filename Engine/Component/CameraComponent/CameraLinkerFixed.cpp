#include "txMovableObject.h"
#include "txComponentHeader.h"

#include "CameraLinkerFixed.h"
#include "GameCamera.h"

void CameraLinkerFixed::update(float elapsedTime)
{
	// 先调用基类更新,基类更新会更新子组件,可能会引起更新顺序的错误
	CameraLinker::update(elapsedTime);
	if(mLinkObject == NULL)
	{
		return;
	}

	const VECTOR3& targetPos = mLinkObject->getPosition();//获得目标当前坐标
	VECTOR3 curPos = targetPos + mRelativePosition;
	static_cast<GameCamera*>(mComponentOwner)->setPosition(curPos);
}