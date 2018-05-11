#include "txCommand.h"

void txCommand::runEndCallBack()
{
	int callbackCount = mEndCallback.size();
	for (int i = 0; i < callbackCount; ++i)
	{
		if (mEndCallback[i].first != NULL)
		{
			mEndCallback[i].first(this, mEndCallback[i].second);
		}
	}
	mEndCallback.clear();
}
void txCommand::runStartCallBack()
{
	int callbackCount = mStartCallback.size();
	for (int i = 0; i < callbackCount; ++i)
	{
		if (mStartCallback[i].first != NULL)
		{
			mStartCallback[i].first(this, mStartCallback[i].second);
		}
	}
	mStartCallback.clear();
}