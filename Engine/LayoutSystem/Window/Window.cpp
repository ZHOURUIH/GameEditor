#include "txEngineRoot.h"
#include "txRendererManager.h"
#include "txPropertyManager.h"
#include "txProperty.h"
#include "txRenderWindow.h"
#include "txComponentHeader.h"

#include "WindowHeader.h"
#include "WindowProperty.h"
#include "LayoutManager.h"
#include "LayoutUtility.h"
#include "Layout.h"
#include "WindowPrefabManager.h"

GLushort Window::mWindowIndices[UI_INDEX_COUNT] = { 0, 1, 2, 0, 2, 3 };
GLushort Window::mWireFrameIndices[WIREFRAME_INDEX_COUNT] = { 0, 1, 1, 2, 2, 3, 3, 0 };
GLushort Window::mCoordinateIndices[COORDINATE_INDEX_COUNT] = { 0, 2, 1, 3 };

Window::Window(const std::string& name, const WINDOW_TYPE& type)
	:
	txComponentOwner(name),
	mParent(NULL),
	mType(type),
	mVisible(true),
	mRotateAround(true),
	mDirty(true),
	mTexCoordDirty(true),
	mAlpha(1.0f),
	mColour(0.0f, 0.0f, 0.0f),
	mColourBlendMode(BM_ADD),
	mSymmetryMode(SM_NONE),
	mHandleInputEnable(true),
	mFocus(false),
	mWaterEffect(false),
	mWaterTime(0.0f),
	mLocalScale(1.0f, 1.0f),
	mTransformCenter(0.5f, 0.5f, 0.0f),
	mIsComplexWindow(false),
	mIsInnerChild(false),
	mParentComplexWindow(NULL),
	mRenderBaseWireframe(true),
	mDiriveParentRotation(true)
{
	mShowWireframe = false;
	mShowCoordinate = false;
	mWireframeColour = VECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
	mCoordinateColour = VECTOR4(0.4f, 0.2f, 0.0f, 1.0f);
	mAutoHeight = txDim::IDENTITY;
	mAutoWidth = txDim::IDENTITY;
	mTypeName = LayoutUtility::getTypeNameFromWindowType(mType);
	setPosition(txPoint::ZERO);
	setSize(txPoint::IDENTITY);
}

Window::~Window()
{
	destroy();
}

void Window::init(Layout* layout)
{
	mParentLayout = layout;
	initReceiver();
	initComponents();
}

void Window::initComponents()
{	
	addComponent("Audio", TOSTRING(txComponentAudio))->setActive(true);
	addComponent("MoveRelative", TOSTRING(WindowComponentMoveRelative))->setActive(false);
	addComponent("Rotate", TOSTRING(WindowComponentRotate))->setActive(false);
	addComponent("RotateSpeed", TOSTRING(WindowComponentRotateSpeed))->setActive(false);
	addComponent("Scale", TOSTRING(WindowComponentScale))->setActive(false);
	addComponent("Alpha", TOSTRING(WindowComponentAlpha))->setActive(false);
	addComponent("Move", TOSTRING(WindowComponentMove))->setActive(false);
	addComponent("ColourFade", TOSTRING(WindowComponentColour))->setActive(false);
	addComponent("HSL", TOSTRING(WindowComponentHSL))->setActive(false);
	addComponent("SelffitScale", TOSTRING(WindowComponentSelffitScale))->setActive(false);
	// 所有改变Rect的组件需要提前更新
	getFirstComponent(TOSTRING(WindowComponentMove))->setPreUpdate(true);
	getFirstComponent(TOSTRING(WindowComponentMoveRelative))->setPreUpdate(true);
}

void Window::initProperty()
{
	addProperty<WindowPropertyPosition>();
	addProperty<WindowPropertySize>();
	addProperty<WindowPropertyAutoSizeType>();
	addProperty<WindowPropertyHeightDim>();
	addProperty<WindowPropertyWidthDim>();
	addProperty<WindowPropertyPrefab>();
	addProperty<WindowPropertyVisible>();
	addProperty<WindowPropertyHandleInput>();
	addProperty<WindowPropertyAlpha>();
	addProperty<WindowPropertyColour>();
	addProperty<WindowPropertyColourBlendMode>();
	addProperty<WindowPropertySymmetryMode>();
	addProperty<WindowPropertyWaterEffect>();
	addProperty<WindowPropertyRotation>();
	addProperty<WindowPropertyScale>();
	addProperty<WindowPropertyTransform>();
	addProperty<WindowPropertyTransformCenter>();
	addProperty<WindowPropertyHSL>();
	addProperty<WindowPropertyRotateAround>();
	addProperty<WindowPropertyDiriveParentRotate>();
	addProperty<WindowPropertyInnerChild>();
}

void Window::setProperty(const std::string& propertyName, const std::string& propertyValue)
{
	txProperty* property = getProperty(propertyName);
	if (property == NULL)
	{
		return;
	}
	property->set(propertyValue, this);
	if (mParent != NULL)
	{
		mParent->notifyChildPropertyChanged(propertyName, propertyValue);
	}
}

void Window::setVisible(const bool& visible)
{ 
	if (mVisible != visible)
	{
		mVisible = visible;
		mLayoutManager->notifyWindowVisibleChanged(this);
	}
}

void Window::copyChildren(Window* destWindow, const bool& useParentNameAsPreName, const std::string& preName)
{
	if (destWindow == NULL)
	{
		return;
	}

	int sourceChildCount = mChildren.size();
	FOR_STL(mChildren, int i = 0; i < sourceChildCount; ++i)
	{
		Window* sourceChild = mChildren[i];
		std::string childNewName = (useParentNameAsPreName ? mName : preName) + sourceChild->mName;
		// 先创建对应类型的窗口,然后将所有属性拷贝过去,然后再拷贝子窗口的所有子窗口
		Window* myChild = NULL;
		if (destWindow->mParentLayout != NULL)
		{
			myChild = destWindow->mParentLayout->createWindow(sourceChild->getTypeName(), childNewName);
		}
		else
		{
			if (destWindow->mType == WT_PREFAB)
			{
				myChild = mWindowPrefabManager->createWindow(sourceChild->getTypeName(), childNewName);
			}
			else
			{
				myChild = Layout::createIndependentWindow(sourceChild->getTypeName(), childNewName);
			}
		}
		if (myChild != NULL)
		{
			destWindow->addChild(myChild);
			sourceChild->copyProperty(myChild);
			// 没有链接到任何预设时,才拷贝子窗口
			if (!sourceChild->hasLinkedPrefab(false))
			{
				sourceChild->copyChildren(myChild, useParentNameAsPreName, preName);
			}
		}
	}
	END_FOR_STL(mChildren);
}

void Window::copyProperty(Window* destWindow)
{
	auto iter = mPropertyMap.begin();
	auto iterEnd = mPropertyMap.end();
	FOR_STL(mPropertyMap, ; iter != iterEnd; ++iter)
	{
		txProperty* property = destWindow->getProperty(iter->first);
		if (property != NULL)
		{
			property->set(iter->second->get(this), destWindow);
		}
	}
	END_FOR_STL(mPropertyMap);
}

void Window::setEnableHandleInput(const bool& enable)
{ 
	mHandleInputEnable = enable; 
	mLayoutManager->setHoverDirty(true); 
}

void Window::removeChild(const std::string& name)
{
	// 从全部子窗口查找列表中删除
	Window* window = NULL;
	auto itrAllMap = mChildrenMap.find(name);
	if (itrAllMap != mChildrenMap.end())
	{
		window = itrAllMap->second;
		mChildrenMap.erase(itrAllMap);
	}
	// 找不到则应该是逻辑上出问题了
	if (window == NULL)
	{
		return;
	}
	// 从全部子窗口顺序列表中删除
	auto iterAll = mChildren.begin();
	auto iterAllEnd = mChildren.end();
	FOR_STL(mChildren, ; iterAll != iterAllEnd; ++iterAll)
	{
		if (*iterAll == window)
		{
			mChildren.erase(iterAll, false);
			break;
		}
	}
	END_FOR_STL(mChildren);

	notifyChildRemoved();
}

Window* Window::getChild(const std::string& name)
{
	auto iter = mChildrenMap.find(name);
	if (iter != mChildrenMap.end())
	{
		return iter->second;
	}
	return NULL;
}

void Window::render()
{
	if (mRenderBaseWireframe)
	{
		renderTexture(NULL, NULL, txMath::VEC2_ZERO, txMath::VEC2_ZERO, 1.0f, false);
	}
	int childCount = mChildren.size();
	FOR_STL(mChildren, int i = 0; i < childCount; ++i)
	{
		if (mChildren[i]->isVisible())
		{
			mChildren[i]->render();
		}
	}
	END_FOR_STL(mChildren);
}

void Window::renderTexture(txTexture* texture, txTexture* maskTexture, const VECTOR2& textureCoord, 
	const VECTOR2& textureSize, const float& fArc, const bool& linearDodge, const bool& useMipmap)
{
	const MATRIX4& viewMatrix = mLayoutManager->getUIViewMatrix();
	const MATRIX4& curProject = mLayoutManager->getPerspectiveMatrix();
	if (texture != NULL)
	{
		txRendererInstance::mUIRender->render(texture, maskTexture, curProject * viewMatrix, mWindowVertices, mWindowIndices,
			mTexCoords, mAlpha, mColour, mColourBlendMode, mHSLOffset, mSymmetryMode, mWaterEffect, mWaterTime, textureSize, fArc, linearDodge, useMipmap);
	}

	// 渲染线框和窗口坐标系
	if (mShowWireframe)
	{
		txRendererInstance::mColourRender->render(mWireframeColour, curProject * viewMatrix, mWireframeVertices, mWireFrameIndices, WIREFRAME_INDEX_COUNT, 1.0f);
	}
	if (mShowCoordinate)
	{
		txRendererInstance::mColourRender->render(mCoordinateColour, curProject * viewMatrix, mCoordinateVertices, mCoordinateIndices, COORDINATE_INDEX_COUNT, 1.0f);
	}
}

void Window::detachLayoutParent(const bool& detachLayoutOnly)
{
	if (mParentLayout != NULL)
	{
		mParentLayout->notifyWindowDetached(mName);
		mParentLayout = NULL;
	}
	// 如果不是只断开与布局的联系,则还需要断开与父窗口的联系
	if (!detachLayoutOnly && mParent != NULL)
	{
		mParent->notifyChildDetached(mName);
		mParent = NULL;
	}
	// 使自己所有的子窗口都断开与布局的联系,但是不能打断子窗口的父子关系
	int childCount = mChildren.size();
	FOR_STL(mChildren, int i = 0; i < childCount; ++i)
	{
		mChildren[i]->detachLayoutParent(true);
	}
	END_FOR_STL(mChildren);
}
// 建立与布局和父窗口的联系,使该窗口成为布局中的一个窗口,该窗口下的所有子窗口也会重建与布局的联系,父子关系仍然存在
void Window::attachLayoutParent(Layout* layout, Window* parent, const int& childPos)
{
	if (layout != NULL && mParentLayout == NULL)
	{
		mParentLayout = layout;
		layout->notifyWindowAttached(this);
		// 使自己所有的子窗口都建立与布局的联系
		int childCount = mChildren.size();
		FOR_STL(mChildren, int i = 0; i < childCount; ++i)
		{
			mChildren[i]->attachLayoutParent(layout, NULL, -1);
		}
		END_FOR_STL(mChildren);
	}
	if (parent != NULL && mParent == NULL)
	{
		parent->addChild(this);
		parent->moveChildPos(this, childPos);
	}
	setDirty(true);
}

void Window::refreshWindowRect()
{
	mPixelPosition = LayoutUtility::getWindowPosition(this);
	VECTOR2 lastPixelSize = mPixelSize;
	mPixelSize = LayoutUtility::getWindowSize(this);
	mFinalPosition = mPixelPosition;
	mFinalSize = mPixelSize;
	if (!txMath::isFloatZero(txMath::getLength(lastPixelSize - mPixelSize)))
	{
		notifyPixelSizeChanged();
	}
	mLocalPixelPosition = LayoutUtility::getRelativeWindowPosition(this);
	// 通知自己窗口区域改变
	notifyWindowRectChanged();
	// 通知布局自己的窗口区域改变
	if (mParentLayout != NULL)
	{
		mParentLayout->notifyWindowRectChanged();
	}
}

void Window::refreshWindowTransform()
{
	if (mParent == NULL)
	{
		MATRIX4 localTransform;
		txMath::setMatrixRotation(localTransform, mLocalRotation);
		txMath::setMatrixScale(localTransform, VECTOR3(mLocalScale.x, mLocalScale.y, 1.0f));
		txMath::setMatrixPosition(localTransform, mLocalTranslation);
		mFinalMatrix = localTransform;
	}
	else
	{
		MATRIX4 localTransform;
		txMath::setMatrixRotation(localTransform, mLocalRotation);
		txMath::setMatrixScale(localTransform, VECTOR3(mLocalScale.x, mLocalScale.y, 1.0f));
		// 如果要随父窗口一起旋转,则要对相对的平移做处理
		if (mRotateAround)
		{
			// 窗口中心相对于父窗口中心的像素距离
			VECTOR2 relativePos = mLocalPixelPosition + mPixelSize / 2.0f - mParent->getPixelSize() / 2.0f;
			VECTOR3 newTranslation = mLocalTranslation + VECTOR3(relativePos.x, relativePos.y, 0.0f);
			txMath::setMatrixPosition(localTransform, newTranslation);
		}
		else
		{
			txMath::setMatrixPosition(localTransform, mLocalTranslation);
		}
		mFinalMatrix = mParent->getFinalMatrix() * localTransform;
		// 如果不继承父窗口的旋转值,则需要将父窗口的旋转去除
		if (!mDiriveParentRotation)
		{
			mFinalMatrix *= MATRIX4(txMath::inverse(txMath::getMatrixRotation(mParent->getFinalMatrix())));
		}
		// 因为已经窗口的渲染坐标已经包含了相对于父窗口的平移,所以此处在对相对平移处理完后,需要通过调整矩阵来去除渲染坐标中包含的相对平移
		if (mRotateAround)
		{
			VECTOR3 finalTranslation = txMath::getMatrixPosition(mFinalMatrix);
			VECTOR2 relativePos = mLocalPixelPosition + mPixelSize / 2.0f - mParent->getPixelSize() / 2.0f;
			txMath::setMatrixPosition(mFinalMatrix, finalTranslation - VECTOR3(relativePos.x, relativePos.y, 0.0f));
		}
	}
	if (mParentLayout != NULL)
	{
		mParentLayout->notifyWindowRectChanged();
	}

	// 一般刷新变换矩阵之前都会确定窗口的位置属性都是最新的,所以在刷新变换矩阵后,刷新窗口的渲染顶点数据
	VECTOR2 finalPos, finalSize;
	getFinalPosSize(finalPos, finalSize);
	// 更新顶点坐标
	LayoutUtility::generateVertices(mWindowVertices, finalPos, finalSize, mFinalMatrix, mTransformCenter, false, mLayoutManager->getFovY(), mLayoutManager->getDirectVertices());
	// 更新线框的顶点坐标和坐标系的顶点坐标
	LayoutUtility::generateWireframeCoordinate(mWireframeVertices, mCoordinateVertices, mWindowVertices);
}

void Window::forceRefresh()
{
	// 先判断父节点是否需要刷新,如果需要刷新,则强制刷新父节点
	if (mParent != NULL && mParent->mDirty)
	{
		mParent->forceRefresh();
	}
	refreshWindowRect();
	refreshWindowTransform();
	setDirty(false, false);
}

void Window::getFinalPosSize(VECTOR2& finalPos, VECTOR2& finalSize)
{
	finalPos = mFinalPosition;
	finalSize = mFinalSize;
	if (txMath::getLength(mFrameTremblingOffset) > 0.0f)
	{
		VECTOR2 center = finalPos + finalSize / 2.0f;
		finalSize = finalSize + mFrameTremblingOffset * 2.0f;
		finalPos = center - finalSize / 2.0f;
	}
}

void Window::update(const float& elapsedTime)
{
	if (!isVisible())
	{
		return;
	}
	mWaterTime += elapsedTime;
	firstUpdate(elapsedTime);
	if (mDirty)
	{
		refreshWindowRect();
	}
	insertUpdate(elapsedTime);
	if (mDirty)
	{
		// 更新最终变换
		refreshWindowTransform();
	}
	mDirty = false;
	// 确保纹理坐标已经更新
	if (mTexCoordDirty)
	{
		notifyTexCoordNeedUpdated();
		mTexCoordDirty = false;
	}
	// 更新子窗口
	int childCount = mChildren.size();
	FOR_STL(mChildren, int i = 0; i < childCount; ++i)
	{
		mChildren[i]->update(elapsedTime);
	}
	END_FOR_STL(mChildren);
}

void Window::setDirty(const bool& dirty, const bool& refreshNow)
{
	mDirty = dirty;
	if (mDirty)
	{
		// 如果需要立即刷新,则要刷新窗口尺寸和窗口变换
		if (refreshNow)
		{
			forceRefresh();
		}
		int childCount = mChildren.size();
		FOR_STL(mChildren, int i = 0; i < childCount; ++i)
		{
			// 此处不能使用mDirty,因为在forceRefresh中会改变mDirty
			mChildren[i]->setDirty(dirty, refreshNow);
		}
		END_FOR_STL(mChildren);
	}
}

bool Window::isPointInRect(const VECTOR2& point, const bool& useTranform)
{
	if (useTranform)
	{
		// 构建窗口渲染的4个顶点
		static GLfloat vertices[UI_VERTEX_COUNT * 3];
		static VECTOR3 vertexPoint[UI_VERTEX_COUNT];
		VECTOR2 finalPos, finalSize;
		getFinalPosSize(finalPos, finalSize);
		LayoutUtility::generateVertices(vertices, finalPos, finalSize, mFinalMatrix, mTransformCenter, false, mLayoutManager->getFovY(), mLayoutManager->getDirectVertices());
		for (int i = 0; i < UI_VERTEX_COUNT; ++i)
		{
			vertexPoint[i] = VECTOR3(vertices[i * 3 + 0], vertices[i * 3 + 1], vertices[i * 3 + 2]);
		}
		// 构建鼠标射线
		VECTOR3 worldPoint = LayoutUtility::getPickupRayDirection(point);
		VECTOR3 bary;
		// 检测与第一个三角形相交
		// 正面
		if (txMath::intersectRayTriangle(txMath::VEC3_ZERO, worldPoint, vertexPoint[0], vertexPoint[1], vertexPoint[2], bary))
		{
			// 暂不计算交点
			//VECTOR3 point = (1 - bary.x - bary.y) * vertexPoint[0] + bary.x * vertexPoint[1] + bary.y * vertexPoint[2];
			return true;
		}
		// 反面
		else if (txMath::intersectRayTriangle(txMath::VEC3_ZERO, worldPoint, vertexPoint[0], vertexPoint[2], vertexPoint[1], bary))
		{
			//VECTOR3 point = (1 - bary.x - bary.y) * vertexPoint[0] + bary.x * vertexPoint[1] + bary.y * vertexPoint[2];
			return true;
		}
		// 检测与第二个三角形相交
		// 正面
		else if (txMath::intersectRayTriangle(txMath::VEC3_ZERO, worldPoint, vertexPoint[0], vertexPoint[2], vertexPoint[3], bary))
		{
			return true;
			//VECTOR3 point = (1 - bary.x - bary.y) * vertexPoint[0] + bary.x * vertexPoint[2] + bary.y * vertexPoint[3];
		}
		// 反面
		else if (txMath::intersectRayTriangle(txMath::VEC3_ZERO, worldPoint, vertexPoint[0], vertexPoint[3], vertexPoint[2], bary))
		{
			return true;
			//VECTOR3 point = (1 - bary.x - bary.y) * vertexPoint[0] + bary.x * vertexPoint[2] + bary.y * vertexPoint[3];
		}
		return false;
	}
	else
	{
		txRect rect(mFinalPosition, mFinalPosition + mFinalSize);
		return rect.isPointInRect(point);
	}
}

Window* Window::getHoverWindow(const VECTOR2& position, const bool& useTransform, const bool& forceVisible, const bool& forceHandleInput)
{
	// 使用后序遍历,先访问自己的最后一个子窗口
	Window* hover = NULL;
	int childCount = mChildren.size();
	FOR_STL(mChildren, int i = childCount - 1; i >= 0; --i)
	{
		hover = mChildren[i]->getHoverWindow(position, useTransform, forceVisible, forceHandleInput);
		if (hover != NULL)
		{
			break;
		}
	}
	END_FOR_STL(mChildren);
	if (hover != NULL)
	{
		return hover;
	}
	// 如果没有在任何一个子窗口内,判断是否在自己的窗口范围内,并且自己可以接受输入事件
	// 可选中的窗口必须是可见的和接受输入事件的,或者由选中标记忽略可见性或者接受输入事件
	if ((forceHandleInput || getEnableHandleInput()) && (forceVisible || isVisible()) && isPointInRect(position, useTransform))
	{
		return this;
	}
	return NULL;
}

bool Window::isVisible()
{
	if (mParent != NULL)
	{
		return mVisible && mParent->isVisible();
	}
	return mVisible;
}

bool Window::rename(const std::string& newName)
{
	if (mName == newName)
	{
		return false;
	}
	std::string oldName = mName;
	mName = newName;
	if (mParentLayout != NULL)
	{
		// 通知Layout自己的名字改变了
		bool ret = mParentLayout->notifyWindowNameChanged(oldName, this);
		// 如果父窗口不允许自己改名,则需要将名字还原
		if (!ret)
		{
			mName = oldName;
			return false;
		}
	}
	// 通知父窗口自己的名字改变了,此处不需要再判断父窗口是否允许改名
	if (NULL != mParent)
	{
		mParent->notifyChildNameChanged(oldName, this);
	}
	return true;
}

bool Window::hasLinkedPrefab(const bool& recursive)
{
	// 首先判断自己是否链接了预设
	if (mPrefabName != EMPTY_STRING)
	{
		return true;
	}
	if (recursive)
	{
		bool linked = false;
		// 然后判断子窗口是否链接了预设
		int childCount = mChildren.size();
		FOR_STL(mChildren, int i = 0; i < childCount; ++i)
		{
			// 发现有子窗口链接了预设就立即返回
			if (mChildren[i]->hasLinkedPrefab(recursive))
			{
				linked = true;
				break;
			}
		}
		END_FOR_STL(mChildren);
		return linked;
	}
	return false;
}
	
bool Window::isChild(Window* window, const bool& recursive)
{
	// 先判断是否是自己的第一级子窗口
	auto iterChild = mChildrenMap.find(window->getName());
	if (iterChild != mChildrenMap.end())
	{
		return true;
	}
	// 不在第一级子窗口,如果递归查找,则继续在子窗口中查找
	else if (recursive)
	{
		bool ret = false;
		iterChild = mChildrenMap.begin();
		auto iterChildEnd = mChildrenMap.end();
		FOR_STL(mChildrenMap, ; iterChild != iterChildEnd; ++iterChild)
		{
			if (iterChild->second->isChild(window))
			{
				ret = true;
				break;
			}
		}
		END_FOR_STL(mChildrenMap);
		return ret;
	}
	return false;
}

void Window::setPrefabName(const std::string& name)
{
	if (mPrefabName == name)
	{
		return;
	}
		
	WindowPrefab* prefab = mWindowPrefabManager->getPrefab(name);
	if (prefab != NULL)
	{
		prefab->copyChildren(this, false, mName);
	}
	mPrefabName = name;
	txVector<std::string> params;
	params.push_back(mName);
	params.push_back(name);
	mEngineRoot->sendEvent(EE_WINDOW_PREFAB_CHANGED, params);
}

void Window::notifyChildNameChanged(const std::string& oldName, Window* window)
{
	// 修改全部子窗口查找列表中的名字
	auto iterAll = mChildrenMap.find(oldName);
	if (iterAll != mChildrenMap.end())
	{
		auto iterNew = mChildrenMap.find(window->mName);
		if (iterNew == mChildrenMap.end())
		{
			mChildrenMap.insert(window->mName, window);
			mChildrenMap.erase(iterAll);
		}
		else
		{
			ENGINE_ERROR("error : there is a child named : %s! Window::notifyChildNameChanged", window->mName.c_str());
		}
	}
}

void Window::selfRectChanged()
{
	if (mParentComplexWindow != NULL)
	{
		mParentComplexWindow->notifyChildRectChanged(this);
	}
}

void Window::notifyChildDestroied(const std::string& childName)
{
	removeChild(childName);
}

void Window::notifyChildDetached(const std::string& childName)
{
	removeChild(childName);
}

void Window::notifyChildPropertyChanged(const std::string& propertyName, const std::string& newValue)
{
	mPrefabName = EMPTY_STRING;
	if (mParent != NULL)
	{
		mParent->notifyChildPropertyChanged(propertyName, newValue);
	}
}

void Window::notifyChildAdded()
{
	mPrefabName = EMPTY_STRING;
	if (mParent != NULL)
	{
		mParent->notifyChildAdded();
	}
}

void Window::notifyChildRemoved()
{
	mPrefabName = EMPTY_STRING;
	if (mParent != NULL)
	{
		mParent->notifyChildRemoved();
	}
}

void Window::destroy()
{
	int size = mChildren.size();
	FOR_STL(mChildren, int i = 0; i < size; ++i)
	{
		mChildren[i]->notifyParentDestroied();
	}
	END_FOR_STL(mChildren);

	if (mParent != NULL)
	{
		mParent->notifyChildDestroied(mName);
	}

	if (mParentComplexWindow != NULL)
	{
		mParentComplexWindow->notifyInnerChildDestroied(mName);
	}
		
	if (mParentLayout != NULL)
	{
		mParentLayout->notifyWindowDestroied(mName);
	}

	mChildren.clear();
	mChildrenMap.clear();
}

void Window::saveWindow(std::string& windowStream)
{
	// 先序遍历所有窗口,先保存父窗口,然后再保存所有子窗口
	txStringUtility::jsonStartStruct(windowStream, 1, true);
	txStringUtility::jsonAddPair(windowStream, TYPE_PROPERTY_NAME, mTypeName, 2, true);
	txStringUtility::jsonAddPair(windowStream, NAME_PROPERTY_NAME, mName, 2, true);
	if (mParent != NULL)
	{
		txStringUtility::jsonAddPair(windowStream, PARENT_PROPERTY_NAME, mParent->mName, 2, true);
	}
	// 遍历自己的所有属性
	auto iterProperty = mPropertyMap.begin();
	auto iterPropertyEnd = mPropertyMap.end();
	int propertyCount = mPropertyMap.size();
	FOR_STL(mPropertyMap, ; iterProperty != iterPropertyEnd; ++iterProperty)
	{
		txStringUtility::jsonAddPair(windowStream, iterProperty->second->getName(), iterProperty->second->get(this), 2, true);
	}
	END_FOR_STL(mPropertyMap);
	txStringUtility::jsonEndStruct(windowStream, 1, true);
	// 如果没有关联任何预设,则保存子窗口信息,内部子窗口不保存
	if (mPrefabName == EMPTY_STRING)
	{
		int childCount = mChildren.size();
		FOR_STL(mChildren, int i = 0; i < childCount; ++i)
		{
			mChildren[i]->saveWindow(windowStream);
		}
		END_FOR_STL(mChildren);
	}
}

void Window::saveTemplate(std::string& windowStream)
{
	// 先序遍历所有窗口,先保存父窗口,然后再保存所有子窗口
	txStringUtility::jsonStartArray(windowStream, 0, true);
	txStringUtility::jsonStartStruct(windowStream, 1, true);
	// 首先保存窗口类型
	txStringUtility::jsonAddPair(windowStream, TYPE_PROPERTY_NAME, mTypeName, 2, true);
	// 自己的名字
	txStringUtility::jsonAddPair(windowStream, NAME_PROPERTY_NAME, mName, 2, true);
	// 遍历自己的所有属性
	auto iterProperty = mPropertyMap.begin();
	auto iterPropertyEnd = mPropertyMap.end();
	int propertyCount = mPropertyMap.size();
	FOR_STL(mPropertyMap, ; iterProperty != iterPropertyEnd; ++iterProperty)
	{
		txStringUtility::jsonAddPair(windowStream, iterProperty->second->getName(), iterProperty->second->get(this), 2, true);
	}
	END_FOR_STL(mPropertyMap);
	txStringUtility::jsonEndStruct(windowStream, 1, true);
	txStringUtility::jsonEndArray(windowStream, 0, true);
}

int Window::getChildPos(Window* window)
{
	if (window == NULL)
	{
		return -1;
	}
	// 首先查找当前窗口的位置
	int pos = -1;
	int childCount = mChildren.size();
	FOR_STL(mChildren, int i = 0; i < childCount; ++i)
	{
		if (window == mChildren[i])
		{
			pos = i;
			break;
		}
	}
	END_FOR_STL(mChildren);
	return pos;
}

bool Window::moveChildPos(Window* window, const int& destPos)
{
	if (window == NULL || destPos < 0 || destPos >= (int)mChildren.size())
	{
		return false;
	}
	// 首先查找当前窗口的位置
	int pos = getChildPos(window);
	if (pos < 0 || pos == destPos)
	{
		return false;
	}
	mChildren.erase(mChildren.begin() + pos);
	mChildren.insert(mChildren.begin() + destPos, window);
	return true;
}
	
bool Window::moveChildPos(const std::string& name, const int& destPos)
{
	return moveChildPos(getChild(name), destPos);
}