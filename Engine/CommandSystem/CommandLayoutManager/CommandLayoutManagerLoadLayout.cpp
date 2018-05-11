#include "txCommandHeader.h"
#include "LayoutManager.h"
#include "Layout.h"

void CommandLayoutManagerLoadLayout::execute()
{
	LayoutManager* layoutManager = CMD_CAST<LayoutManager*>(mReceiver);
	Layout* layout = layoutManager->createLayout(mLayoutID, mName, mRenderOrder, mName + ".json", mAsync, mCallback, mUserData);
	// 加载成功,并且只有非异步加载时才能设置隐藏,因为异步加载时布局并没有完全加载完
	if (layout != NULL && !mAsync)
	{
		// 加载时设置显示状态是需要立即生效的,显示时会立即生效,隐藏时需要设置强制隐藏,不通知脚本
		if (mVisible)
		{
			layout->setVisible(mVisible, true, EMPTY_STRING);
		}
		else
		{
			layout->setVisibleForce(mVisible);
		}
	}
}

std::string CommandLayoutManagerLoadLayout::showDebugInfo()
{
	COMMAND_DEBUG("name : %s, layout id : %d, render order : %d, visible : %s", mName.c_str(), mLayoutID, mRenderOrder, txStringUtility::boolToString(mVisible).c_str());
}