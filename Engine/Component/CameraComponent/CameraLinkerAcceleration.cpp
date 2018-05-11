#include "txMathUtility.h"
#include "Utility.h"
#include "txSpring.h"
#include "txComponentHeader.h"

#include "CameraLinkerAcceleration.h"
#include "GameCamera.h"

CameraLinkerAcceleration::CameraLinkerAcceleration(const std::string& type, const std::string& name)
:
CameraLinker(type, name)
{
	mSpringX = TRACE_NEW(txSpring, mSpringX);
	mSpringY = TRACE_NEW(txSpring, mSpringY);
	mSpringZ = TRACE_NEW(txSpring, mSpringZ);
}

void CameraLinkerAcceleration::preUpdate(float elapsedTime)
{
	if (mLinkObject != NULL)
	{
		txComponentPhysics* physicalCom = static_cast<txComponentPhysics*>(mLinkObject->getFirstActiveComponentByBaseType(TOSTRING(txComponentPhysics)));
		// 获得加速度
		VECTOR3 acceleration = physicalCom->getAcceleration();
		VECTOR3 curRelative = static_cast<GameCamera*>(mComponentOwner)->getPosition() - mLinkObject->getPosition();
		float relativeAngle = txMath::getAngleFromVector(curRelative);
		acceleration = txMath::rotateVector3(acceleration, relativeAngle) * -1.0f;

		mSpringX->setCurLength(std::abs(curRelative.x));
		mSpringX->setForce(acceleration.x);

		mSpringY->setCurLength(std::abs(curRelative.y));
		mSpringY->setForce(acceleration.y);

		mSpringZ->setCurLength(std::abs(curRelative.z));
		mSpringZ->setForce(acceleration.z);
	}
	txComponent::preUpdate(elapsedTime);
}

void CameraLinkerAcceleration::update(float elapsedTime)
{
	if (mLinkObject != NULL)
	{
		mSpringX->update(elapsedTime);
		mSpringY->update(elapsedTime);
		mSpringZ->update(elapsedTime);

		txComponentPhysics* physicalCom = static_cast<txComponentPhysics*>(mLinkObject->getFirstActiveComponentByBaseType(TOSTRING(txComponentPhysics)));

		const VECTOR3& targetPos = mLinkObject->getPosition();
		float curX = 0.0f;
		float curY = 0.0f;
		float curZ = 0.0f;

		//判断是否为零
		if (!txMath::isFloatZero(mRelativePosition.x))
		{
			curX = mSpringX->getLength() * mRelativePosition.x / std::abs(mRelativePosition.x);
		}
		else
		{
			VECTOR3 acceleration = physicalCom->getAcceleration();
			if (!txMath::isFloatZero(acceleration.x))
			{
				curX = mSpringX->getLength() * acceleration.x / std::abs(acceleration.x);
			}
		}
		if (!txMath::isFloatZero(mRelativePosition.y))
		{
			curY = mSpringY->getLength() * mRelativePosition.y / std::abs(mRelativePosition.y);
		}
		else
		{
			VECTOR3 acceleration = physicalCom->getAcceleration();
			if (!txMath::isFloatZero(acceleration.y))
			{
				curY = mSpringY->getLength() * acceleration.y / std::abs(acceleration.y);
			}
		}
		if (!txMath::isFloatZero(mRelativePosition.z))
		{
			curZ = mSpringZ->getLength() * mRelativePosition.z / std::abs(mRelativePosition.z);
		}
		else
		{
			VECTOR3 acceleration = physicalCom->getAcceleration();
			if (!txMath::isFloatZero(acceleration.z))
			{
				curZ = mSpringZ->getLength() * acceleration.z / std::abs(acceleration.z);
			}
		}

		// 改变摄像机位置
		VECTOR3 curPos = targetPos + VECTOR3(curX, curY, curZ);
		static_cast<GameCamera*>(mComponentOwner)->setPosition(curPos);
	}
	txComponent::update(elapsedTime);
}

void CameraLinkerAcceleration::destroy()
{
	TRACE_DELETE(mSpringX);
	TRACE_DELETE(mSpringY);
	TRACE_DELETE(mSpringZ);
}

void CameraLinkerAcceleration::setRelativePosition(const VECTOR3& pos, const CAMERA_LINKER_SWITCH& switchType, const bool& useDefaultSwitchSpeed, const float& switchSpeed)
{
	CameraLinker::setRelativePosition(pos, switchType, useDefaultSwitchSpeed, switchSpeed);

	mSpringX->setNormaLength(std::abs(mRelativePosition.x));
	mSpringY->setNormaLength(std::abs(mRelativePosition.y));
	mSpringZ->setNormaLength(std::abs(mRelativePosition.z));

	mSpringX->setCurLength(std::abs(mRelativePosition.x));
	mSpringY->setCurLength(std::abs(mRelativePosition.y));
	mSpringZ->setCurLength(std::abs(mRelativePosition.z));

	mSpringX->setForce(0.0f);
	mSpringY->setForce(0.0f);
	mSpringZ->setForce(0.0f);

	mSpringX->setSpeed(0.0f);
	mSpringY->setSpeed(0.0f);
	mSpringZ->setSpeed(0.0f);
	// 改变摄像机位置
	const VECTOR3& targetPos = mLinkObject->getPosition();
	VECTOR3 curPos = targetPos + mRelativePosition;
	static_cast<GameCamera*>(mComponentOwner)->setPosition(curPos);
}