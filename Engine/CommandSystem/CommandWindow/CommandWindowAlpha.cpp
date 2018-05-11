#include "txTrembling.h"
#include "txTremblingManager.h"
#include "txEngineRoot.h"
#include "Utility.h"

#include "txCommandHeader.h"
#include "WindowHeader.h"
#include "WindowComponentAlpha.h"

void CommandWindowAlpha::execute()
{
	Window* window = CMD_CAST<Window*>(mReceiver);
	WindowComponentAlpha* comTrembling = static_cast<WindowComponentAlpha*>(window->getFirstComponent(TOSTRING(WindowComponentAlpha)));
	if (comTrembling != NULL)
	{
		comTrembling->setActive(true);
		comTrembling->setDoingCallback(mDoingCallBack, mDoingUserData);
		comTrembling->setDoneCallback(mDoneCallBack, mDoneUserData);
		comTrembling->setStartAlpha(mStartAlpha);
		comTrembling->setTargetAlpha(mTargetAlpha);
		comTrembling->play(mName, mLoop, mOnceLength, mTimeOffset, mFullOnce, mAmplitude);
		if (mAlphaChildren)
		{
			txVector<Window*> children;
			window->getAllChildren(children);
			int childCount = children.size();
			FOR_STL(children, int i = 0; i < childCount; ++i)
			{
				CommandWindowAlpha* cmdChildTremble = NEW_CMD(cmdChildTremble);
				cmdChildTremble->mName = mName;
				cmdChildTremble->mOnceLength = mOnceLength;
				cmdChildTremble->mTimeOffset = mTimeOffset;
				cmdChildTremble->mLoop = mLoop;
				cmdChildTremble->mAmplitude = mAmplitude;
				cmdChildTremble->mFullOnce = mFullOnce;
				cmdChildTremble->mAlphaChildren = mAlphaChildren;
				cmdChildTremble->mStartAlpha = mStartAlpha;
				cmdChildTremble->mTargetAlpha = mTargetAlpha;
				mCommandSystem->pushCommand(cmdChildTremble, children[i]);
			}
			END_FOR_STL(children);
		}
	}
}

std::string CommandWindowAlpha::showDebugInfo()
{
	COMMAND_DEBUG("name : %s, loop : %s, once length : %f, offset : %f, fullOnce : %s, amplitude : %f",
		mName.c_str(), txStringUtility::boolToString(mLoop).c_str(), mOnceLength, mTimeOffset, txStringUtility::boolToString(mFullOnce).c_str(), mAmplitude);
}