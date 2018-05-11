#include "Utility.h"
#include "txMovableObject.h"
#include "txComponentHeader.h"

#include "CameraLinker.h"
#include "GameCamera.h"
#include "CameraLinkerSwitchLinear.h"
#include "CameraLinkerSwitchCircle.h"
#include "CameraLinkerSwitchAroundTarget.h"

void CameraLinker::update(float elapsedTime)
{
	if (mLinkObject != NULL)
	{
		if (mCurSwitch != NULL)
		{
			mCurSwitch->update(elapsedTime);
		}
	}
	txComponent::update(elapsedTime);
}

void CameraLinker::initSwitch()
{
	CameraLinkerSwitchLinear* lineSwitch = TRACE_NEW(CameraLinkerSwitchLinear, lineSwitch, CLS_LINEAR, this);
	mSwitchList.insert(lineSwitch->getType(), lineSwitch);
	CameraLinkerSwitchCircle* circleSwitch = TRACE_NEW(CameraLinkerSwitchCircle, circleSwitch, CLS_CIRCLE, this);
	mSwitchList.insert(circleSwitch->getType(), circleSwitch);
	CameraLinkerSwitchAroundTarget* roateCharacter = TRACE_NEW(CameraLinkerSwitchAroundTarget, roateCharacter, CLS_AROUND_TARGET, this);
	mSwitchList.insert(roateCharacter->getType(), roateCharacter);
}

void CameraLinker::destroySwitch()
{
	if (mSwitchList.size() == 0)
	{
		return;
	}
	auto iter = mSwitchList.begin();
	auto iterEnd = mSwitchList.end();
	FOR_STL(mSwitchList, ; iter != iterEnd; ++iter)
	{
		TRACE_DELETE(iter->second);
	}
	END_FOR_STL(mSwitchList);
	mSwitchList.clear();
	mCurSwitch = NULL;
}

CameraLinkerSwitch* CameraLinker::getSwitch(const CAMERA_LINKER_SWITCH& type)
{
	auto iter = mSwitchList.find(type);
	if (iter != mSwitchList.end())
	{
		return iter->second;
	}
	return NULL;
}

void CameraLinker::setRelativePosition(const VECTOR3& pos, const CAMERA_LINKER_SWITCH& switchType, const bool& useDefaultSwitchSpeed, const float& switchSpeed)
{
	// 如果不使用转换器,则直接设置位置
	if (switchType == CLS_NONE)
	{
		mRelativePosition = pos;
	}
	// 如果使用转换器,则查找相应的转换器,设置参数
	else
	{
		mCurSwitch = getSwitch(switchType);
		// 找不到则直接设置位置
		if (mCurSwitch == NULL)
		{
			mRelativePosition = pos;
		}
		else
		{
			// 如果不适用默认速度,其实是转换器当前的速度,则设置新的速度
			if (!useDefaultSwitchSpeed)
			{
				mCurSwitch->init(mRelativePosition, pos, switchSpeed);
			}
			// 否则就将转换器当前的速度设置进去
			else
			{
				mCurSwitch->init(mRelativePosition, pos, mCurSwitch->getSwitchSpeed());
			}
		}
	}
}

void CameraLinker::lateUpdate(float elapsedTime)
{
	if (mLinkObject != NULL)
	{
		GameCamera* camera = static_cast<GameCamera*>(mComponentOwner);
		if (mLookatTarget)
		{
			//让摄像机朝向
			camera->lookAt(mLinkObject->getPosition() + mLookatOffset);
		}
		if (mUseTargetRotation)
		{
			camera->setRotation(mLinkObject->getRotation());
		}
	}
	txComponent::lateUpdate(elapsedTime);
}