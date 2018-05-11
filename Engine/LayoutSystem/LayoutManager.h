#ifndef _LAYOUT_MANAGER_H_
#define _LAYOUT_MANAGER_H_

#include "txIPoint.h"
#include "ThreadLock.h"
#include "txCommandReceiver.h"

#include "WindowFactory.h"

class Layout;
class Window;
class LayoutManager : public txCommandReceiver
{
public:
	LayoutManager();
	virtual ~LayoutManager();

	virtual void init();
	virtual void destroy();
	virtual void update(float elapsedTime);
	virtual void render();
	virtual void processKey(const float& elapsedTime);
	// 通过布局文件创建一个Layout,如果文件名为空,则创建一个空的layout
	Layout* createLayout(const int& layoutID, const std::string& name, const int& order, const std::string& fileName = EMPTY_STRING, const bool& async = false, LayoutAsyncDone callback = NULL, void* userData = NULL);
	// 创建一个空的临时布局
	Layout* createTempLayout(const std::string& name);
	void destroyWindow(Window* window);
	void destroyLayout(const std::string& layoutName);
	void destroyLayout(const int& layoutID);
	void destroyTempLayout(const std::string& layoutName);
	Layout* getLayout(const std::string& layoutName);
	Layout* getLayout(const int& layoutID);
	Layout* getTempLayout(const std::string& layoutName);
	// force表示是否忽略窗口的不接收输入事件的属性,强制遍历所有的窗口
	Window* getHoverWindow(const VECTOR2& position, const bool& useTransform, const bool& forceVisible, const bool& forceHandleInput);
	void focusWindow(Window* window);
	void setGlobalWindowOffset(const txIPoint& offset);
	void changeText(const std::string& str);
	// 通知布局管理器某个布局的名字发生了改变,如果管理器不允许改名,则会返回false
	bool notifyLayoutRenamed(const std::string& oldName, Layout* layout);
	// 通知布局管理器某个窗口被销毁了
	void notifyWindowDestroy(Window* window)
	{
		if (mHoverWindow == window)
		{
			mHoverWindow = NULL;
		}
		if (mActiveWindow == window)
		{
			mActiveWindow = NULL;
		}
	}
	// 通知布局管理器有窗口的显示状态改变
	void notifyWindowVisibleChanged(Window* window);
	// 通知布局管理器渲染窗口宽高改变
	void notifyRenderWindowChanged();
	void clearActiveHoverWindow()
	{
		mHoverWindow = NULL;
		mActiveWindow = NULL;
	}
	const MATRIX4& getPerspectiveMatrix()
	{
		if (getDirectVertices())
		{
			return txMath::MAT4_IDENTITY;
		}
		return mPerspectiveMatrix;
	}
	const MATRIX4& getInversePerspectiveMatrix()
	{
		if (getDirectVertices())
		{
			return txMath::MAT4_IDENTITY;
		}
		return mInversePerspectiveMatrix;
	}
	const MATRIX4& getUIViewMatrix()
	{
		if (getDirectVertices())
		{
			return txMath::MAT4_IDENTITY;
		}
		return mUIViewMatrix;
	}
	const MATRIX4& getInverseUIViewMatrix()
	{
		if (getDirectVertices())
		{
			return txMath::MAT4_IDENTITY;
		}
		return mInverseUIViewMatrix;
	}
		
	// 获得成员变量
	const bool& getHoverUseTransform()							{ return mHoverUseTransform; }
	const bool& getForceVisible()								{ return mForceVisible; }
	const bool& getForceHandInput()								{ return mForceHandInput; }
	const std::string& getOnlyHoverLayoutName()					{ return mOnlyHoverLayout; }
	txMap<std::string, Layout*>& getLayoutNameList()			{ return mLayoutNameList; }
	txMap<std::string, Layout*>& getTempLayoutList()			{ return mTempLayoutList; }
	const float& getFovY()										{ return mFovY; }
	bool getDirectVertices()									{ return false; }	// 强制不直接渲染到屏幕上
	const bool& getEditorMode()									{ return mEditorMode; }
	// 设置成员变量
	void setDirectVertices(const bool& direct)					{ mDirectVertices = direct; }
	void setHoverUseTransform(const bool& use)					{ mHoverUseTransform = use; }
	void setForceVisible(const bool& force)						{ mForceVisible = force; }
	void setForceHandInput(const bool& force)					{ mForceHandInput = force; }
	void setOnlyHoverLayoutName(const std::string& name)		{ mOnlyHoverLayout = name; }
	void setHoverDirty(const bool& dirty)						{ mHoverDirty = dirty; }
	void setEditorMode(const bool& editorMode)					{ mEditorMode = editorMode; }
protected:
	void initPerspective();
	// 重新判断当前鼠标悬停的窗口
	void refreshHoverWindow(const txIPoint& curMousePos, const txIPoint& lastMousePos);
	void removeLayoutFromList(Layout* layout);
	void removeLayoutFromList(const std::string& name);
	void addLayoutToList(Layout* layout);
	int createUniqueID() { return mIDSeed++; }
protected:
	// 布局的渲染顺序,越靠前的越先渲染,不同的布局如果拥有相同的顺序,则先创建的先渲染
	txMap<int, txVector<Layout*> > mRenderOrder;		
	txMap<std::string, Layout*> mLayoutNameList;
	txMap<int, Layout*> mLayoutIDList;
	txMap<std::string, Layout*> mTempLayoutList;// 仅用作临时的布局,比如需要创建临时的窗口时所使用的布局
	Window* mHoverWindow;
	Window* mActiveWindow;
	std::string mOnlyHoverLayout;				// 只检测该布局的鼠标悬停,如果该值为空,则表示检测所有显示的布局
	MATRIX4 mPerspectiveMatrix;					// 不直接渲染到屏幕时的投影矩阵
	MATRIX4 mInversePerspectiveMatrix;			// 投影矩阵的逆矩阵
	MATRIX4 mUIViewMatrix;						// UI渲染视图矩阵
	MATRIX4 mInverseUIViewMatrix;				// UI渲染视图矩阵的逆矩阵
	float mFovY;								// 投影矩阵的fovy
	bool mDirectVertices;						// 在渲染UI时,是否直接渲染到屏幕上,为真则直接渲染到屏幕上,为假则会通过视图和投影变换然后渲染
	bool mEditorMode;							// 是否是编辑器模式
	bool mForceVisible;							// 在检测鼠标悬停时是否检测不可见的窗口
	bool mForceHandInput;						// 在检测鼠标悬停时是否检测不接收输入事件的窗口
	bool mHoverUseTransform;					// 在检测鼠标悬停时是否考虑窗口的变换矩阵
	bool mHoverDirty;							// 是否需要重新检测鼠标悬停窗口
	ThreadLock mRenderOrderLock;				// 只锁定更新渲染和创建布局
	int mIDSeed;
};

#endif