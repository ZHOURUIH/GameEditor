#include "LayoutScriptAutoHide.h"
#include "Layout.h"
#include "LayoutTool.h"

void LayoutScriptAutoHide::update(const float& elapsedTime)
{
	LayoutScript::update(elapsedTime);
	// 已经显示完成,并且需要自动隐藏时才会计时
	if (mShowDone && mCurTime >= 0.0f)
	{
		mCurTime += elapsedTime;
		if (mCurTime >= mAutoHideTime)
		{
			LayoutTool::HIDE_LAYOUT(mLayout->getLayoutID());
		}
	}
}

void LayoutScriptAutoHide::hideDone()
{
	mHideDone = true;
	mShowDone = false;
	LayoutTool::HIDE_LAYOUT_FORCE(mLayout->getLayoutID());
}