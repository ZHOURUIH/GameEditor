#ifndef _LAYOUT_SCRIPT_AUTO_HIDE_H_
#define _LAYOUT_SCRIPT_AUTO_HIDE_H_

#include "LayoutScript.h"

class LayoutScriptAutoHide : public LayoutScript
{
public:
	LayoutScriptAutoHide(const std::string& name, Layout* layout)
		:
		LayoutScript(name, layout)
	{
		mShowDone = false;
		mHideDone = true;
		mAutoHide = true;
		mAutoHideTime = 3.0f;
		mCurTime = 0.0f;
	}
	virtual void onReset()
	{
		LayoutScript::onReset();
		mCurTime = 0.0f;
		mShowDone = false;
		mHideDone = true;
	}
	virtual void onShow(const bool& immediately, const std::string& param)
	{
		startShowOrHide();
		if (immediately)
		{
			showDone();
		}
	}
	virtual void onHide(const bool& immediately, const std::string& param)
	{
		startShowOrHide();
		if (immediately)
		{
			hideDone();
		}
	}
	virtual void update(const float& elapsedTime);
	void setAutoHide(const bool& autoHide)
	{
		mCurTime = autoHide ? 0.0f : -1.0f;
	}
	const bool& getShowDone(){return mShowDone;}
	const bool& getHideDone(){return mHideDone;}
	void resetHideTime()
	{
		// 只有当需要自动隐藏时,点击屏幕才会重置时间
		if (mCurTime >= 0.0f)
		{
			mCurTime = 0.0f;
		}
	}
protected:
	void startShowOrHide()
	{
		mHideDone = false;
		mShowDone = false;
	}
	void showDone()
	{
		mHideDone = false;
		mShowDone = true;
	}
	void hideDone();
protected:
	bool mShowDone;
	bool mHideDone;
	bool mAutoHide;
	float mAutoHideTime;
	float mCurTime;
};

#endif