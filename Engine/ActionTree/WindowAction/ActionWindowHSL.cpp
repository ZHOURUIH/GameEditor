#include "ActionWindowHSL.h"
#include "WindowComponentHSL.h"
#include "ActionPropertyHSL.h"
#include "LayoutTool.h"

void ActionWindowHSL::initProperty()
{
	ActionWindow::initProperty();
	addProperty<ActionWindowHSLPropertyStartHSL>();
	addProperty<ActionWindowHSLPropertyTargetHSL>();
}
void ActionWindowHSL::execute(const float& timeOffset, const bool& direction)
{
	ActionWindow::execute(timeOffset, direction);
	if (mWindow == NULL)
	{
		return;
	}
	if (direction)
	{
		LayoutTool::HSL_KEYFRAME_WINDOW_EX(mWindow, ZERO_ONE, mStartHSL, mTargetHSL, mExecuteTime, false, 
			timeOffset, mDoingCallback, mDoingUserData, hslDone, this);
	}
	else
	{
		LayoutTool::HSL_KEYFRAME_WINDOW_EX(mWindow, ZERO_ONE, mTargetHSL, mStartHSL, mExecuteTime, false, 
			timeOffset, mInverseDoingCallback, mInverseDoingUserData, inverseHSLDone, this);
	}
}

void ActionWindowHSL::hslDone(txComponentKeyFrame* component, void* userData, const bool& breakHSL, const bool& done)
{
	ActionWindowHSL* actionScale = static_cast<ActionWindowHSL*>(userData);
	if (!breakHSL)
	{
		actionScale->setActionState(AES_EXECUTE_DONE);
	}
}

void ActionWindowHSL::inverseHSLDone(txComponentKeyFrame* component, void* userData, const bool& breakHSL, const bool& done)
{
	ActionWindowHSL* actionScale = static_cast<ActionWindowHSL*>(userData);
	if (!breakHSL)
	{
		actionScale->setActionState(AES_EXECUTE_DONE);
	}
}

void ActionWindowHSL::recoverAction()
{
	ActionWindow::recoverAction();
	if (mWindow == NULL)
	{
		return;
	}
	mWindow->setHSLOffset(mStartHSL);
	WindowComponentHSL* componentHSL = static_cast<WindowComponentHSL*>(mWindow->getFirstActiveComponentByBaseType(TOSTRING(WindowComponentHSL)));
	if (componentHSL != NULL)
	{
		componentHSL->stop();
	}
}

void ActionWindowHSL::pauseAction()
{
	ActionWindow::pauseAction();
	if (mWindow == NULL)
	{
		return;
	}
	WindowComponentHSL* componentHSL = static_cast<WindowComponentHSL*>(mWindow->getFirstActiveComponentByBaseType(TOSTRING(WindowComponentHSL)));
	if (componentHSL != NULL)
	{
		componentHSL->pause();
	}
}

void ActionWindowHSL::resumeAction(const bool& direction)
{
	ActionWindow::resumeAction();
	if (mWindow == NULL)
	{
		return;
	}
	WindowComponentHSL* componentHSL = static_cast<WindowComponentHSL*>(mWindow->getFirstActiveComponentByBaseType(TOSTRING(WindowComponentHSL)));
	if (componentHSL != NULL)
	{
		if (direction)
		{
			LayoutTool::HSL_KEYFRAME_WINDOW_EX(mWindow, ZERO_ONE, mStartHSL, mTargetHSL, mExecuteTime, false, 
				componentHSL->getTremblingPercent() * mExecuteTime, mDoingCallback, mDoingUserData, hslDone, this);
		}
		else
		{
			LayoutTool::HSL_KEYFRAME_WINDOW_EX(mWindow, ZERO_ONE, mTargetHSL, mStartHSL, mExecuteTime, false, 
				(1.0f - componentHSL->getTremblingPercent()) * mExecuteTime, mInverseDoingCallback, mInverseDoingUserData, inverseHSLDone, this);
		}
	}
}

void ActionWindowHSL::stopAction()
{
	ActionWindow::stopAction();
	if (mWindow == NULL)
	{
		return;
	}
	WindowComponentHSL* componentHSL = static_cast<WindowComponentHSL*>(mWindow->getFirstActiveComponentByBaseType(TOSTRING(WindowComponentHSL)));
	if (componentHSL != NULL)
	{
		componentHSL->setActive(false);
	}
}