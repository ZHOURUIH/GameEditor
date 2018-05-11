#ifndef _COMMAND_LAYOUT_MANAGER_LOAD_LAYOUT_H_
#define _COMMAND_LAYOUT_MANAGER_LOAD_LAYOUT_H_

#include "txCommand.h"

// º”‘ÿLayout
class CommandLayoutManagerLoadLayout : public txCommand
{
public:
	virtual void reset()
	{
		mName = EMPTY_STRING;
		mLayoutID = -1;
		mVisible = true;
		mRenderOrder = 0;
		mAsync = false;
		mParam = EMPTY_STRING;
		mImmediatelyShow = false;
		mCallback = NULL;
		mUserData = NULL;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	std::string mName;
	int mLayoutID;
	bool mVisible;
	int mRenderOrder;
	bool mAsync;
	std::string	mParam;
	bool mImmediatelyShow;
	LayoutAsyncDone mCallback;
	void* mUserData;
};

#endif