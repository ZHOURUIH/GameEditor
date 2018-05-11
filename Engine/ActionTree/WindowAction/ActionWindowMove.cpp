#include "ActionWindowMove.h"
#include "WindowComponentMove.h"
#include "ActionPropertyMove.h"
#include "LayoutTool.h"

void ActionWindowMove::initProperty()
{
	ActionWindow::initProperty();
	addProperty<ActionWindowMovePropertyStartPos>();
	addProperty<ActionWindowMovePropertyTargetPos>();
}
void ActionWindowMove::execute(const float& timeOffset, const bool& direction)
{
	ActionWindow::execute(timeOffset, direction);
	if (mWindow == NULL)
	{
		return;
	}
	if (direction)
	{
		LayoutTool::MOVE_KEYFRAME_WINDOW_EX(mWindow, ZERO_ONE, mStartPosition, mTargetPosition, mExecuteTime, false, 
			timeOffset, mDoingCallback, mDoingUserData, moveDone, this);
	}
	else
	{
		LayoutTool::MOVE_KEYFRAME_WINDOW_EX(mWindow, ZERO_ONE, mTargetPosition, mStartPosition, mExecuteTime, false, 
			timeOffset, mInverseDoingCallback, mInverseDoingUserData, inverseMoveDone, this);
	}
}

void ActionWindowMove::moveDone(txComponentKeyFrame* moveComponent, void* userdata, const bool& breakMove, const bool& done)
{
	ActionWindowMove* actionMove = static_cast<ActionWindowMove*>(userdata);
	if (!breakMove)
	{
		actionMove->setActionState(AES_EXECUTE_DONE);
	}
}

void ActionWindowMove::inverseMoveDone(txComponentKeyFrame* moveComponent, void* userdata, const bool& breakMove, const bool& done)
{
	ActionWindowMove* actionMove = static_cast<ActionWindowMove*>(userdata);
	if (!breakMove)
	{
		actionMove->setActionState(AES_EXECUTE_DONE);
	}
}

void ActionWindowMove::recoverAction()
{
	ActionWindow::recoverAction();
	if (mWindow == NULL)
	{
		return;
	}
	mWindow->setPosition(mStartPosition, true);
	WindowComponentMove* componentMove = static_cast<WindowComponentMove*>(mWindow->getFirstActiveComponentByBaseType(TOSTRING(WindowComponentMove)));
	if (componentMove != NULL)
	{
		componentMove->stop();
	}
}

void ActionWindowMove::pauseAction()
{
	ActionWindow::pauseAction();
	if (mWindow == NULL)
	{
		return;
	}
	WindowComponentMove* componentMove = static_cast<WindowComponentMove*>(mWindow->getFirstActiveComponentByBaseType(TOSTRING(WindowComponentMove)));
	if (componentMove != NULL)
	{
		componentMove->pause();
	}
}

void ActionWindowMove::resumeAction(const bool& direction)
{
	ActionWindow::resumeAction();
	if (mWindow == NULL)
	{
		return;
	}
	WindowComponentMove* componentMove = static_cast<WindowComponentMove*>(mWindow->getFirstActiveComponentByBaseType(TOSTRING(WindowComponentMove)));
	if (componentMove != NULL)
	{
		if (direction)
		{
			LayoutTool::MOVE_KEYFRAME_WINDOW_EX(mWindow, ZERO_ONE, mStartPosition, mTargetPosition, mExecuteTime, false, 
				componentMove->getTremblingPercent() * mExecuteTime, mDoingCallback, mDoingUserData, moveDone, this);
		}
		else
		{
			LayoutTool::MOVE_KEYFRAME_WINDOW_EX(mWindow, ZERO_ONE, mTargetPosition, mStartPosition, mExecuteTime, false, 
				(1.0f - componentMove->getTremblingPercent()) * mExecuteTime, mInverseDoingCallback, mInverseDoingUserData, inverseMoveDone, this);
		}
	}
}

void ActionWindowMove::stopAction()
{
	ActionWindow::stopAction();
	if (mWindow == NULL)
	{
		return;
	}
	WindowComponentMove* componentMove = static_cast<WindowComponentMove*>(mWindow->getFirstActiveComponentByBaseType(TOSTRING(WindowComponentMove)));
	if (componentMove != NULL)
	{
		componentMove->setActive(false);
	}
}