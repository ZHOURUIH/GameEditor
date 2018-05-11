#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "txIPoint.h"
#include "txEngineDefine.h"
#include "txPropertyReceiver.h"
#include "txURect.h"
#include "txRect.h"
#include "txComponentOwner.h"

class ComplexWindow;
class Layout;
class Window : public txPropertyReceiver, public txComponentOwner
{
public:
	Window(const std::string& name, const WINDOW_TYPE& type);
	virtual ~Window();

	virtual void init(Layout* layout);
	virtual void initComponents();
	virtual void render();	// 绘制窗口
	virtual void firstUpdate(const float& elapsedTime)
	{
		updatePreComponent(elapsedTime);
	}
	virtual void insertUpdate(const float& elapsedTime)
	{
		updateComponents(elapsedTime);
	}
	virtual void update(const float& elapsedTime);			// 更新窗口组件
	void destroy();
	// 只是为其他派生出的窗口类提供一个渲染纹理的接口,以便于进行一些共有的操作
	void renderTexture(txTexture* texture, txTexture* maskTexture, const VECTOR2& textureCoord, 
		const VECTOR2& textureSize, const float& fArc, const bool& linearDodge, const bool& useMipmap = true);
	// 断开与布局和父窗口的联系,使该窗口成为一个独立的窗口,该窗口下的所有子窗口也会断开与布局的联系,但是父子关系仍然存在
	// detachLayoutOnly表示是否只断开与布局的联系,不断开窗口之间的父子关系
	virtual void detachLayoutParent(const bool& detachLayoutOnly = false);
	// 建立与布局和父窗口的联系,使该窗口成为布局中的一个窗口,该窗口下的所有子窗口也会重建与布局的联系,父子关系仍然存在
	virtual void attachLayoutParent(Layout* layout, Window* parent, const int& childPos);
	// 保存窗口
	virtual void saveWindow(std::string& windowStream);
	// 保存为窗口模板,不保存父窗口名和子窗口
	virtual void saveTemplate(std::string& windowStream);
	// 完成异步加载,该函数必须在主线程中调用
	virtual void finishAsycLoad(){}
	void setRect(const txURect& rect)			// 设置位置
	{
		mRect = rect;
		selfRectChanged();
		setDirty(true);
	}
	void addChild(Window* child)
	{
		auto iter = mChildrenMap.find(child->mName);
		if (iter != mChildrenMap.end())
		{
			return;
		}
		child->setParent(this);
		mChildrenMap.insert(child->mName, child);
		mChildren.push_back(child);
		notifyChildAdded();
	}
	virtual void removeChild(const std::string& name);			// 只将子窗口从列表中移除,不会销毁子窗口
	virtual Window* getChild(const std::string& name);			// 获取子窗口
	// 获得所有子窗口列表
	virtual void getAllChildren(txVector<Window*>& children)
	{
		children = mChildren;
		if (children.isLocked())
		{
			children.unlock();
		}
	}
	virtual bool rename(const std::string& newName);
	bool hasLinkedPrefab(const bool& recursive = true);			// 窗口或者所有的子窗口是否链接了预设
	bool isChild(Window* window, const bool& recursive = true);	// window是否为当前窗口的子窗口
	void setPrefabName(const std::string& name);
	bool isVisible();// 获取可视状态
	void setPosition(const txPoint& position, const bool& refreshNow = false)
	{
		txPoint size = mRect.mMax - mRect.mMin;
		mRect.mMin = position;
		mRect.mMax = mRect.mMin + size;
		selfRectChanged();
		setDirty(true, refreshNow);
	}
	void setSize(const txPoint& size, const bool& refreshNow = false)
	{
		mRect.mMax = mRect.mMin + size;
		selfRectChanged();
		setDirty(true, refreshNow);
	}
	const txPoint& getPosition(){ return mRect.mMin; }
	txPoint getSize(){ return mRect.mMax - mRect.mMin; }
	virtual void setDirty(const bool& dirty, const bool& refreshNow = false);
	// 屏幕上的一个点是否在窗口渲染区域内,useTranform表示是否考虑窗口的矩阵变换
	bool isPointInRect(const VECTOR2& point, const bool& useTranform);
	virtual void setMatrixScale(const VECTOR2& scale, const bool& refreshNow = false)
	{
		mLocalScale = scale; 
		setDirty(true, refreshNow); 
	}
	virtual void setMatrixRotation(const MATRIX3& rotation, const bool& refreshNow = false)
	{
		mLocalRotation = rotation; 
		setDirty(true, refreshNow); 
	}
	virtual void setMatrixTranslation(const VECTOR3& trans, const bool& refreshNow = false)
	{ 
		mLocalTranslation = trans; 
		setDirty(true, refreshNow); 
	}
	void setAutoSizeType(const AUTO_SIZE_TYPE& type)
	{
		mAutoSizeType = type;
		selfRectChanged();
		setDirty(true);
	}
	void setAutoHeight(const txDim& autoHeight)
	{
		mAutoHeight = autoHeight;
		selfRectChanged();
		setDirty(true);
	}
	void setAutoWidth(const txDim& autoWidth)
	{
		mAutoWidth = autoWidth;
		selfRectChanged();
		setDirty(true);
	}
	void setOffset(const txIPoint& offset)
	{
		// 由于offset会直接影响Rect,所以需要计算出每次设置offset的差值
		txIPoint offsetDelta = offset - mOffset;
		if (offsetDelta.x != 0 || offsetDelta.y != 0)
		{
			mRect.mMin.x.mAbsolute += offsetDelta.x;
			mRect.mMin.y.mAbsolute += offsetDelta.y;
			mRect.mMax.x.mAbsolute += offsetDelta.x;
			mRect.mMax.y.mAbsolute += offsetDelta.y;
			mOffset = offset;
			selfRectChanged();
			setDirty(true);
		}
	}
	Window* getHoverWindow(const VECTOR2& position, const bool& useTransform, const bool& forceVisible, const bool& forceHandleInput);
	void connectEvent(const WINDOW_EVENT& event, WindowEventCallback callback, void* userData = NULL)
	{
		mCallbackList.insert(event, std::make_pair(callback, userData));
	}
	int getChildPos(Window* window);					// 得到指定窗口在该窗口中的位置,忽略内部子窗口
	bool moveChildPos(Window* window, const int& destPos);		// 将指定窗口移动到该窗口中的指定位置,忽略内部子窗口
	bool moveChildPos(const std::string& name, const int& destPos);	// 将指定窗口移动到该窗口中的指定位置,忽略内部子窗口
	void setProperty(const std::string& propertyName, const std::string& propertyValue);
	void setVisible(const bool& visible);

	// 将当前窗口下的所有子窗口拷贝到目标窗口下,不会影响目标窗口的原有子窗口
	// useParentNameAsPreName是指定是否在新创建出的子窗口名字前加上父窗口(也就是当前窗口)的名字作为子窗口的新名字
	// preName是当useParentNameAsPreName为false时,在新创建的子窗口名字前加上的字符串
	void copyChildren(Window* destWindow, const bool& useParentNameAsPreName, const std::string& preName);
	// 拷贝自身所有属性到目标窗口
	// 返回值表示该源窗口的属性内预设是否不为空,true则不为空,此时则不应该再拷贝子窗口到目标窗口中,false则为空
	virtual void copyProperty(Window* destWindow);
	void setEnableHandleInput(const bool& enable);
		
	// 设置成员变量
	void setComplexParent(ComplexWindow* parent)		{ mParentComplexWindow = parent; }
	void setParent(Window* parent)						{ mParent = parent; }
	void setAlpha(const float& alpha)					{ mAlpha = alpha; }
	void setColourBlendMode(const BLEND_MODE& mode)		{ mColourBlendMode = mode; }
	void setSymmetryMode(const SYMMETRY_MODE& mode)		{ mSymmetryMode = mode; setTexCoordsDirty(true); }
	void setFrameTremblingOffset(const VECTOR2& offset)	{ mFrameTremblingOffset = offset; setDirty(true); }
	void setFinalSize(const VECTOR2& size)				{ mFinalSize = size; }				// 只能在组件中使用,其他地方使用无效
	void setFinalPosition(const VECTOR2& position)		{ mFinalPosition = position; }		// 只能在组件中使用,其他地方使用无效
	void setHSLOffset(const VECTOR3& hsl)				{ mHSLOffset = hsl; }
	void setColour(const VECTOR3& colour)				{ mColour = colour; }
	void setMatrixTransformCenter(const VECTOR3& center){ mTransformCenter = center; }
	void setWireframeColour(const VECTOR4& colour)		{ mWireframeColour = colour; }
	void setCoordinateColour(const VECTOR4& colour)		{ mCoordinateColour = colour; }
	void setRotateAround(const bool& around)			{ mRotateAround = around; }
	void setInnerChild(const bool& inner)				{ mIsInnerChild = inner; }
	virtual void setFocus(const bool& focus)			{ mFocus = focus; }
	void setShowWireframe(const bool& show)				{ mShowWireframe = show; }
	void setShowCoordinate(const bool& show)			{ mShowCoordinate = show; }
	void setWaterEffect(const bool& effect)				{ mWaterEffect = effect; }
	void setTexCoordsDirty(const bool& dirty)			{ mTexCoordDirty = dirty; }
	void setRenderBaseWireframe(const bool& renderBase)	{ mRenderBaseWireframe = renderBase; }
	void setDiriveParentRotation(const bool& dirive)	{ mDiriveParentRotation = dirive; }
		
	// 获得成员变量
	const std::string& getTypeName()				{ return mTypeName; }
	const std::string& getPrefabName()				{ return mPrefabName; }
	const VECTOR2& getFrameTremblingOffset()		{ return mFrameTremblingOffset; }
	const VECTOR2& getLocalPixelPosition()			{ return mLocalPixelPosition; }
	const VECTOR2& getMatrixScale()					{ return mLocalScale; }
	const VECTOR2& getFinalSize()					{ return mFinalSize; }		// 这里返回的可能是没有更新过的
	const VECTOR2& getPixelPosition()				{ return mPixelPosition; }	// 这里返回的可能是没有更新过的
	const VECTOR2& getFinalPosition()				{ return mFinalPosition; }	// 这里返回的可能是没有更新过的
	const VECTOR2& getPixelSize()					{ return mPixelSize; }		// 这里返回的可能是没有更新过的
	const VECTOR3& getColour()						{ return mColour; }
	const VECTOR3& getHSLOffset()					{ return mHSLOffset; }
	const VECTOR3& getMatrixTranslation()			{ return mLocalTranslation; }
	const VECTOR3& getMatrixTransformCenter()		{ return mTransformCenter; }
	const VECTOR4& getWireframeColour()				{ return mWireframeColour; }
	const VECTOR4& getCoordinateColour()			{ return mCoordinateColour; }
	const MATRIX3& getMatrixRotation()				{ return mLocalRotation; }
	const MATRIX4& getFinalMatrix()					{ return mFinalMatrix; }
	const txDim& getAutoHeight()					{ return mAutoHeight; }
	const txDim& getAutoWidth()						{ return mAutoWidth; }
	const txURect& getRect()						{ return mRect; }
	const txIPoint& getOffset()						{ return mOffset; }
	const bool& getWaterEffect()					{ return mWaterEffect; }
	const bool& getEnableHandleInput()				{ return mHandleInputEnable; }
	const bool& getRotateAround()					{ return mRotateAround; }
	const bool& getWindowVisible()					{ return mVisible; }		// 获取该窗口是否可见,不考虑父窗口
	const bool& isComplexWindow()					{ return mIsComplexWindow; }
	const bool& isInnerChild()						{ return mIsInnerChild; }
	const bool& isFocus()							{ return mFocus; }
	const bool& getShowWireframe()					{ return mShowWireframe; }
	const bool& getShowCoordinate()					{ return mShowCoordinate; }
	const bool& isRenderBaseWireframe()				{ return mRenderBaseWireframe; }
	const bool& getTexCoordsDirty()					{ return mTexCoordDirty; }
	const bool& getDiriveParentRotation()			{ return mDiriveParentRotation; }
	const float& getAlpha()							{ return mAlpha; }
	ComplexWindow* getComplexParent()				{ return mParentComplexWindow; }
	Window* getParent()								{ return mParent; }
	Layout* getLayout()								{ return mParentLayout; }
	const WINDOW_TYPE& getType()					{ return mType; }
	const BLEND_MODE& getColourBlendMode()			{ return mColourBlendMode; }
	const SYMMETRY_MODE& getSymmetryMode()			{ return mSymmetryMode; }
	const AUTO_SIZE_TYPE& getAutoSizeType()			{ return mAutoSizeType; }
	txVector<Window*>& getChildren()				{ return mChildren; }	// 获得子窗口列表,不包含复合窗口中的内部子窗口

	// 公有通知函数
	// 基类的得到资源列表,只有非复合窗口才有用,复合窗口不能重写该虚函数
	virtual void getUsedResourceList(txVector<std::string>& resourceList){}
	// 子窗口矩形改变时对父窗口发出的通知
	virtual void notifyChildRectChanged(Window* child){}
	// 具有图片的子窗口图片大小改变时对父窗口发出的通知
	virtual void notifyChildTextureSizeChanged(Window* child){}
	virtual void notifyChildTextureChanged(Window* child){}
	// 通知该窗口需要更新纹理坐标数据
	virtual void notifyTexCoordNeedUpdated(){}
	// 通知该窗口父窗口被销毁了
	void notifyParentDestroied(){ mParent = NULL; }

	// 处理键盘鼠标输入消息
	virtual void processKey(const float& elapsedTime){}
	virtual void onLeftDown(const VECTOR2& position){}
	virtual void onRightDown(const VECTOR2& position){}
	virtual void onLeftUp(const VECTOR2& position){}
	virtual void onRightUp(const VECTOR2& position){}
	virtual void onMouseMove(const VECTOR2& position){}
	virtual void onMouseEnter(){ pushWindowEvent(WE_MOUSE_ENTER); }
	virtual void onMouseLeave(){ pushWindowEvent(WE_MOUSE_LEAVE); }
	virtual void onReturn(){}
	virtual void setText(const std::string& text, const bool& async){}
	virtual const std::string& getText() { return EMPTY_STRING; }
protected:
	virtual void initProperty();
	virtual void selfRectChanged();
	void pushWindowEvent(const WINDOW_EVENT& event)
	{
		auto iter = mCallbackList.find(event);
		if (iter != mCallbackList.end())
		{
			iter->second.first(this, iter->second.second);
		}
	}

	// 私有通知函数
	virtual void notifyChildNameChanged(const std::string& oldName, Window* window);	// 通知该窗口子窗口的名字改变了
	void notifyChildDestroied(const std::string& childName);	// 通知该窗口子窗口被销毁了
	void notifyChildDetached(const std::string& childName);	// 通知该窗口子窗口断开了联系
	void notifyChildPropertyChanged(const std::string& propertyName, const std::string& newValue); // 通知该窗口子窗口的属性改变
	void notifyChildAdded();		// 通知该窗口有窗口添加或者子窗口有窗口添加
	void notifyChildRemoved();		// 通知该窗口子窗口(包括子窗口的子窗口)被移除了
	virtual void notifyPixelSizeChanged(){}
	virtual void notifyWindowRectChanged(){}
	void refreshWindowRect();
	void refreshWindowTransform();
	void forceRefresh();
	void getFinalPosSize(VECTOR2& finalPos, VECTOR2& finalSize);
protected:
	GLfloat mWindowVertices[UI_VERTEX_COUNT * 3];				// 窗口渲染顶点坐标
	GLfloat mTexCoords[UI_TEXTURE_COORD_COUNT * 2];				// 纹理坐标
	GLfloat mWireframeVertices[UI_VERTEX_COUNT * 3];			// 边框的顶点坐标
	GLfloat mCoordinateVertices[UI_VERTEX_COUNT * 3];			// 坐标系顶点坐标
	static GLushort mWindowIndices[UI_INDEX_COUNT];				// 窗口顶点索引
	static GLushort mWireFrameIndices[WIREFRAME_INDEX_COUNT];	// 边框顶点索引
	static GLushort mCoordinateIndices[COORDINATE_INDEX_COUNT];	// 坐标系顶点索引
	txURect mRect;							// 窗口矩形区域
	bool mVisible;							// 是否显示，如果不显示，则不渲染也不更新
	bool mFocus;							// 是否为当前获得输入焦点的窗口
	bool mRotateAround;						// 父窗口旋转时,是否会改变自己在父窗口中的位置,true则窗口会围绕父窗口中心旋转,false则会绕自己中心旋转
	bool mDiriveParentRotation;				// 窗口的旋转是否会受到父窗口旋转的影响
	bool mDirty;							// 窗口是否需要更新
	bool mTexCoordDirty;					// 纹理坐标是否需要更新
	bool mHandleInputEnable;				// 窗口是否接受输入事件
	bool mIsComplexWindow;					// 是否是复合窗口
	bool mIsInnerChild;						// 是否是一个内部子窗口,即便是复合窗口下的第一级子窗口的子窗口也会认为是内部子窗口,虽然也可以通过复合父窗口是否为空来判断是否是内部子窗口,但是这个标记应该还有其他作用
	bool mWaterEffect;						// 是否启动水波效果
	bool mRenderBaseWireframe;				// 子类的线框渲染是否由基类负责
	bool mShowWireframe;					// 是否渲染窗口边框
	bool mShowCoordinate;					// 是否渲染窗口本地坐标系
	float mWaterTime;						// 水波效果更新时间
	float mAlpha;							// 窗口渲染透明度
	ComplexWindow* mParentComplexWindow;	// 所属的复合窗口,只有内部子窗口才会有
	Window* mParent;						// 父窗口
	Layout* mParentLayout;					// 窗口所属的布局
	std::string mPrefabName;				// 关联的预设名
	std::string mTypeName;					// 窗口类型名
	WINDOW_TYPE mType;						// 窗口类型
	VECTOR2 mPixelPosition;					// 窗口原始的像素位置
	VECTOR2 mPixelSize;						// 窗口原始的像素大小
	VECTOR2 mFinalPosition;					// 窗口最终的像素位置
	VECTOR2 mFinalSize;						// 窗口最终的像素大小
	VECTOR2 mLocalPixelPosition;			// 窗口相对于父窗口的像素坐标
	VECTOR2 mFrameTremblingOffset;			// 边框震动的偏移量,只在最终渲染时才会使用的,不会影响其他的任何属性,只是改变最终的渲染效果,数字窗口和文本窗口暂不支持边框震动
	VECTOR3 mColour;						// 窗口的附加颜色
	VECTOR3 mHSLOffset;						// 窗口的HSL偏移,色相,饱和度,亮度
	VECTOR3 mTransformCenter;				// 窗口的变换中心,x,y是一个相对值,实际值会乘以窗口的最终宽高,z是一个实际值
	VECTOR4 mWireframeColour;				// 窗口边框颜色
	VECTOR4 mCoordinateColour;				// 窗口本地坐标系颜色
	BLEND_MODE mColourBlendMode;			// 窗口附加颜色叠加方式
	SYMMETRY_MODE mSymmetryMode;			// 窗口渲染内容的对称方式,当窗口纹理不是一整张图片时,对称效果会出现错误
	AUTO_SIZE_TYPE mAutoSizeType;			// 窗口大小自动计算方式
	txDim mAutoHeight;						// 自动计算高度时的计算方式
	txDim mAutoWidth;						// 自动计算宽度时的计算方式
	txIPoint mOffset;						// 渲染偏移量,设置偏移量会改变自己的mRect值
	txMap<WINDOW_EVENT, std::pair<WindowEventCallback, void*> > mCallbackList;
	txVector<Window*> mChildren;			// 用于更新和渲染窗口的所有子窗口列表,保存着子窗口之间的顺序,不包含内部子窗口
	txMap<std::string, Window*> mChildrenMap;// 用于查找的子窗口列表,没有子窗口的顺序,不包含内部子窗口
	// 以下是窗口额外的变换,在所有的操作之后,也在mFrameTremblingOffset之后才会使用,如果能不使用该变量就能实现的功能就尽量不要使用该变量
	VECTOR2 mLocalScale;					// 窗口本地的缩放
	MATRIX3 mLocalRotation;					// 窗口本地的旋转
	VECTOR3 mLocalTranslation;				// 窗口本地的平移
	MATRIX4 mFinalMatrix;					// 窗口最终的变换,是计算了父窗口变换后的结果
};

#endif