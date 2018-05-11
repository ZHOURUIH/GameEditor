#include "ActionWindowRotate.h"
#include "WindowComponentRotate.h"
#include "ActionPropertyRotate.h"
#include "LayoutTool.h"

void ActionWindowRotate::initProperty()
{
	ActionWindow::initProperty();
	addProperty<ActionWindowRotatePropertyStartRotation>();
	addProperty<ActionWindowRotatePropertyTargetRotation>();
}
void ActionWindowRotate::execute(const float& timeOffset, const bool& direction)
{
	ActionWindow::execute(timeOffset, direction);
	if (mWindow == NULL)
	{
		return;
	}
	if (direction)
	{
		LayoutTool::ROTATE_KEYFRMAE_WINDOW_EX(mWindow, ZERO_ONE, mStartAngle, mTargetAngle, mExecuteTime, false,
			timeOffset, mDoingCallback, mDoingUserData, rotateDone, this);
	}
	else
	{
		LayoutTool::ROTATE_KEYFRMAE_WINDOW_EX(mWindow, ZERO_ONE, mTargetAngle, mStartAngle, mExecuteTime, false, 
			timeOffset, mInverseDoingCallback, mInverseDoingUserData, inverseRotateDone, this);
	}
}

void ActionWindowRotate::rotateDone(txComponentKeyFrame* component, void* userData, const bool& breakRotate, const bool& done)
{
	ActionWindowRotate* actionRotate = static_cast<ActionWindowRotate*>(userData);
	if (!breakRotate)
	{
		actionRotate->setActionState(AES_EXECUTE_DONE);
	}
}

void ActionWindowRotate::inverseRotateDone(txComponentKeyFrame* component, void* userData, const bool& breakRotate, const bool& done)
{
	ActionWindowRotate* actionRotate = static_cast<ActionWindowRotate*>(userData);
	if (!breakRotate)
	{
		actionRotate->setActionState(AES_EXECUTE_DONE);
	}
}

void ActionWindowRotate::recoverAction()
{
	ActionWindow::recoverAction();
	if (mWindow == NULL)
	{
		return;
	}
	mWindow->setMatrixRotation(txMath::eulerAngleToMatrix3(mStartAngle), true);
	WindowComponentRotate* componentRotate = static_cast<WindowComponentRotate*>(mWindow->getFirstActiveComponent(TOSTRING(WindowComponentRotate)));
	if (componentRotate != NULL)
	{
		componentRotate->stop();
	}
}

void ActionWindowRotate::pauseAction()
{
	ActionWindow::pauseAction();
	if (mWindow == NULL)
	{
		return;
	}
	WindowComponentRotate* componentRotate = static_cast<WindowComponentRotate*>(mWindow->getFirstActiveComponent(TOSTRING(WindowComponentRotate)));
	if (componentRotate != NULL)
	{
		componentRotate->pause();
	}
}

void ActionWindowRotate::resumeAction(const bool& direction)
{
	ActionWindow::resumeAction();
	if (mWindow == NULL)
	{
		return;
	}
	WindowComponentRotate* componentRotate = static_cast<WindowComponentRotate*>(mWindow->getFirstActiveComponent(TOSTRING(WindowComponentRotate)));
	if (componentRotate != NULL)
	{
		if (direction)
		{
			LayoutTool::ROTATE_KEYFRMAE_WINDOW_EX(mWindow, ZERO_ONE, mStartAngle, mTargetAngle, mExecuteTime, false, 
				componentRotate->getTremblingPercent() * mExecuteTime, mDoingCallback, mDoingUserData, rotateDone, this);
		}
		else
		{
			LayoutTool::ROTATE_KEYFRMAE_WINDOW_EX(mWindow, ZERO_ONE, mTargetAngle, mStartAngle, mExecuteTime, false, 
				(1.0f - componentRotate->getTremblingPercent()) * mExecuteTime, mInverseDoingCallback, mInverseDoingUserData, inverseRotateDone, this);
		}
	}
}

void ActionWindowRotate::stopAction()
{
	ActionWindow::stopAction();
	if (mWindow == NULL)
	{
		return;
	}
	WindowComponentRotate* componentRotate = static_cast<WindowComponentRotate*>(mWindow->getFirstActiveComponentByBaseType(TOSTRING(WindowComponentRotate)));
	if (componentRotate != NULL)
	{
		componentRotate->setActive(false);
	}
}