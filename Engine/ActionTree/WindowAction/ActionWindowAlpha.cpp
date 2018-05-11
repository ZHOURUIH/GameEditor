#include "ActionWindowAlpha.h"
#include "WindowComponentAlpha.h"
#include "ActionPropertyAlpha.h"
#include "LayoutTool.h"

void ActionWindowAlpha::initProperty()
{
	ActionWindow::initProperty();
	addProperty<ActionWindowAlphaPropertyStartAlpha>();
	addProperty<ActionWindowAlphaPropertyTargetAlpha>();
}
void ActionWindowAlpha::execute(const float& timeOffset, const bool& direction)
{
	ActionWindow::execute(timeOffset, direction);
	if (mWindow == NULL)
	{
		return;
	}
	if (direction)
	{
		LayoutTool::ALPHA_KEYFRAME_WINDOW_EX(mWindow, ZERO_ONE, mStartAlpha, mTargetAlpha, mExecuteTime, false, timeOffset, 
			mChangingCallback, mChangingData, changeDone, this);
	}
	else
	{
		LayoutTool::ALPHA_KEYFRAME_WINDOW_EX(mWindow, ZERO_ONE, mTargetAlpha, mStartAlpha, mExecuteTime, false, timeOffset, 
			mInverseChangingCallback, mInverseChangingData, inverserChangeDone, this);
	}
}

void ActionWindowAlpha::pauseAction()
{
	ActionWindow::pauseAction();
	if (mWindow == NULL)
	{
		return;
	}
	txComponentAlpha* component = static_cast<txComponentAlpha*>(mWindow->getFirstActiveComponentByBaseType(TOSTRING(txComponentAlpha)));
	if (NULL != component)
	{
		component->pause();
	}
}

void ActionWindowAlpha::resumeAction(const bool& direction)
{
	ActionWindow::resumeAction(direction);
	if (mWindow == NULL)
	{
		return;
	}
	txComponentAlpha* component = static_cast<txComponentAlpha*>(mWindow->getFirstActiveComponentByBaseType(TOSTRING(txComponentAlpha)));
	if (NULL != component)
	{
		if (direction)
		{
			LayoutTool::ALPHA_KEYFRAME_WINDOW_EX(mWindow, ZERO_ONE, mStartAlpha, mTargetAlpha, mExecuteTime, false, 
				component->getTremblingPercent() * mExecuteTime, mChangingCallback, mChangingData, changeDone, this);
		}
		else
		{
			LayoutTool::ALPHA_KEYFRAME_WINDOW_EX(mWindow, ZERO_ONE, mTargetAlpha, mStartAlpha, mExecuteTime, false, 
				(1.0f - component->getTremblingPercent()) * mExecuteTime, mInverseChangingCallback, mInverseChangingData, inverserChangeDone, this);
		}
	}
}

void ActionWindowAlpha::recoverAction()
{
	ActionWindow::recoverAction();
	if (mWindow == NULL)
	{
		return;
	}
	mWindow->setAlpha(mStartAlpha);
	txComponentAlpha* component = static_cast<txComponentAlpha*>(mWindow->getFirstActiveComponentByBaseType(TOSTRING(txComponentAlpha)));
	if (NULL != component)
	{
		component->stop();
	}
}

void ActionWindowAlpha::stopAction()
{
	ActionWindow::stopAction();
	if (mWindow == NULL)
	{
		return;
	}
	txComponentAlpha* component = static_cast<txComponentAlpha*>(mWindow->getFirstActiveComponentByBaseType(TOSTRING(txComponentAlpha)));
	if (NULL != component)
	{
		component->setActive(false);
	}
}

void ActionWindowAlpha::changeDone(txComponentKeyFrame* component, void* userData, const bool& breakLoop, const bool& done)
{
	ActionWindowAlpha* action = static_cast<ActionWindowAlpha*>(userData);
	if (!breakLoop)
	{
		action->setActionState(AES_EXECUTE_DONE);
	}
}

void ActionWindowAlpha::inverserChangeDone(txComponentKeyFrame* component, void* userData, const bool& breakLoop, const bool& done)
{
	ActionWindowAlpha* action = static_cast<ActionWindowAlpha*>(userData);
	if (!breakLoop)
	{
		action->setActionState(AES_EXECUTE_DONE);
	}
}