#ifndef _LAYOUT_EDITOR_CORE_H_
#define _LAYOUT_EDITOR_CORE_H_

#include "txEngineBase.h"
#include "CommonDefine.h"
#include "EditorCoreBase.h"

class Layout;
class Window;
class ECRenderWindowCoordinate;
class LayoutEditorCore : public EditorCoreBase, public txEngineBase
{
public:
	LayoutEditorCore();
	virtual void init();
	virtual void update(float elapsedTime);
	// 创建一个空的Layout 
	Layout* createLayout(const std::string& layoutName = "layout");
	Layout* loadLayout(const std::string& fileName);
	void closeLayout(){ closeLayout(mCurLayout); }
	void closeLayout(const std::string& layoutName);
	void closeLayout(Layout* layout);
	void destroyLayout() { destroyLayout(mCurLayout); }
	void destroyLayout(const std::string& layoutName);
	void destroyLayout(Layout* layout);
	void saveLayout();
	void editPrefab(const std::string& prefabName);
	// 将指定窗口保存为预设
	void savePrefab(const std::string& windowName);
	// 将指定窗口保存为模板
	void saveTemplate(const std::string& windowName);
	void setLayoutVisible(Layout* layout, const bool& visible);
	void setLayoutVisible(const std::string& layoutName, const bool& visible);
	// force表示是否强制设置状态,即使状态相同也要设置,并且发送状态改变的事件
	void setLayoutModified(const bool& modified, const bool& force = false, const bool& notifyEvent = true);
	const bool& isLayoutModified() { return mLayoutModified; }
	void setWireframeVisible(const bool& visible);
	void setSelectionWindowCoordinateVisible(const bool& visible);
	void setRenderWindowCoordinateVisible(const bool& visible);
	const bool& getWireframeVisible(){ return mWireframeVisible; }
	const bool& getSelectionWindowCoordinateVisible(){ return mCoordinateVisible; }
	const bool& getRenderWindowCoordinateVisible();
	void setScreenDensity(const float& density);
	void pickupAllResources(const std::string& targetPath);
	// 平移选中的窗口
	void moveSelectedWindow(const VECTOR2& moveDelta);
	// 删除窗口
	void deleteWindow(const std::string& name, const std::string& layoutName = EMPTY_STRING);
	// 通过父Layout创建Window,并添加到layout中
	Window* addWindow(Layout* parent, const std::string& typeName, const std::string& windowName = EMPTY_STRING, const int& childPos = -1);
	// 创建一个Window的子窗口
	Window* addWindow(Window* parent, const std::string& typeName, const std::string& windowName = EMPTY_STRING, const int& childPos = -1);
	// 给parent添加一个子窗口,直接将child添加到parent子窗口列表中
	// child下的所有子窗口也会一起添加或复制到parent中
	void addWindow(Layout* layout, Window* parent, Window* child, const int& childPos);
	txVector<Window*>& getSelections() { return mMultiSelections; }
	void setSelection(Window* selection);
	void setSelection(txVector<Window*>& selection);
	void reselect();
	void clearSelection(const bool& sendEvt = true);
	// 拷贝一个窗口,copyChild指定是否连子窗口一起拷贝,cutWindow指定是否剪切,剪切时强制连同子窗口一起剪切
	void copyWindow(const std::string& name, const bool& copyChild = false, const bool& cutWindow = false);
	void pasteWindow(const std::string& destParent, const int& pos = -1);
	// 如果想要改变layout的名字,则必须调用此函数,不能直接调用layout的改名
	bool renameLayout(const std::string& newName);
	// 移动窗口在父窗口中的渲染顺序
	void moveWindowPos(Window* window, const int& destPos);
	void notifyMouseEnterWindow(const std::string& windowName);
	void notifyMouseLeaveWindow(const std::string& windowName);
	Layout* getCurLayout()					{ return mCurLayout; }
	ECRenderWindowCoordinate* getRenderWindowCoordinate(){ return mRenderWindowCoordinate; }
protected:
	// 打开一个询问是否保存布局的对话框,返回值表示是否继续执行下去
	bool askSaveLayout(const std::string& info);
	void notifySelectionChanged(const bool& notifyEvent = true);
	void processKey();
protected:
	txVector<Window*>			mMultiSelections;		// 所有被选中的窗口,并且有顺序之分
	txSet<Window*>				mMultiSelSet;			// 所有被选中的窗口,仅用作判断是否选中了相同的窗口
	Window*						mCopyWindow;			// 拷贝的窗口
	Layout*						mCopySrcLayout;			// 拷贝的源窗口所属的布局
	bool						mCutWindow;				// 拷贝时是否需要删除原窗口
	std::string					mCutWindowName;			// 需要剪切的窗口名
	bool						mLayoutModified;		// 当前的布局是否改变过
	Layout*						mCurLayout;				// 当前正在编辑的布局
	bool						mWireframeVisible;		// 是不显示线框
	bool						mCoordinateVisible;		// 是否显示窗口坐标
	ECRenderWindowCoordinate*	mRenderWindowCoordinate;// 屏幕坐标系
	VECTOR4						mPreWireframeColor;		// 上一次的线框颜色
	VECTOR4						mPreCoordinateColor;	// 上一次的坐标系颜色
};

#endif