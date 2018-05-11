#ifndef _COMPLEX_WINDOW_H_
#define _COMPLEX_WINDOW_H_

#include "Window.h"

// 复合窗口基类,该类窗口可以包含多个内部子窗口,从而组成一个新的窗口类型
// 内部子窗口最好只能是非复合窗口
// 内部子窗口在外部看来与父窗口是一个整体,并非真正的子窗口
class ComplexWindow : public Window
{
public:
	ComplexWindow(const std::string& name, const WINDOW_TYPE& type);
	virtual ~ComplexWindow();
	virtual void render();	// 绘制窗口
	virtual void update(const float& elapsedTime);
	void destroy();
	virtual void copyProperty(Window* window);
	virtual bool rename(const std::string& newName);
	virtual Window* getChild(const std::string& name);
	virtual void detachLayoutParent(const bool& detachLayoutOnly = false);
	// 建立与布局和父窗口的联系,使该窗口成为布局中的一个窗口,该窗口下的所有子窗口也会重建与布局的联系,父子关系仍然存在
	virtual void attachLayoutParent(Layout* layout, Window* parent, const int& childPos);
	virtual void setDirty(const bool& dirty, const bool& refreshNow);
	virtual void getUsedResourceList(txVector<std::string>& resourceList);
	virtual void removeChild(const std::string& name);
	virtual void notifyChildNameChanged(const std::string& oldName, Window* window);
	virtual void notifyInnerChildDestroied(const std::string& name);
	virtual void saveWindow(std::string& windowStream);
	virtual void saveTemplate(std::string& windowStream);
	txVector<Window*>& getInnerChildren() { return mInnerChildList; }
	virtual void getAllChildren(txVector<Window*>& children)
	{
		children = mInnerChildList;
		int childCount = mChildren.size();
		FOR_STL(mChildren, int i = 0; i < childCount; ++i)
		{
			children.push_back(mChildren[i]);
		}
		END_FOR_STL(mChildren);
	}
protected:
	void addInnerChild(Window* child);
	Window* createInnerChild(const std::string& windowType, const std::string& name);
	virtual void renameInnerChildren(const std::string& namePrefix) = 0;
	// 递归拷贝所有子窗口的属性
	void copyChildProperty(Window* sourceWindow, Window* destWindow);
protected:
	txMap<std::string, Window*> mInnerChildMap;					// 复合窗口下的第一级内部子窗口查找列表
	txVector<Window*> mInnerChildList;	// 复合窗口下的第一级内部子窗口列表
};

#endif