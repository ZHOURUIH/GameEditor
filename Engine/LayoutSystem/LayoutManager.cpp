#include "txEngineDefine.h"
#include "txEngineRoot.h"
#include "txRenderWindow.h"
#include "txPropertyManager.h"
#include "txComponentFactory.h"
#include "txComponentFactoryManager.h"
#include "txActionFactory.h"
#include "txActionFactoryManager.h"
#include "InputSystem.h"

#include "LayoutManager.h"
#include "LayoutUtility.h"
#include "WindowPrefabManager.h"
#include "WindowTemplateManager.h"
#include "WindowFactoryManager.h"
#include "WindowHeader.h"
#include "Layout.h"

LayoutManager::LayoutManager()
:
txCommandReceiver(TOSTRING(LayoutManager)),
mHoverWindow(NULL),
mActiveWindow(NULL),
mForceVisible(false),
mForceHandInput(false),
mHoverUseTransform(true),
mDirectVertices(true),
mHoverDirty(true),
mIDSeed(1000)
{}

LayoutManager::~LayoutManager()
{
	destroy();
}

void LayoutManager::init()
{
	VECTOR3 dir = -txMath::AXIS_Z;
	VECTOR3 up = txMath::AXIS_Y;
	VECTOR3 position = txMath::VEC3_ZERO;
	mUIViewMatrix = txMath::lookAt(position, position + dir, up);
	mInverseUIViewMatrix = txMath::inverse(mUIViewMatrix);
	initPerspective();
}

void LayoutManager::refreshHoverWindow(const txIPoint& curMousePos, const txIPoint& lastMousePos)
{
	VECTOR2 curPos = VECTOR2((float)curMousePos.x, (float)curMousePos.y);
	// 计算鼠标当前所在窗口
	Window* newWindow = getHoverWindow(curPos, mHoverUseTransform, mForceVisible, mForceHandInput);
	// 判断鼠标是否还在当前窗口内
	if (mHoverWindow != NULL)
	{
		// 鼠标还在当前窗口内,并且没有移动到其他窗口上,则发送鼠标移动的事件
		if (newWindow == mHoverWindow)
		{
			// 只有当鼠标真的移动了,才会通知有鼠标移动事件,不过也许此时悬停窗口已经不接收输入事件了,需要判断一下
			if (curMousePos - lastMousePos != txIPoint(0, 0) 
				&& (mHoverWindow->getEnableHandleInput() || mForceHandInput)
				&& (mHoverWindow->isVisible() || mForceVisible))
			{
				mHoverWindow->onMouseMove(curPos);
			}
		}
		// 鼠标已经移动到了其他窗口中,发送鼠标离开的事件
		else
		{
			// 不过也许此时悬停窗口已经不接收输入事件了,需要判断一下
			if ((mHoverWindow->getEnableHandleInput() || mForceHandInput) 
				&& (mHoverWindow->isVisible() || mForceVisible))
			{
				mHoverWindow->onMouseLeave();
				// 先向上发送鼠标离开窗口的事件
				mEngineRoot->sendEvent(EE_MOUSE_LEAVE_WINDOW, mHoverWindow->getName());
			}
			// 找到鼠标所在的新的窗口,给该窗口发送鼠标进入的事件
			if (newWindow != NULL)
			{
				newWindow->onMouseEnter();
				// 向上发送鼠标进入窗口的事件
				mEngineRoot->sendEvent(EE_MOUSE_ENTER_WINDOW, newWindow->getName());
			}
		}
	}
	// 如果上一帧鼠标没有在任何窗口内,则计算这一帧鼠标所在的窗口
	else
	{
		// 发送鼠标进入的事件
		if (newWindow != NULL)
		{
			newWindow->onMouseEnter();
			// 向上发送鼠标悬停窗口事件
			mEngineRoot->sendEvent(EE_MOUSE_ENTER_WINDOW, newWindow->getName());
		}
	}
	mHoverWindow = newWindow;
	setHoverDirty(false);
}

void LayoutManager::destroy()
{
	auto it = mLayoutNameList.begin();
	auto itend = mLayoutNameList.end();
	FOR_STL(mLayoutNameList, ; it != itend; ++it)
	{
		TRACE_DELETE(it->second);
	}
	END_FOR_STL(mLayoutNameList);
	mLayoutNameList.clear();
	mLayoutIDList.clear();
	mRenderOrder.clear();
	auto iterTemp = mTempLayoutList.begin();
	auto iterTempEnd = mTempLayoutList.end();
	FOR_STL(mTempLayoutList, ; iterTemp != iterTempEnd; ++iterTemp)
	{
		TRACE_DELETE(iterTemp->second);
	}
	END_FOR_STL(mTempLayoutList);
	mTempLayoutList.clear();
}

void LayoutManager::update(float elapsedTime)
{
	LOCK(mRenderOrderLock);
	auto iterOrder = mRenderOrder.begin();
	auto iterOrderEnd = mRenderOrder.end();
	FOR_STL(mRenderOrder, ; iterOrder != iterOrderEnd; ++iterOrder)
	{
		auto& layouts = iterOrder->second;
		int layoutCount = layouts.size();
		FOR_STL(layouts, int i = 0; i < layoutCount; ++i)
		{
			layouts[i]->update(elapsedTime);
		}
		END_FOR_STL(layouts);
	}
	END_FOR_STL(mRenderOrder);
	UNLOCK(mRenderOrderLock);
	processKey(elapsedTime);
}

void LayoutManager::processKey(const float& elapsedTime)
{
	// 检测输入,可以通过左键或者右键的点击来激活某个布局,并且将鼠标事件发送到该布局
	const txIPoint& curMousePos = mInputSystem->getMousePosition();
	const txIPoint& lastMousePos = mInputSystem->getLastMousePosition();
	// 鼠标位置改变,或者需要刷新鼠标悬停时才刷新鼠标悬停窗口
	if (curMousePos.x != lastMousePos.x || curMousePos.y != lastMousePos.y || mHoverDirty)
	{
		refreshHoverWindow(curMousePos, lastMousePos);
	}
	// 只有左键或者右键点下才能激活窗口
	Window* focusedWindow = mActiveWindow;
	// 鼠标左键按下事件
	if (mInputSystem->getMouseButtonState(MBD_LEFT).mState == KPS_CURRENT_DOWN)
	{
		if (mHoverWindow != NULL)
		{
			VECTOR2 curPos = VECTOR2((float)curMousePos.x, (float)curMousePos.y);
			mHoverWindow->onLeftDown(curPos);
			focusedWindow = mHoverWindow;
			// 向上发送窗口被选中事件
			mEngineRoot->sendEvent(EE_WINDOW_SELECTED, mHoverWindow->getName());
		}
		// 点击空白的地方
		else
		{
			if (focusedWindow != NULL)
			{
				// 向上发送窗口取消选中事件
				mEngineRoot->sendEvent(EE_WINDOW_UNSELECTED, focusedWindow->getName());
			}
			focusedWindow = NULL;
		}
	}
	// 鼠标右键按下事件
	else if (mInputSystem->getMouseButtonState(MBD_RIGHT).mState == KPS_CURRENT_DOWN)
	{
		if (mHoverWindow != NULL)
		{
			VECTOR2 curPos = VECTOR2((float)curMousePos.x, (float)curMousePos.y);
			mHoverWindow->onRightDown(curPos);
			focusedWindow = mHoverWindow;
		}
		else
		{
			// 点击空白的地方
			focusedWindow = NULL;
		}
	}
	// 鼠标左键抬起事件
	else if (mInputSystem->getMouseButtonState(MBD_LEFT).mState == KPS_CURRENT_UP)
	{
		if (mHoverWindow != NULL)
		{
			VECTOR2 curPos = VECTOR2((float)curMousePos.x, (float)curMousePos.y);
			mHoverWindow->onLeftUp(curPos);
		}
	}
	// 鼠标右键抬起事件
	else if (mInputSystem->getMouseButtonState(MBD_RIGHT).mState == KPS_CURRENT_UP)
	{
		if (mHoverWindow != NULL)
		{
			VECTOR2 curPos = VECTOR2((float)curMousePos.x, (float)curMousePos.y);
			mHoverWindow->onRightUp(curPos);
			// 右键点击窗口可直接切换窗口的显示状态
			if (mEditorMode)
			{
				mHoverWindow->setVisible(!mHoverWindow->isVisible());
			}
		}
	}

	if (focusedWindow != mActiveWindow)
	{
		focusWindow(focusedWindow);
	}

	if (mActiveWindow != NULL)
	{
		mActiveWindow->processKey(elapsedTime);
	}
}

void LayoutManager::focusWindow(Window* window)
{
	// 如果焦点窗口有改变,则取消上一个窗口的焦点
	if (window != mActiveWindow)
	{
		if (mActiveWindow != NULL)
		{
			mActiveWindow->setFocus(false);
		}
		// 设置新的窗口焦点
		mActiveWindow = window;
		if (mActiveWindow != NULL)
		{
			mActiveWindow->setFocus(true);
		}
	}
}

Window* LayoutManager::getHoverWindow(const VECTOR2& position, const bool& useTransform, const bool& forceVisible, const bool& forceHandleInput)
{
	Window* hover = NULL;
	if (mOnlyHoverLayout == EMPTY_STRING)
	{
		LOCK(mRenderOrderLock);
		// 首先找到鼠标位于哪些布局上,从后往前遍历布局
		auto iterOrder = mRenderOrder.rbegin();
		auto iterOrderEnd = mRenderOrder.rend();
		FOR_STL(mRenderOrder, bool find = false; iterOrder != iterOrderEnd && !find; ++iterOrder)
		{
			auto& layouts = iterOrder->second;
			int layoutCount = layouts.size();
			FOR_STL(layouts, int i = 0; i < layoutCount; ++i)
			{
				Layout* layout = layouts[layoutCount - i - 1];
				if (layout->isVisible())
				{
					const txRect& layoutRect = layout->getLayoutRect();
					if (layoutRect.isPointInRect(position))
					{
						hover = layout->getHoverWindow(position, useTransform, forceVisible, forceHandleInput);
						if (hover != NULL)
						{
							find = true;
							break;
						}
					}
				}
			}
			END_FOR_STL(layouts);
		}
		END_FOR_STL(mRenderOrder);
		UNLOCK(mRenderOrderLock);
	}
	else
	{
		auto iterLayout = mLayoutNameList.find(mOnlyHoverLayout);
		if (iterLayout != mLayoutNameList.end())
		{
			Layout* layout = iterLayout->second;
			if (layout != NULL && layout->isVisible())
			{
				const txRect& layoutRect = layout->getLayoutRect();
				if (layoutRect.isPointInRect(position))
				{
					hover = layout->getHoverWindow(position, useTransform, forceVisible, forceHandleInput);
				}
			}
		}
	}
	return hover;
}

void LayoutManager::render()
{
	LOCK(mRenderOrderLock);
	auto iterOrder = mRenderOrder.begin();
	auto iterOrderEnd = mRenderOrder.end();
	FOR_STL(mRenderOrder, ; iterOrder != iterOrderEnd; ++iterOrder)
	{
		txVector<Layout*>& layouts = iterOrder->second;
		int layoutCount = layouts.size();
		FOR_STL(layouts, int i = 0; i < layoutCount; ++i)
		{
			layouts[i]->render();
		}
		END_FOR_STL(layouts);
	}
	END_FOR_STL(mRenderOrder);
	UNLOCK(mRenderOrderLock);
}

Layout* LayoutManager::createLayout(const int& layoutID, const std::string& name, const int& order, const std::string& fileName, const bool& async, LayoutAsyncDone callback, void* userData)
{
	// 查找是否已存在,不存在则创建新布局
	auto iterLayout = mLayoutNameList.find(name);
	if (iterLayout != mLayoutNameList.end())
	{
		return iterLayout->second;
	}
	std::string layoutFile = fileName;
	if (layoutFile != EMPTY_STRING)
	{
		layoutFile = P_UI_LAYOUT_PATH + fileName;
	}
	int id = layoutID;
	if (id == -1)
	{
		id = createUniqueID();
	}
	Layout* layout = TRACE_NEW(Layout, layout, id, name, layoutFile);
	addLayoutToList(layout);

	// 渲染顺序列表
	LOCK(mRenderOrderLock);
	auto iterOrder = mRenderOrder.find(order);
	if (iterOrder != mRenderOrder.end())
	{
		iterOrder->second.push_back(layout);
	}
	else
	{
		txVector<Layout*> layoutVector;
		layoutVector.push_back(layout);
		mRenderOrder.insert(order, layoutVector);
	}
	UNLOCK(mRenderOrderLock);
		
	// 初始化布局
	if (!layout->init(async))
	{
		destroyLayout(layout->getName());
		layout = NULL;
	}
	return layout;
}

Layout* LayoutManager::createTempLayout(const std::string& name)
{
	Layout* layout = TRACE_NEW(Layout, layout, -1, name);
	mTempLayoutList.insert(layout->getName(), layout);
	return layout;
}

void LayoutManager::destroyWindow(Window* window)
{
	// 后序遍历销毁窗口,从最底层窗口开始销毁,此处不能用引用获得子窗口列表,因为在销毁窗口过程中会对列表进行修改
	txVector<Window*> children;
	window->getAllChildren(children);
	auto iter = children.begin();
	auto iterEnd = children.end();
	FOR_STL(children, ; iter != iterEnd; ++iter)
	{
		Window* childWindow = *iter;
		destroyWindow(childWindow);
	}
	END_FOR_STL(children);
	mWindowFactoryManager->getFactory(window->getType())->destroyWindow(window);
}

void LayoutManager::destroyLayout(const std::string& layoutName)
{
	auto it = mLayoutNameList.find(layoutName);
	if (it != mLayoutNameList.end())
	{
		LOCK(mRenderOrderLock);
		auto iterOrder = mRenderOrder.begin();
		auto iterOrderEnd = mRenderOrder.end();
		FOR_STL(mRenderOrder, bool find = false; iterOrder != iterOrderEnd && !find; ++iterOrder)
		{
			auto& layouts = iterOrder->second;
			auto iterVec = layouts.begin();
			auto iterVecEnd = layouts.end();
			FOR_STL(layouts, ; iterVec != iterVecEnd; ++iterVec)
			{
				if (*iterVec == it->second)
				{
					iterOrder->second.erase(iterVec);
					find = true;
					break;
				}
			}
			END_FOR_STL(layouts);
		}
		END_FOR_STL(mRenderOrder);
		TRACE_DELETE(it->second);
		removeLayoutFromList(layoutName);
		UNLOCK(mRenderOrderLock);
	}
}

void LayoutManager::destroyLayout(const int& layoutID)
{
	Layout* layout = getLayout(layoutID);
	if (layout != NULL)
	{
		destroyLayout(layout->getName());
	}
}

void LayoutManager::destroyTempLayout(const std::string& layoutName)
{
	auto it = mTempLayoutList.find(layoutName);
	if (it != mTempLayoutList.end())
	{
		TRACE_DELETE(it->second);
		mTempLayoutList.erase(it);
	}
}

Layout* LayoutManager::getLayout(const std::string& layoutName)
{
	auto it = mLayoutNameList.find(layoutName);
	if (it != mLayoutNameList.end())
	{
		return it->second;
	}
	return NULL;
}

Layout* LayoutManager::getLayout(const int& layoutID)
{
	auto it = mLayoutIDList.find(layoutID);
	if (it != mLayoutIDList.end())
	{
		return it->second;
	}
	return NULL;
}

Layout* LayoutManager::getTempLayout(const std::string& layoutName)
{
	auto it = mTempLayoutList.find(layoutName);
	if (it != mTempLayoutList.end())
	{
		return it->second;
	}
	return NULL;
}

void LayoutManager::setGlobalWindowOffset(const txIPoint& offset)
{
	auto iter = mLayoutNameList.begin();
	auto iterEnd = mLayoutNameList.end();
	FOR_STL(mLayoutNameList, ; iter != iterEnd; ++iter)
	{
		Window* window = iter->second->getRootWindow();
		if (window != NULL)
		{
			window->setOffset(offset);
		}
	}
	END_FOR_STL(mLayoutNameList);
}

void LayoutManager::changeText(const std::string& str)
{
	if (mActiveWindow != NULL)
	{
		mActiveWindow->setText(str, false);
	}
}

bool LayoutManager::notifyLayoutRenamed(const std::string& oldName, Layout* layout)
{
	if (!mLayoutNameList.contains(oldName) || mLayoutNameList.contains(layout->getName()))
	{
		return false;
	}
	removeLayoutFromList(layout->getName());
	addLayoutToList(layout);
	return true;
}

void LayoutManager::notifyWindowVisibleChanged(Window* window)
{
	// 窗口显示状态改变时重新判断当前悬停窗口
	setHoverDirty(true);
}

void LayoutManager::notifyRenderWindowChanged()
{
	auto it = mLayoutNameList.begin();
	auto itEnd = mLayoutNameList.end();
	FOR_STL(mLayoutNameList, ; it != itEnd; ++it)
	{
		it->second->notifyRenderWindowChanged();
	}
	END_FOR_STL(mLayoutNameList);
	initPerspective();
}

void LayoutManager::initPerspective()
{
	mFovY = 45.0f;
	mPerspectiveMatrix = txMath::perspective(mFovY, mRenderWindow->getAspect(), 1.0f, 2500.0f);
	mInversePerspectiveMatrix = txMath::inverse(mPerspectiveMatrix);
}

void LayoutManager::removeLayoutFromList(Layout* layout)
{
	mLayoutNameList.erase(layout->getName());
	mLayoutIDList.erase(layout->getLayoutID());
}

void LayoutManager::removeLayoutFromList(const std::string& name)
{
	Layout* layout = getLayout(name);
	if (layout == NULL)
	{
		return;
	}
	mLayoutNameList.erase(name);
	mLayoutIDList.erase(layout->getLayoutID());
}

void LayoutManager::addLayoutToList(Layout* layout)
{
	mLayoutNameList.insert(layout->getName(), layout);
	mLayoutIDList.insert(layout->getLayoutID(), layout);
}