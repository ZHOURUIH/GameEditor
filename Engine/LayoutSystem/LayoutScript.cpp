#include "LayoutScript.h"
#include "Layout.h"
#include "WindowHeader.h"
#include "txCommandSystem.h"

bool LayoutScript::connectWindowEvent(const WINDOW_EVENT& event, const std::string& windowName, WindowEventCallback callback, void* userData)
{
	Window* window = mLayout->getWindow(windowName);
	if (window != NULL)
	{
		window->connectEvent(event, callback, userData);
		return true;
	}
	ENGINE_ERROR("error : can not connect window event, window name : %s, event : %d", windowName.c_str(), (int)event);
	return false;
}

void LayoutScript::notifyStartShowOrHide()
{
	auto iter = mDelayCmdList.begin();
	auto iterEnd = mDelayCmdList.end();
	FOR_STL(mDelayCmdList, ; iter != iterEnd; ++iter)
	{
		mCommandSystem->interruptCommand(*iter);
	}
	END_FOR_STL(mDelayCmdList);
	mDelayCmdList.clear();
}

void LayoutScript::addDelayCmd(txCommand* cmd)
{
	mDelayCmdList.insert(cmd->getAssignID());
	cmd->addStartCommandCallback(onCmdStarted, this);
}

void LayoutScript::interruptCommand(const int& assignID)
{
	if (mDelayCmdList.contains(assignID))
	{
		mDelayCmdList.erase(assignID);
		mCommandSystem->interruptCommand(assignID);
	}
}

void LayoutScript::onCmdStarted(txCommand* cmd, void* userdata)
{
	LayoutScript* script = static_cast<LayoutScript*>(userdata);
	if (script->mDelayCmdList.contains(cmd->getAssignID()))
	{
		script->mDelayCmdList.erase(cmd->getAssignID());
	}
	else
	{
		ENGINE_ERROR("ÃüÁîÖ´ĞĞºóÒÆ³ıÃüÁîÊ§°Ü!");
	}
}