#ifndef _LAYOUT_SCRIPT_H_
#define _LAYOUT_SCRIPT_H_

#include "txCommandReceiver.h"
#include "txEngineBase.h"
#include "WindowHeader.h"

class Layout;
class LayoutScript : public txCommandReceiver
{
public:
	LayoutScript(const std::string& name, Layout* layout)
		:
		txCommandReceiver(name),
		mLayout(layout)
	{}
	virtual ~LayoutScript(){ destroy(); }
	virtual void assignWindow() = 0;
	virtual void init(){}
	void destroy(){}
	virtual void update(const float& elapsedTime){}
	// 在开始显示之前,需要将所有的状态都重置到加载时的状态
	virtual void onReset() { }
	// 重置布局状态后,再根据当前游戏状态设置布局显示前的状态
	virtual void onGameState() { }
	virtual void onShow(const bool& immediately, const std::string& param){}
	virtual void onHide(const bool& immediately, const std::string& param){}
	// 通知脚本开始显示或隐藏,中断全部命令
	void notifyStartShowOrHide();
	void addDelayCmd(txCommand* cmd);
	void interruptCommand(const int& assignID);
	Layout* getLayout(){ return mLayout; }
	void setRoot(Window* root) { mRoot = root; }
	Window* getRoot() { return mRoot; }
	bool connectWindowEvent(const WINDOW_EVENT& event, const std::string& windowName, WindowEventCallback callback, void* userData = NULL);
protected:
	static void onCmdStarted(txCommand* cmd, void* userdata);
protected:
	Layout* mLayout;
	Window* mRoot;
	txSet<int> mDelayCmdList;  // 布局显示和隐藏时的延迟命令列表,当命令执行时,会从列表中移除该命令
};

#endif