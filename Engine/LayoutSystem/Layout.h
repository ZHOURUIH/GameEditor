#ifndef _LAYOUT_H_
#define _LAYOUT_H_

#include "txRect.h"
#include "txEngineBase.h"

class Window;
class LayoutScript;
class Layout : public txEngineBase
{
public:
	Layout(const int& layoutID, const std::string& name);
	Layout(const int& layoutID, const std::string& name, const std::string& fileName);
	virtual ~Layout();

	virtual bool init(const bool& async, LayoutAsyncDone callback = NULL, void* userData = NULL);
	virtual void update(float elapsedTime);
	virtual void render();
	virtual void destroy();
	bool rename(const std::string& newName);
	const std::string& getName(){ return mName; }
	void setName(const std::string& name) { mName = name; }
	// 设置是否会立即隐藏,应该由布局脚本调用
	void setScriptControlHide(const bool& control) { mScriptControlHide = control; }
	const std::string& getFileName() { return mFileName; }
	// async表示该函数是否是在子线程中调用的
	bool loadLayout(const std::string& fileName, const bool& async = false, LayoutAsyncDone callback = NULL, void* userData = NULL);
	// 如果loadLayout是在多线程中调用的,则需要在加载完后在主线程中调用finishAsycLoadLayout来完成布局资源的创建
	virtual void finishAsyncLoadLayout();
	void saveLayout(std::string& layoutStream);
	Window* getRootWindow(){ return mRootWindow; }
	Window* getWindow(const std::string& windowName);
	// 创建一个独立窗口
	static Window* createIndependentWindow(const std::string& windowType, const std::string& windowName, const bool& initWindow = true);
	// 根据一个已存在的窗口创建一个独立窗口,会拷贝源窗口所有属性到创建出的窗口中,不拷贝父窗口和子窗口
	static Window* createIndependentWindow(Window* sourceWindow, const std::string& windowName, const bool& initWindow = true);
	// 创建一个属于该布局的窗口
	virtual Window* createWindow(const std::string& windowType, const std::string& windowName);
	void setVisible(const bool& visible, const bool& immediately, const std::string& param);
	void setVisibleForce(const bool& visible);
	bool isVisible();
	const txRect& getLayoutRect() { return mLayoutRect; }
	Window* getHoverWindow(const VECTOR2& point, const bool& useTransform, const bool& forceVisible, const bool& forceHandleInput);
	// 得到一个布局中所有窗口使用的资源名列表,仅文件名,不含路径
	void getUsedResourcesList(txVector<std::string>& resourceList);

	void deleteWindow(const std::string& name);
	bool notifyWindowNameChanged(const std::string& oldName, Window* window);
	void notifyWindowDestroied(const std::string& name);
	void notifyRenderWindowChanged();
	void notifyWindowDetached(const std::string& name);	// 通知布局窗口断开了与布局的联系,由窗口发出
	void notifyWindowAttached(Window* window);		// 通知布局窗口建立了与布局的联系,由窗口发出
	void notifyWindowRectChanged()	// 由Window在自身的所占区域发生变化时调用
	{
		mDirty = true;
	}
	txMap<std::string, Window*>&  getWindowList() { return mWindowList; }
	const int& getLayoutID() { return mLayoutID; }
protected:
	void generateRect();
	void setRootWindow(Window* window){ mRootWindow = window; }
	LayoutScript* createScript();
protected:
	txMap<std::string, Window*> mWindowList;
	Window* mRootWindow;
	std::string mName;
	txRect mLayoutRect;	// 布局中所有窗口所占据的矩形区域
	std::string mFileName;
	int mLayoutID;
	bool mDirty;
	bool mLoadDone;		// 是否已经加载完成
	bool mScriptControlHide;
	LayoutScript* mScript;			// 布局脚本
	bool mScriptInited;			// 脚本是否已经初始化
	LayoutAsyncDone mDoneCallback;
	void* mDoneUserData;
};

#endif