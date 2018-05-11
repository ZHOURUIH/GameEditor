#include "ActionWindowScale.h"
#include "WindowComponentScale.h"
#include "ActionPropertyScale.h"
#include "LayoutTool.h"

void ActionWindowScale::initProperty()
{
	ActionWindow::initProperty();
	addProperty<ActionWindowScalePropertyStartScale>();
	addProperty<ActionWindowScalePropertyTargetScale>();
}
void ActionWindowScale::execute(const float& timeOffset, const bool& direction)
{
	ActionWindow::execute(timeOffset, direction);
	if (mWindow == NULL)
	{
		return;
	}
	if (direction)
	{
		LayoutTool::SCALE_KEYFRAME_WINDOW_EX(mWindow, ZERO_ONE, mStartScale, mTargetScale, mExecuteTime, false, 
			timeOffset, false, mDoingCallback, mDoingUserData, scaleDone, this);
	}
	else
	{
		LayoutTool::SCALE_KEYFRAME_WINDOW_EX(mWindow, ZERO_ONE, mTargetScale, mStartScale, mExecuteTime, false, 
			timeOffset, false, mInverseDoingCallback, mInverseDoingUserData, inverseScaleDone, this);
	}
}

void ActionWindowScale::scaleDone(txComponentKeyFrame* component, void* userData, const bool& breakScale, const bool& done)
{
	ActionWindowScale* actionScale = static_cast<ActionWindowScale*>(userData);
	if (!breakScale)
	{
		actionScale->setActionState(AES_EXECUTE_DONE);
	}
}

void ActionWindowScale::inverseScaleDone(txComponentKeyFrame* component, void* userData, const bool& breakScale, const bool& done)
{
	ActionWindowScale* actionScale = static_cast<ActionWindowScale*>(userData);
	if (!breakScale)
	{
		actionScale->setActionState(AES_EXECUTE_DONE);
	}
}

void ActionWindowScale::recoverAction()
{
	ActionWindow::recoverAction();
	if (mWindow == NULL)
	{
		return;
	}
	mWindow->setMatrixScale(mStartScale, true);
	txComponentScale* componentScale = static_cast<txComponentScale*>(mWindow->getFirstActiveComponentByBaseType(TOSTRING(txComponentScale)));
	if (componentScale != NULL)
	{
		componentScale->stop();
	}
}

void ActionWindowScale::pauseAction()
{
	ActionWindow::pauseAction();
	if (mWindow == NULL)
	{
		return;
	}
	txComponentScale* componentScale = static_cast<txComponentScale*>(mWindow->getFirstActiveComponentByBaseType(TOSTRING(txComponentScale)));
	if (componentScale != NULL)
	{
		componentScale->pause();
	}
}

void ActionWindowScale::resumeAction(const bool& direction)
{
	ActionWindow::resumeAction();
	if (mWindow == NULL)
	{
		return;
	}
	txComponentScale* componentScale = static_cast<txComponentScale*>(mWindow->getFirstActiveComponentByBaseType(TOSTRING(txComponentScale)));
	if (componentScale != NULL)
	{
		if (direction)
		{
			LayoutTool::SCALE_KEYFRAME_WINDOW_EX(mWindow, ZERO_ONE, mStartScale, mTargetScale, mExecuteTime, false, 
				componentScale->getTremblingPercent() * mExecuteTime, false, mDoingCallback, mDoingUserData, scaleDone, this);
		}
		else
		{
			LayoutTool::SCALE_KEYFRAME_WINDOW_EX(mWindow, ZERO_ONE, mTargetScale, mStartScale, mExecuteTime, false, 
				(1.0f - componentScale->getTremblingPercent()) * mExecuteTime, false, mInverseDoingCallback, mInverseDoingUserData, inverseScaleDone, this);
		}
	}
}

void ActionWindowScale::stopAction()
{
	ActionWindow::stopAction();
	if (mWindow == NULL)
	{
		return;
	}
	txComponentKeyFrame* componentScale = static_cast<txComponentKeyFrame*>(mWindow->getFirstActiveComponentByBaseType(TOSTRING(txComponentKeyFrame)));
	if (componentScale != NULL)
	{
		componentScale->setActive(false);
	}
}