#include "txMovableObject.h"
#include "txComponentHeader.h"

#include "CameraLinkerFixedTime.h"
#include "GameCamera.h"

void CameraLinkerFixedTime::update(float elapsedTime)
{
	// 先调用基类更新,基类更新会更新子组件,可能会引起更新顺序的错误
	CameraLinker::update(elapsedTime);

	if(mLinkObject == NULL)
	{
		return;
	}
	GameCamera* camera = static_cast<GameCamera*>(mComponentOwner);
	// 获得目标当前坐标
	const VECTOR3& targetPos = mLinkObject->getPosition();	
	// 摄像机的目标位置
	VECTOR3 nextPos = targetPos + mRelativePosition;
	// 得到摄像机当前位置
	const VECTOR3& cameraCurPos = camera->getPosition();
	// 计算总距离
	float totalDistance = txMath::getLength(cameraCurPos - nextPos);
	VECTOR3 cameraNewPos = cameraCurPos;
	if (totalDistance > 0.1f)
	{
		// 计算速度
		float speed = totalDistance / mTime;
		// 这一帧移动的距离
		float moveDis = speed * elapsedTime;
		// 如果距离太大以至于超过了目标点,则直接设置到目标点
		if (moveDis >= totalDistance)
		{
			cameraNewPos = nextPos;
		}
		else
		{
			VECTOR3 dir = txMath::normalize(nextPos - cameraCurPos);
			cameraNewPos = moveDis * dir + cameraCurPos;
			if (mIgnoreY)
			{
				cameraNewPos.y = nextPos.y;
			}
		}
	}	
	else
	{
		cameraNewPos = nextPos;
	}
	camera->setPosition(cameraNewPos);
}